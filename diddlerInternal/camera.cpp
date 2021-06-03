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

namespace camera {
    GLuint image_texture;
    bool isinit = false;

    void initTexture() {
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);
    }


	void updateCameraFrame(float* pixels, int resolution, float min, float max) {

        if (!pixels) {
            return;
        }

        byte* pixelData = new byte[(resolution * resolution) * 3];
        int pixelPtr = 0;

        for (int pxOffset = 0; pxOffset < (resolution * resolution); pxOffset++) {
            float thisPxFloat = pixels[pxOffset];

            if (thisPxFloat > 0.f) {
                float diff2 = thisPxFloat / max;
                int pxVal = 255 -  (diff2 * 255);
                pixelData[pixelPtr] = (byte)pxVal;
                pixelData[pixelPtr + 1] = (byte)pxVal;
                pixelData[pixelPtr + 2] = (byte)pxVal;
            }
            else {
                pixelData[pixelPtr] = 0x00;
                pixelData[pixelPtr + 1] = 0x00;
                pixelData[pixelPtr + 2] = 0x00;
            }
            pixelPtr += 3;
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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)pixelData);

        free(pixelData);



	}

    void drawCameraWindow() {
        ImGui::Begin("Camera");

        ImGui::Image((void*)image_texture, ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 80));
        ImGui::SliderInt("Resolution", &toolgun::cameraResolution, 32, 512);


        ImGui::End();
    }
}
