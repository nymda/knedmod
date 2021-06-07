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

typedef unsigned int uint;

namespace camera {
    GLuint image_texture;
    bool isinit = false;

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

	void updateCameraFrame(float* pixels, int resolution, float min, float max, bool saveSnapshot) {

        if (!pixels) {
            return;
        }

        byte* pixelData = new byte[(resolution * resolution) * 4];
        int pixelPtr = 0;

        for (int pxOffset = 0; pxOffset < (resolution * resolution); pxOffset++) {
            float thisPxFloat = pixels[pxOffset];

            if (thisPxFloat > 0.f) {
                float diff2 = (thisPxFloat) / max;
                int pxVal = 255 - ((diff2) * 255);
                pixelData[pixelPtr] = (byte)pxVal;
                pixelData[pixelPtr + 1] = (byte)pxVal;
                pixelData[pixelPtr + 2] = (byte)pxVal;
                pixelData[pixelPtr + 3] = 0xff;
            }
            else {
                pixelData[pixelPtr] = 0x00;
                pixelData[pixelPtr + 1] = 0x00;
                pixelData[pixelPtr + 2] = 0x00;
                pixelData[pixelPtr + 3] = 0xff;
            }
            pixelPtr += 4;
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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixelData);

        if (saveSnapshot) {
            createBitmap((DWORD*)pixelData, resolution);
        }

        free(pixelData);

	}

    void updateCameraFrameColor(byte* pixels, int resolution, float min, float max, bool saveSnapshot) {

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

        //free(pixels);

    }

    void drawCameraWindow() {
        ImGui::Begin("Camera");

        ImGui::Image((void*)image_texture, ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 40));

        ImGui::End();
    }

    float* pixels = nullptr;
    byte* pixelsColor = nullptr;
    float minDist = 1000.f;
    float maxDist = 0.f;

    glm::vec3 getSingleScreenVector(float x, float y, glm::mat4x4& vMatrix, glm::mat4x4& pMatrix) {
        glm::vec2 ray_nds = glm::vec2(x, y);
        glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
        glm::mat4 invProjMat = glm::inverse(pMatrix);
        glm::vec4 eyeCoords = invProjMat * ray_clip;
        eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
        glm::mat4 invViewMat = glm::inverse(vMatrix);
        glm::vec4 rayWorld = invViewMat * eyeCoords;
        return glm::normalize(glm::vec3(rayWorld));
    }

    void quatCameraOutline(int resolution, float iFov){

        int res = resolution;
        float fov = iFov;
        minDist = 1000.f;
        maxDist = 0.f;

        free(pixelsColor);
        pixelsColor = new byte[(res * res) * 4];

        td::Color red{ 1.f, 0.f, 0.f, 1.f };
        td::Color green{ 0.f, 1.f, 0.f, 1.f };
        td::Color blue{ 0.f, 0.f, 1.f, 1.f };
        td::Color white{ 1.f, 1.f, 1.f, 1.f };

        glm::vec2 directions[] = { glm::vec2(2, 2), glm::vec2(-2, 2), glm::vec2(2, -2), glm::vec2(-2, -2), };

        RaycastFilter filter{ 0 };
        filter.m_Mask = -1;
        filter.m_RejectTransparent = true;

        glm::quat camera_rotation_bl = *(glm::quat*)(&glb::player->cameraQuat);
        glm::vec3 raycast_dir_bl = camera_rotation_bl * glm::vec3(0, 0, -1);

        td::VoxelsPaletteInfo palOut = {};
        raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, &filter, &palOut);

        glm::vec3 glCameraPos = glm::vec3(glb::player->cameraPosition.x, glb::player->cameraPosition.y, glb::player->cameraPosition.z);
        glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
        glm::mat4x4 vmatrix = glm::lookAt(glCameraPos, glTarget, glm::vec3(0, 1, 0 ));
        glm::mat4x4 pmatrix = glm::perspective(50.f, 1.f, 1.f, 1000.f);

        int pixelOffset = 0;

        //glm::vec3 vecTL = getSingleScreenVector((fov / 2.f), -(fov / 2.f), vmatrix, pmatrix);
        //glm::vec3 vecTR = getSingleScreenVector(-(fov / 2.f), -(fov / 2.f), vmatrix, pmatrix);
        //glm::vec3 vecBL = getSingleScreenVector((fov / 2.f), (fov / 2.f), vmatrix, pmatrix);
        //glm::vec3 vecBR = getSingleScreenVector(-(fov / 2.f), (fov / 2.f), vmatrix, pmatrix);
        //
        //rd = raycaster::castRayManual(glb::player->cameraPosition, { vecTL.x, vecTL.y, vecTL.z }, &filter);
        //drawCube(rd.worldPos, 0.05f, white);
        //
        //rd = raycaster::castRayManual(glb::player->cameraPosition, { vecTR.x, vecTR.y, vecTR.z }, &filter);
        //drawCube(rd.worldPos, 0.05f, white);
        //
        //glm::vec3 vHalf = glm::normalize((vecTL + vecTR) * 0.5f);
        //
        //rd = raycaster::castRayManual(glb::player->cameraPosition, { vHalf.x, vHalf.y, vHalf.z }, &filter);
        //drawCube(rd.worldPos, 0.05f, green);
        //
        //rd = raycaster::castRayManual(glb::player->cameraPosition, { vecBL.x, vecBL.y, vecBL.z }, &filter);
        //drawCube(rd.worldPos, 0.05f, white);
        //
        //rd = raycaster::castRayManual(glb::player->cameraPosition, { vecBR.x, vecBR.y, vecBR.z }, &filter);
        //drawCube(rd.worldPos, 0.05f, white);
        //
        //glm::vec3 vHalf2 = glm::normalize((vecTL + vecBL) * 0.5f);
        //
        //rd = raycaster::castRayManual(glb::player->cameraPosition, { vHalf2.x, vHalf2.y, vHalf2.z }, &filter);
        //drawCube(rd.worldPos, 0.05f, green);

        float fovSplit = 1.f / res;

        glm::mat4 invProjMat = glm::inverse(pmatrix);
        glm::mat4 invViewMat = glm::inverse(vmatrix);

        for (int y = res; y > 0; y--) {
            for (int x = 0; x < res; x++) {

                float comX = (fov / 2.f) - (x * (fov / res));
                float comY = (fov / 2.f) - (y * (fov / res));

                glm::vec2 ray_nds = glm::vec2(comX, comY);
                glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

                glm::vec4 eyeCoords = invProjMat * ray_clip;
                eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

                glm::vec4 rayWorld = invViewMat * eyeCoords;
                glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

                td::VoxelsPaletteInfo palOut = {};
                rd = raycaster::castRayManual(glb::player->cameraPosition, { rayDirection.x, rayDirection.y, rayDirection.z }, &filter, &palOut);

                if (res <= 512) {
                    drawCube(rd.worldPos, 0.05f, white);
                }


                float thisDist = rd.distance;

                if (thisDist >= 1000.f) {
                    pixelsColor[pixelOffset] = (byte)(0);
                    pixelsColor[pixelOffset + 1] = (byte)(77);
                    pixelsColor[pixelOffset + 2] = (byte)(77);
                    pixelsColor[pixelOffset + 3] = (byte)(255);
                }
                else {
                    pixelsColor[pixelOffset] = (byte)(palOut.m_Color.m_R * 255);
                    pixelsColor[pixelOffset + 1] = (byte)(palOut.m_Color.m_G * 255);
                    pixelsColor[pixelOffset + 2] = (byte)(palOut.m_Color.m_B * 255);
                    pixelsColor[pixelOffset + 3] = (byte)(palOut.m_Color.m_A * 255);
                }
                pixelOffset+=4;
            }
        }

        camera::updateCameraFrameColor(pixelsColor, res, minDist, maxDist, toolgun::takeSnapshot);
    }
}
