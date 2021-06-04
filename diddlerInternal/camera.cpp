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
                img[i] = (char)(color & 0xFF);
                img[i + 1] = (char)((color >> 8) & 0xFF);
                img[i + 2] = (char)((color >> 16) & 0xFF);
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

    void drawCameraWindow() {
        ImGui::Begin("Camera");

        ImGui::Image((void*)image_texture, ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 80));
        ImGui::SliderInt("Resolution", &toolgun::cameraResolution, 32, 512);
        ImGui::Checkbox("Save image", &toolgun::takeSnapshot);

        ImGui::End();
    }
}
