#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include "types.h"
#include "Global.h"
#include "Raycaster.h"
#include "drawCube.h"
#include "objectSpawner.h"
#include <filesystem>
#include "stb_image.h"
#include "maths.h"

namespace fs = std::filesystem;


inline bool exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

//this code is more fragile than your average twitter liberal
//DONT FUCKING TOUCH IT
namespace spawner {

    std::string currentSpawngunObject = "vox\\crate\\object.vox";
    KMSpawnedObject lastSpawnedObject{};
    td::Color white{ 1.f, 1.f, 1.f, 1.f };
    bool spawnOnce = true;

    void spawnObjectProxy(std::string path) {
        lastSpawnedObject = spawnEntity(path);
    }

    void processMostRecentObject() {
        if (lastSpawnedObject.body != 0 && lastSpawnedObject.shape != 0 && lastSpawnedObject.isInitByGame == false) {
            if (lastSpawnedObject.shape->min.x == 0 && lastSpawnedObject.shape->max.x == 0) {
                return;
            }
            else {
                td::Vec4 target = castRayPlayer();
                td::Vec3 objectMin = lastSpawnedObject.shape->min;
                td::Vec3 objectMax = lastSpawnedObject.shape->max;
                td::Vec3 centerBottom = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMin.y, objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                drawCube(objectMin, 0.25, white);
                drawCube(objectMax, 0.25, white);
                drawCube(centerBottom, 0.10, white);
                drawCube(lastSpawnedObject.body->positionC0, 0.10, white);

                td::Vec3 objectSize = { (objectMax.x - objectMin.x), (objectMax.y - objectMin.y), (objectMax.z - objectMin.z) };

                lastSpawnedObject.body->positionC0 = { target.x - (objectSize.x / 2), target.y, target.z - (objectSize.z / 2) };

                lastSpawnedObject.isInitByGame = true;

                std::cout << "X: " << std::to_string(lastSpawnedObject.body->rotationQuatC0.x) << " Y: " << std::to_string(lastSpawnedObject.body->rotationQuatC0.y) << " Z: " << std::to_string(lastSpawnedObject.body->rotationQuatC0.z) << " W: " << std::to_string(lastSpawnedObject.body->rotationQuatC0.w) << std::endl;
            }
        }
    }

    std::vector<LoadedSpawnableObject> enumerateSpawnableObjects() {
        std::vector<LoadedSpawnableObject> returnObj = {};
        for (const auto& file : fs::directory_iterator("vox"))
        {
            bool foundVox = false;
            bool foundImage = false;
            char* currentVoxPath;
            char* currentImagePath;

            for (const auto& fileSubdir : fs::directory_iterator(file.path()))
            {
                if (strcmp(fileSubdir.path().filename().string().c_str(), "object.vox") == 0)
                {
                    foundVox = true;
                }
                if (strcmp(fileSubdir.path().filename().string().c_str(), "object.png") == 0)
                {
                    foundImage = true;
                }

                if (foundVox && foundImage) {
                    LoadedSpawnableObject lso{};
                    lso.basePath = file.path().string();
                    lso.imagePath = lso.basePath + "\\object.png";
                    lso.voxPath = lso.basePath + "\\object.vox";

                    int imgSize = 255;
                    LoadTextureFromFile(lso.imagePath.c_str(), &lso.imageTexture, &imgSize, &imgSize);

                    returnObj.push_back(lso);
                }
            }
        }
        return returnObj;
    }

    void handleSpawnerWeapon() {
        const char* spawngunName = "spawngun";
        if (memcmp(glb::player->heldItemName, spawngunName, 8) == 0) {
            td::Vec4 target = castRayPlayer();
            drawCube({ target.x, target.y, target.z }, 0.25, white);

            if (glb::player->isAttacking == true) {
                if (spawnOnce) {
                    spawnOnce = false;
                    lastSpawnedObject = spawnEntity(currentSpawngunObject);
                }
            }
            else {
                spawnOnce = true;
            }
        }
    }

    KMSpawnedObject spawnEntity(std::string filepath) {
        if (!exists(filepath)) {
            std::cout << "no file" << std::endl;
            return {false, 0, 0};
        }

        td::small_string file_path((char*)(filepath).c_str());

        std::cout << "make spawnVox work, o magic man" << std::endl; //this std::cout is literally required for the next function to run. i have no fucking idea why. 

        uintptr_t vox = glb::oSpawnVox(file_path, 80000008000000, 1.0f); //what is 80000008000000? why is it here? what is its purpose? why do we exist? why would a loving god create so much agony?

        if (vox == 0x00) {
            std::cout << "Created vox was null" << std::endl;
            return { false, 0, 0 };
        }

        glb::oCreateTexture(vox);

        glb::oCreatePhysics(vox);

        uintptr_t BODY = glb::oTMalloc(0x100u);

        glb::oB_Constructor((uintptr_t)BODY, (uintptr_t)nullptr);
        glb::oSetDynamic((uintptr_t)BODY, true);

        uintptr_t SHAPE = glb::oTMalloc(0xB0u);
        glb::oS_Constructor((uintptr_t)SHAPE, (uintptr_t)BODY);

        *(uintptr_t*)(SHAPE + 0x90) = vox;

        glb::oUpdateShapes((uintptr_t)BODY);

        float voxX = *(float*)(vox + 0x0044 + 4);
        float voxY = *(float*)(vox + 0x0044 + 8);
        float voxZ = *(float*)(vox + 0x0044 + 12);

        std::cout << "X: " << voxX << " Y: " << voxY << " Z: " << voxZ << std::endl;

        float bodyX = ((voxX / 10.f) / 2.f);
        float bodyY = ((voxY / 10.f) / 2.f);
        float bodyZ = ((voxZ / 10.f) / 2.f);

        std::cout << "X: " << bodyX << " Y: " << bodyY << " Z: " << bodyZ << std::endl;

        td::Vec4 ray = castRayPlayer();

        td::Vec4 newRot = { -0.5, -0.5, -0.5, 0.5 };

        std::cout << "======================================" << std::endl;
        std::cout << "Shape address: 0x" << std::hex << SHAPE << std::endl;
        std::cout << "Shape min:       " << "X: " << ((TDShape*)SHAPE)->min.x << " Y: " << ((TDShape*)SHAPE)->min.y << " Z: " << ((TDShape*)SHAPE)->min.z << std::endl;
        std::cout << "Shape max:       " << "X: " << ((TDShape*)SHAPE)->max.x << " Y: " << ((TDShape*)SHAPE)->max.y << " Z: " << ((TDShape*)SHAPE)->max.z << std::endl;
        std::cout << "Body address:  0x" << std::hex << BODY << std::endl;
        std::cout << "Vox address:   0x" << std::hex << vox << std::endl;

        *(float*)(BODY + 0x28) = 0;
        *(float*)(BODY + 0x28 + 4) = 0;
        *(float*)(BODY + 0x28 + 8) = 0;
        *(float*)(BODY + 0x28 + 12) = newRot.x;
        *(float*)(BODY + 0x28 + 16) = newRot.y;
        *(float*)(BODY + 0x28 + 20) = newRot.z;
        *(float*)(BODY + 0x28 + 24) = newRot.w;

        return { false, (TDShape*)SHAPE, (TDBody*)BODY };
    }

    void spawnTestEntity() {
        spawnEntity("knedcube.vox");
    }
}

