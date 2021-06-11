#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawCube.h"
#include "toolgun.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "dotProjector.h"

typedef unsigned int uint;

namespace camera {
    GLuint image_texture;
    bool isinit = false;
    bool colourMode = true;
    bool mono = false;
    bool interlaceMode = true;
    bool transparency = true;

    size_t currentFramebufferSize = 0;
    //byte* frameBuffer = nullptr;
    RaycastFilter rcf = { };

    void write_bmp(const std::string path, const uint width, const uint height, const int* const data) {
        const uint pad = (4 - (3 * width) % 4) % 4, filesize = 54 + (3 * width + pad) * height; // horizontal line must be a multiple of 4 bytes long, header is 54 bytes
        char header[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
        for (uint i = 0; i < 4; i++) {
            header[2 + i] = (char)((filesize >> (8 * i)) & 255);
            header[18 + i] = (char)((width >> (8 * i)) & 255);
            header[22 + i] = (char)((height >> (8 * i)) & 255);
        }
        char* img = new char[filesize];
        for (uint i = 0; i < 54; i++) img[i] = header[i];
        for (uint y = 0; y < height; y++) {
            for (uint x = 0; x < width; x++) {
                const int color = data[x + (height - 1 - y) * width];
                const int i = 54 + 3 * x + y * (3 * width + pad);
                img[i + 2] = (char)(color & 0xFF);
                img[i + 1] = (char)((color >> 8) & 0xFF);
                img[i] = (char)((color >> 16) & 0xFF);
            }
            for (uint p = 0; p < pad; p++) img[54 + (3 * width + p) + y * (3 * width + pad)] = 0;
        }
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file.write(img, filesize);
        file.close();
        delete[] img;
    }

    float randFloat(float min, float max) {
        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }

    std::string createBitmap(DWORD* frameBuffer, int resolution) {
        auto p1 = std::chrono::system_clock::now();
        std::string filename = "lidar/LIDARCAM_" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count()) + ".bmp";
        write_bmp(filename, resolution, resolution, (int*)frameBuffer);
        return filename;
    }

    void initTexture() {
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);
    }

    void interlacedImage(byte* frameBuffer, int resolution, bool flip, float fov, float aspect, glm::quat* camRotation, td::Vec3 camPosition, td::Vec3 forwardVector, td::Vec3 upVector, RaycastFilter* filter) {
        //construct a new viewMatrix and projectionMatrix
        //could use the matrices already created by the game, if i can find them

        glm::vec3 raycast_dir_bl = *camRotation * glm::vec3(forwardVector.x, forwardVector.y, forwardVector.z);
        raycaster::rayData rd = raycaster::castRayManual(camPosition, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, filter);
        glm::vec3 glCameraPos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);
        glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
        glm::mat4x4 vmatrix = glm::lookAt(glCameraPos, glTarget, glm::vec3(upVector.x, upVector.y, upVector.z));
        glm::mat4x4 pmatrix = glm::perspective(50.f, aspect, 1.f, 1000.f);
        glm::mat4 invProjMat = glm::inverse(pmatrix);
        glm::mat4 invViewMat = glm::inverse(vmatrix);

        int pixelOffset = 0;

        //draw from left to right, bottom to top. This matches openGLs pixel format. 
        for (int y = resolution; y > 0; y--) {
            if (((y & 2) == 0) == flip) {
                pixelOffset += resolution * 4;
                continue;
            }

            for (int x = 0; x < resolution; x++) {
                //slightly eh implementation of Stochastic Sampling. Thanks Josh!
                float pxSize = (fov / resolution);
                float comX = (fov / 2.f) - (x * pxSize) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));
                float comY = (fov / 2.f) - (y * pxSize) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));

                glm::vec2 ray_nds = glm::vec2(comX, comY);
                glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
                glm::vec4 eyeCoords = invProjMat * ray_clip;
                eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

                glm::vec4 rayWorld = invViewMat * eyeCoords;
                glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
                rd = raycaster::castRayManual(camPosition, { rayDirection.x, rayDirection.y, rayDirection.z }, filter);

                if (rd.distance < 1000.f) {
                    int iThisDist = (rd.distance) / 2;

                    if (mono) {
                        int avg = (((rd.palette.m_Color.m_R + rd.palette.m_Color.m_G + rd.palette.m_Color.m_B) / 3.f) * 255) - iThisDist;
                        if (avg < 0) {
                            avg = 0;
                        }
                        frameBuffer[pixelOffset + 0] = (byte)(avg);
                        frameBuffer[pixelOffset + 1] = (byte)(avg);
                        frameBuffer[pixelOffset + 2] = (byte)(avg);
                        frameBuffer[pixelOffset + 3] = (byte)(255);
                    }
                    else {
                        int iColourR = ((rd.palette.m_Color.m_R * 255) - iThisDist);
                        if (iColourR < 0) { iColourR = 0; }
                        int iColourG = ((rd.palette.m_Color.m_G * 255) - iThisDist);
                        if (iColourG < 0) { iColourG = 0; }
                        int iColourB = ((rd.palette.m_Color.m_B * 255) - iThisDist);
                        if (iColourB < 0) { iColourB = 0; }

                        frameBuffer[pixelOffset + 0] = (byte)(iColourR);
                        frameBuffer[pixelOffset + 1] = (byte)(iColourG);
                        frameBuffer[pixelOffset + 2] = (byte)(iColourB);
                        frameBuffer[pixelOffset + 3] = (byte)(255);
                    }
                }
                else {
                    if (mono) {
                        frameBuffer[pixelOffset + 0] = (byte)(128); //0
                        frameBuffer[pixelOffset + 1] = (byte)(128); //77
                        frameBuffer[pixelOffset + 2] = (byte)(128); //77
                        frameBuffer[pixelOffset + 3] = (byte)(255);
                    }
                    else {
                        frameBuffer[pixelOffset + 0] = (byte)(0);
                        frameBuffer[pixelOffset + 1] = (byte)(77);
                        frameBuffer[pixelOffset + 2] = (byte)(77);
                        frameBuffer[pixelOffset + 3] = (byte)(255);
                    }
                }
                pixelOffset+=4;
            }
        }
    }


    void constructDistanceFrame(dotProjector::pixelResponse* pixelResponse, int resolution, float min, float max) {
        if (!isinit) {
            initTexture();
        } 

        byte* frameBuffer = new byte[(resolution * resolution) * 4];
        size_t frameBufferBytePointer = 0;

        for (int i = 0; i < pixelResponse->size; i++) {
            if (pixelResponse->data[i].dist < 1000.f) {
                float diff2 = (pixelResponse->data[i].dist) / max;
                int pxVal = 255 - ((diff2) * 255);

                frameBuffer[frameBufferBytePointer + 0] = (byte)pxVal;
                frameBuffer[frameBufferBytePointer + 1] = (byte)pxVal;
                frameBuffer[frameBufferBytePointer + 2] = (byte)pxVal;
                frameBuffer[frameBufferBytePointer + 3] = 0xFF;
            }
            else {
                frameBuffer[frameBufferBytePointer + 0] = 0x00;
                frameBuffer[frameBufferBytePointer + 1] = 0x00;
                frameBuffer[frameBufferBytePointer + 2] = 0x00;
                frameBuffer[frameBufferBytePointer + 3] = 0xFF;
            }

            frameBufferBytePointer += 4;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        #endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)frameBuffer);

        free(frameBuffer);
    }

    void constructColourFrame(dotProjector::pixelResponse* pixelResponse, int resolution, bool enableDistanceFog) {
        if (!isinit) {
            initTexture();
        }

        byte* rgbFrameBuffer = new byte[(resolution * resolution) * 4];
        size_t rgbFrameBufferBytePointer = 0;

        for (int i = 0; i < pixelResponse->size; i++) {
            if (pixelResponse->data[i].dist < 1000.f) {

                int iThisDist = ((int)pixelResponse->data[i].dist) / 2;
                int iColourR = (pixelResponse->data[i].color_R- iThisDist);
                if (iColourR < 0) { iColourR = 0; }
                int iColourG = (pixelResponse->data[i].color_G - iThisDist);
                if (iColourG < 0) { iColourG = 0; }
                int iColourB = (pixelResponse->data[i].color_B - iThisDist);
                if (iColourB < 0) { iColourB = 0; }

                rgbFrameBuffer[rgbFrameBufferBytePointer + 0] = (byte)iColourR;
                rgbFrameBuffer[rgbFrameBufferBytePointer + 1] = (byte)iColourG;
                rgbFrameBuffer[rgbFrameBufferBytePointer + 2] = (byte)iColourB;
                rgbFrameBuffer[rgbFrameBufferBytePointer + 3] = 0xFF;
            }
            else {
                rgbFrameBuffer[rgbFrameBufferBytePointer + 0] = (byte)0;
                rgbFrameBuffer[rgbFrameBufferBytePointer + 1] = (byte)77;
                rgbFrameBuffer[rgbFrameBufferBytePointer + 2] = (byte)77;
                rgbFrameBuffer[rgbFrameBufferBytePointer + 3] = 0xFF;
            }

            rgbFrameBufferBytePointer += 4;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        #endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)rgbFrameBuffer);

        free(rgbFrameBuffer);
    }

    void constructFrameManual(byte* pixels, int resolution, bool saveSnapshot) {

        if (!pixels) {
            return;
        }

        if (!isinit) {
            initTexture();
        }

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
        #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        #endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixels);

        if (saveSnapshot) {
            createBitmap((DWORD*)pixels, resolution);
        }
    }

    void drawCameraWindow() {
        ImGui::Begin("Camera");
        ImGui::Image((void*)image_texture, ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 35));
        ImGui::End();
    }
  
    int lastResolution = 64;
    byte* frameBuffer;
    bool flip = true;
    void updateImageColour(int resolution, float fov) {
        if (interlaceMode) {
            if (resolution != lastResolution || !frameBuffer) {
                free(frameBuffer);
                frameBuffer = new byte[(resolution * resolution) * 4];
            }
            lastResolution = resolution;
            if (transparency) {
                rcf.m_RejectTransparent = true;
            }
            else {
                rcf.m_RejectTransparent = false;
            }

            flip = !flip;
            interlacedImage(frameBuffer, resolution, flip, fov, 1.f, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
            constructFrameManual(frameBuffer, resolution, false);
        }
        else {
            dotProjector::pixelResponse* response = dotProjector::projectDotMatrix(resolution, fov, 1.f, true, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
            if (camera::colourMode) {
                constructColourFrame(response, resolution, false);
            }
            else {
                constructDistanceFrame(response, resolution, response->minDist, response->maxDist);
            }
        }
    }
}
