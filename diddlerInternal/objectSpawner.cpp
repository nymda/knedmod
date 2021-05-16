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
#include <glm/gtc/quaternion.hpp>
#include "toolgun.h"

namespace fs = std::filesystem;

spawner::objectSpawnerParams defaultParams = {};

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

    std::vector<toolgun::fadeShapeOutline> spawnedObjects = {};
    std::vector<KMSpawnedObject> spawnList = {};
    KMSpawnedObject lastSpawnedObject{};
    td::Color white{ 1.f, 1.f, 1.f, 1.f };
    bool spawnOnce = true;

    KMSpawnedObject spawnObjectProxy(std::string path, objectSpawnerParams params) {
        KMSpawnedObject lsp = spawnEntity(path, params);
        lastSpawnedObject = lsp;
        lastSpawnedObject.params = params;
        if (params.pushSpawnList) {
            spawnList.push_back(lsp);
        }

        for (objectAttribute att : params.attributes) {
            //std::cout << "Run oSOA" << std::endl;
            glb::oSOA(lsp.shape, &att.attribute, &att.level);
        }
        
        if (params.animate) {
            toolgun::fadeShapeOutline tmp;
            tmp.shape = lsp.shape;
            tmp.alpha = 255;
            spawnedObjects.push_back(tmp);
        }

        return lsp;
    }

    void cleanup() {
        lastSpawnedObject = {};
        spawnList.clear();
    }

    void processMostRecentObject() {

        bool clearSpawnedObjects = true;
        if (spawnedObjects.size() > 0) {
            for (toolgun::fadeShapeOutline& fso : spawnedObjects) {
                glb::oHighlightShape(glb::renderer, fso.shape, (float)(fso.alpha / 255.f));
                if (fso.alpha > 0) {
                    fso.alpha -= 4;
                    clearSpawnedObjects = false;
                }
            }

            if (clearSpawnedObjects) {
                spawnedObjects.clear();
                clearSpawnedObjects = false;
            }
        }

        if (glb::game) {
            if (glb::game->State == gameState::menu) {
                if (spawnList.size() > 0) {
                    std::cout << "[I] Cleaning spawned objects" << std::endl;
                    cleanup();
                }
            }
        }

        if (lastSpawnedObject.body != 0 && lastSpawnedObject.shape != 0 && lastSpawnedObject.isInitByGame == false) {

            if (lastSpawnedObject.shape->posMin.x == 0 && lastSpawnedObject.shape->posMax.x == 0) {
                return;
            }
            else {
                raycaster::rayData rd = raycaster::castRayPlayer();
                td::Vec3 target = rd.worldPos;
                td::Vec3 objectMin = lastSpawnedObject.shape->posMin;
                td::Vec3 objectMax = lastSpawnedObject.shape->posMax;
                td::Vec3 centerBottom = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMin.y, objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                drawCube(objectMin, 0.25, white);
                drawCube(objectMax, 0.25, white);
                drawCube(centerBottom, 0.10, white);
                drawCube(lastSpawnedObject.body->Position, 0.10, white);

                td::Vec3 objectSize = { (objectMax.x - objectMin.x), (objectMax.y - objectMin.y), (objectMax.z - objectMin.z) };
               
                if (lastSpawnedObject.params.spawnType == objectSpawnType::thrown) {
                    float spawnPosx = (glb::player->cameraPosition.x - (objectSize.x / 2)) + glb::player->cameraEuler().x;
                    float spawnPosy = (glb::player->cameraPosition.y - (objectSize.y / 2)) + glb::player->cameraEuler().y;
                    float spawnPosz = (glb::player->cameraPosition.z - (objectSize.z / 2)) + glb::player->cameraEuler().z;

                    lastSpawnedObject.body->Position = { spawnPosx,  spawnPosy, spawnPosz };
                    lastSpawnedObject.body->Velocity = lastSpawnedObject.params.startVelocity;

                    float roVeloX = (rand() % 20) - 10;
                    float roVeloY = (rand() % 20) - 10;
                    float roVeloZ = (rand() % 20) - 10;
                    lastSpawnedObject.body->RotationVelocity = { roVeloX, roVeloY, roVeloZ };
                }
                else if (lastSpawnedObject.params.spawnType == objectSpawnType::front) {
                    float spawnPosx = (glb::player->cameraPosition.x - (objectSize.x / 2)) + (glb::player->cameraEuler().x * 2.25f);
                    float spawnPosy = (glb::player->cameraPosition.y - (objectSize.y / 2)) + (glb::player->cameraEuler().y * 2.25f);
                    float spawnPosz = (glb::player->cameraPosition.z - (objectSize.z / 2)) + (glb::player->cameraEuler().z * 2.25f);
                    lastSpawnedObject.body->Rotation = lastSpawnedObject.params.customRotation;
                    lastSpawnedObject.body->Velocity = lastSpawnedObject.params.startVelocity;
                    lastSpawnedObject.body->Position = { spawnPosx,  spawnPosy, spawnPosz };



                    //uintptr_t tmpJoint = glb::oTMalloc(208);
                    //glb::oConstructJoint(tmpJoint);
                }
                else {
                    lastSpawnedObject.body->Position = { (target.x - (objectSize.x / 2)), target.y, (target.z - (objectSize.z / 2)) };
                }

                lastSpawnedObject.isInitByGame = true;

            }
        }
    }

    std::vector<objectAttribute> enumAttributes(std::string atPath) {
        std::vector<objectAttribute> processed;
        char splitter = ':';
        std::ifstream file(atPath);
        std::string str;
        std::string file_contents;

        while (std::getline(file, str))
        {
            size_t spPos = str.find(splitter);

            int lenChunkOne = spPos;
            int lenChunkTwo = str.length() - spPos;
            
            std::string chunkOne = str.substr(0, lenChunkOne);
            std::string chunkTwo = str.substr(spPos+1, lenChunkTwo);

            objectAttribute current;

            current.attribute = td::small_string(chunkOne.c_str());
            current.level = td::small_string(chunkTwo.c_str());

            processed.push_back(current);
        }

        return processed;
    }

    std::vector<spawnerCatagory> enumerateSpawnableObjects() {
        std::vector<spawnerCatagory> returnObj = {};
        for (const auto& file : fs::directory_iterator("vox"))
        {
            int voxCount = 0;

            // /\ iterate over each folder within [VOX]
            std::string path = file.path().string();
            std::string catig = path.substr(4, path.size() - 4);
            spawnerCatagory current;
            current.name = catig;

            for (const auto& catagoryFolder : fs::directory_iterator(file.path()))
            {
                // /\ iterate over each subfolder within [VOX / CATIG]
                bool foundVox = false;
                bool foundImage = false;
                bool foundAttrib = false;
                char* currentVoxPath;
                char* currentImagePath;

                //iterate over [VOX / CATIG / ITEM]
                for (const auto& fileSubdir : fs::directory_iterator(catagoryFolder.path()))
                {
                    if (strcmp(fileSubdir.path().filename().string().c_str(), "object.vox") == 0)
                    {
                        foundVox = true;
                    }
                    if (strcmp(fileSubdir.path().filename().string().c_str(), "object.png") == 0)
                    {
                        foundImage = true;
                    }
                    if (strcmp(fileSubdir.path().filename().string().c_str(), "attri.txt") == 0)
                    {
                        foundAttrib = true;
                    }

                    if (foundVox && foundImage) {
                        LoadedSpawnableObject lso{};
                        lso.catagory = catig;
                        lso.basePath = catagoryFolder.path().string();
                        lso.imagePath = lso.basePath + "\\object.png";
                        lso.voxPath = lso.basePath + "\\object.vox";

                        if (foundAttrib) {
                            lso.attributes = enumAttributes(lso.basePath + "\\attri.txt");
                        }

                        int imgSize = 255;
                        LoadTextureFromFile(lso.imagePath.c_str(), &lso.imageTexture, &imgSize, &imgSize);
                        voxCount++;
                        current.objects.push_back(lso);
                    }

                }
            }

            std::cout << "[I] Loaded " << std::to_string(voxCount) << " vox files from: " << catig << std::endl;

            returnObj.push_back(current);
        }
        return returnObj;
    }

    //void handleSpawnerWeapon() {
    //    const char* spawngunName = "spawngun";
    //    if (memcmp(glb::player->heldItemName, spawngunName, 8) == 0) {
    //        raycaster::rayData rd = raycaster::castRayPlayer();
    //        td::Vec3 target = rd.worldPos;
    //        drawCube({ target.x, target.y, target.z }, 0.05, white);

    //        objectSpawnerParams osp{};
    //        osp.customRotation = rd.angle;

    //        if (glb::player->isAttacking == true) {
    //            if (spawnOnce) {
    //                spawnOnce = false;
    //                lastSpawnedObject = spawnObjectProxy(currentSpawngunObject, osp);
    //            }
    //        }
    //        else {
    //            spawnOnce = true;
    //        }
    //    }
    //}

    void deleteLastObject() {
        if (glb::game->State == gameState::ingame) {
            if (spawnList.size() > 0) {
                spawnList.back().shape->Destroy(spawnList.back().shape, true);
                spawnList.back().body->Destroy(spawnList.back().body, true);
                spawnList.pop_back();
            }
        }
    }

    KMSpawnedObject spawnEntity(std::string filepath, objectSpawnerParams osp) {
        if (!exists(filepath)) {
            std::cout << "[E] no file" << std::endl;
            return { defaultParams, false, 0, 0 };
        }

        std::cout << "======================================" << std::endl;

        uintptr_t BODY = 0;

        if (osp.parentBody) {
            BODY = (uintptr_t)osp.parentBody;
        }
        else {
            BODY = glb::oTMalloc(0x232u);
            glb::oB_Constructor((uintptr_t)BODY, (uintptr_t)nullptr);
        }

        glb::oSetDynamic((uintptr_t)BODY, true);
        std::cout << "Body address:  0x" << std::hex << BODY << std::endl;

        td::small_string file_path((char*)(filepath).c_str());

        uintptr_t names2 = glb::oTMalloc(1024);
        td::small_vector<td::small_string> nameOut = {};
        glb::TDreadSubobjects(&file_path, (int*)&nameOut);

        uintptr_t SHAPE = 0;
        uintptr_t vox = 0;

        if (nameOut.size() == 0) {
            nameOut.push_back(td::small_string(""));
        }

        for (td::small_string sub_path : nameOut) {
            SHAPE = glb::oTMalloc(0x176u);
            glb::oS_Constructor((uintptr_t)SHAPE, (uintptr_t)BODY);

            vox = glb::oSpawnVox(&file_path, &sub_path, 1.f);

            if (strcmp(sub_path.c_str(), "") == 0) {
                std::cout << "Subobject: [none] | Shape: " << std::hex << SHAPE << " | Vox: " << std::hex << vox << std::endl;
            }
            else {
                std::cout << "Subobject: " << sub_path.c_str() << " | Shape: " << std::hex << SHAPE << " | Vox: " << std::hex << vox << std::endl;
            }

            if (vox == 0x00) {
                std::cout << "[E] Created vox was null" << std::endl;
                return { defaultParams, false, 0, 0 };
            }

            glb::oCreateTexture(vox);
            glb::oCreatePhysics(vox);

            *(uintptr_t*)(SHAPE + 0x90) = vox;

            if (osp.nocull) {
                *(byte*)(SHAPE + 9) |= 16;
            }
            if (osp.unbreakable) {
                *(byte*)(SHAPE + 9) |= 4;
            }
        }

        glb::oUpdateShapes((uintptr_t)BODY);

        raycaster::rayData rd = raycaster::castRayPlayer();
        td::Vec3 target = rd.worldPos;
        td::Vec4 newRot = { -0.5, -0.5, -0.5, 0.5 };

        td::Vec3 aVec = rd.angle;
        float angle = atan2(aVec.x, aVec.z);
        float qx = aVec.x * sin(angle / 2);
        float qy = aVec.y * sin(angle / 2);
        float qz = aVec.z * sin(angle / 2);
        float qw = cos(angle / 2);

        *(float*)(BODY + 0x28) = 0;
        *(float*)(BODY + 0x28 + 4) = 0;
        *(float*)(BODY + 0x28 + 8) = 0;

        *(float*)(BODY + 0x28 + 12) = newRot.x;
        *(float*)(BODY + 0x28 + 16) = newRot.y;
        *(float*)(BODY + 0x28 + 20) = newRot.z;
        *(float*)(BODY + 0x28 + 24) = newRot.w;

        return { osp, false, (TDShape*)SHAPE, (TDBody*)BODY, (TDVox*)vox };
    }

}

