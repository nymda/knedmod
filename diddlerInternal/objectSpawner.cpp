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
#include <glm/gtx/quaternion.hpp>

namespace fs = std::filesystem;

spawner::objectSpawnerParams defaultParams = {};


std::string getObjectName(std::string path) {

    int tmpOffset = 0;
    int finalOffset = 0;

    while ((tmpOffset = path.find("\\", tmpOffset + 1)) != std::string::npos) {
        finalOffset = tmpOffset;
    }

    return path.substr(finalOffset + 1, path.length() - finalOffset);
}

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

    float voxScale = 1.f;
    std::vector<spawner::spawnerCatagory> spawnerObjectsDatabase;

    std::vector<toolgun::fadeShapeOutline> spawnedObjects = {};
    std::vector<KMSpawnedObject> spawnList = {};
    KMSpawnedObject lastSpawnedObject{};
    td::Color white{ 1.f, 1.f, 1.f, 1.f };
    bool spawnOnce = true;

    bool freeMode = true;
    bool childMode = false;
    bool thrownMode = false;

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
                //drawCube(objectMin, 0.25, white);
                //drawCube(objectMax, 0.25, white);
                //drawCube(centerBottom, 0.10, white);
                //drawCube(lastSpawnedObject.body->Position, 0.10, white);

                td::Vec3 objectSize = { (objectMax.x - objectMin.x), (objectMax.y - objectMin.y), (objectMax.z - objectMin.z) };
               
                if (lastSpawnedObject.params.spawnType == objectSpawnType::thrown) {


                    td::Vec3 voxSize = { (lastSpawnedObject.vox->sizeX / 10.f) * voxScale, (lastSpawnedObject.vox->sizeY / 10.f) * voxScale, (lastSpawnedObject.vox->sizeZ / 10.f) * voxScale }; //this is the vox size in units where 1vx = 1u, convert to 1vx = .1u
                    glm::quat facePlayer = glm::quat(glm::vec3(glb::player->camPitch + 4.71238898025f, glb::player->camYaw, 0));
                    float spawnPosx = (glb::player->cameraPosition.x + glb::player->cameraEuler().x);
                    float spawnPosy = (glb::player->cameraPosition.y + glb::player->cameraEuler().y);
                    float spawnPosz = (glb::player->cameraPosition.z + glb::player->cameraEuler().z);


                    glm::vec3 vx = facePlayer * glm::vec3(1, 0, 0);
                    glm::vec3 vy = facePlayer * glm::vec3(0, 1, 0);
                    glm::vec3 vz = facePlayer * glm::vec3(0, 0, 1);

                    glm::vec3 translation = ((vz * (voxSize.z / 2.f)) + (vy * (voxSize.y / 2.f)) + (vx * (voxSize.x / 2.f)));

                    *(glm::quat*)&lastSpawnedObject.body->Rotation = facePlayer;

                    lastSpawnedObject.body->Position = { spawnPosx - translation.x, spawnPosy - translation.y, spawnPosz - translation.z };

                    float roVeloX = (rand() % 8) - 4;
                    float roVeloY = (rand() % 8) - 4;
                    float roVeloZ = (rand() % 8) - 4;
                    lastSpawnedObject.body->RotationVelocity = { roVeloX, roVeloY, roVeloZ };
                    lastSpawnedObject.body->Velocity = lastSpawnedObject.params.startVelocity;
                }
                else if (lastSpawnedObject.params.spawnType == objectSpawnType::front) {

                    lastSpawnedObject.body->Rotation = lastSpawnedObject.params.customRotation;

                    glm::quat bodyQuat = *(glm::quat*)&lastSpawnedObject.body->Rotation;
                    glm::vec3 vx = bodyQuat * glm::vec3(1, 0, 0);
                    glm::vec3 vy = bodyQuat * glm::vec3(0, 1, 0);
                    glm::vec3 vz = bodyQuat * glm::vec3(0, 0, 1); //(UP)

                    glm::vec3 translation = ((vz * lastSpawnedObject.params.backTranslate.x) + (vy * lastSpawnedObject.params.backTranslate.y) + (vx * lastSpawnedObject.params.backTranslate.z));
                    float spawnPosx = ((glb::player->cameraPosition.x) + (glb::player->cameraEuler().x * 2.25f));
                    float spawnPosy = ((glb::player->cameraPosition.y) + (glb::player->cameraEuler().y * 2.25f));
                    float spawnPosz = ((glb::player->cameraPosition.z) + (glb::player->cameraEuler().z * 2.25f));

                    lastSpawnedObject.body->Position = { spawnPosx - translation.x,  spawnPosy - translation.y, spawnPosz - translation.z };
                    lastSpawnedObject.body->Velocity = lastSpawnedObject.params.startVelocity;
                }
                else if(lastSpawnedObject.params.spawnType == objectSpawnType::placed) {

                    td::Vec3 voxSize = { (lastSpawnedObject.vox->sizeX / 10.f) * voxScale, (lastSpawnedObject.vox->sizeY / 10.f) * voxScale, (lastSpawnedObject.vox->sizeZ / 10.f) * voxScale }; 
                    glm::vec3 hitPos = { rd.worldPos.x, rd.worldPos.y, rd.worldPos.z };

                    if (rd.angle.x == 0.f) {
                        rd.angle.x += 0.0001f;
                    }

                    if (rd.angle.y == 0.f) {
                        rd.angle.y += 0.0001f;
                    }

                    if (rd.angle.z == 0.f) {
                        rd.angle.z += 0.0001f;
                    }

                    glm::quat facePlayer = glm::quat(glm::vec3(0, glb::player->camYaw, 0));
                    glm::vec3 vxTmp = facePlayer * glm::vec3(1, 0, 0);

                    glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

                    hitDir = glm::normalize(hitDir);

                    glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + -hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.

                    glm::vec3 vx = q * glm::vec3(1, 0, 0);
                    glm::vec3 vy = q * glm::vec3(0, 1, 0);
                    glm::vec3 vz = q * glm::vec3(0, 0, 1); //(UP)

                    glm::vec3 translation = ((vz * (-0.f)) + (vy * (voxSize.y / 2.f)) + (vx * (voxSize.x / 2.f)));

                    *(glm::quat*)&lastSpawnedObject.body->Rotation = q;
                    lastSpawnedObject.body->RotationVelocity = { 0, 0, 0 };
                    lastSpawnedObject.body->Velocity = { 0, 0, 0 };

                    lastSpawnedObject.body->Position = { target.x - translation.x, target.y - translation.y, target.z - translation.z };
                }
                else {
                    lastSpawnedObject.body->RotationVelocity = { 0, 0, 0 };
                    lastSpawnedObject.body->Velocity = { 0, 0, 0 };
                    //lastSpawnedObject.body->Position = lastSpawnedObject.params.startPosition;
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

                        td::small_string ssVoxPath = td::small_string(lso.voxPath.c_str());
                        td::small_string ssSubPath = td::small_string("");

                        lso.voxObject = (TDVox*)glb::oSpawnVox(&ssVoxPath, &ssSubPath, 1.f);

                        lso.objectName = getObjectName(lso.basePath);

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

    void deleteLastObject() {
        if (glb::game->State == gameState::ingame) {
            if (spawnList.size() > 0) {
                //spawnList.back().shape->Destroy(spawnList.back().shape, true);
                spawnList.back().body->Destroy(spawnList.back().body, true);
                spawnList.pop_back();
            }
        }
    }

    //spawns a free object wherever the player is looking
    spawnedObject placeFreeObject(std::string filepath) {
        raycaster::rayData rd = raycaster::castRayPlayer();

        spawnedObject object = {};
        freeObjectSpawnParams params = {};

        spawnFreeEntity(filepath, params, &object);

        td::Vec3 voxSize = { (object.voxes[0]->sizeX / 10.f) * voxScale, (object.voxes[0]->sizeY / 10.f) * voxScale, (object.voxes[0]->sizeZ / 10.f) * voxScale };
        glm::vec3 hitPos = { rd.worldPos.x, rd.worldPos.y, rd.worldPos.z };

        //if any of the angles is exactly 0 then it all goes to fuck
        if (rd.angle.x == 0.f) { rd.angle.x += 0.0001f; }
        if (rd.angle.y == 0.f) { rd.angle.y += 0.0001f; }
        if (rd.angle.z == 0.f) { rd.angle.z += 0.0001f; }

        glm::quat facePlayer = glm::quat(glm::vec3(0, glb::player->camYaw, 0));
        glm::vec3 vxTmp = facePlayer * glm::vec3(1, 0, 0);

        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

        hitDir = glm::normalize(hitDir);

        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + -hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.

        glm::vec3 vx = q * glm::vec3(1, 0, 0);
        glm::vec3 vy = q * glm::vec3(0, 1, 0);
        glm::vec3 vz = q * glm::vec3(0, 0, 1); //(UP)

        glm::vec3 translation = ((vz * (-0.f)) + (vy * (voxSize.y / 2.f)) + (vx * (voxSize.x / 2.f)));

        object.body->Position = { rd.worldPos.x - translation.x, rd.worldPos.y - translation.y, rd.worldPos.z - translation.z };
        *(glm::quat*)&object.body->Rotation = q;
        object.body->Velocity = { 0, 0, 0 };

        return object;
    }

    bool spawnFreeEntity(std::string filepath, freeObjectSpawnParams params, spawnedObject* object) {
        if (!exists(filepath)) {
            std::cout << "[E] no file" << std::endl;
            return false;
        }

        uintptr_t uBODY = glb::oTMalloc(0x232u);
        TDBody* BODY = (TDBody*)uBODY;
        glb::oB_Constructor(uBODY, (uintptr_t)nullptr);
        glb::oSetDynamic(uBODY, true);
        BODY->isAwake = true;
        BODY->countDown = 0xF0;

        td::small_string file_path((char*)(filepath).c_str());
        std::vector<td::small_string> nameOut = {};
        glb::TDreadSubobjects(&file_path, (int*)&nameOut);
        if (nameOut.size() == 0) {
            nameOut.push_back(td::small_string(""));
        }

        for (td::small_string sub_path : nameOut) {
            uintptr_t uSHAPE = glb::oTMalloc(0x176u);
            TDShape* SHAPE = (TDShape*)uSHAPE;

            glb::oS_Constructor(uSHAPE, uBODY);
            uintptr_t VOX = glb::oSpawnVox(&file_path, &sub_path, voxScale);

            glb::oCreateTexture(VOX);
            glb::oCreatePhysics(VOX);

            SHAPE->pVox = (TDVox*)VOX;

            object->shapes.push_back(SHAPE);
            object->voxes.push_back((TDVox*)VOX);

            if (params.nocull) {
                *(byte*)(SHAPE + 9) |= 16;
            }

            ((TDShape*)SHAPE)->Texture = 3;
            ((TDShape*)SHAPE)->TextureIntensity = 1.f;
        }

        object->body = BODY;
        glb::oUpdateShapes(uBODY);

        return true;
    }

    spawnedObject placeChildObject(std::string filepath) {
        raycaster::rayData rd = raycaster::castRayPlayer();

        spawnedObject object = {};

        if (!rd.hitShape) {
            return object;
        }


        childObjectSpawnParams params = {};
        params.parentBody = rd.hitShape->getParentBody();
        spawnChildEntity(filepath, params, &object);

        td::Vec3 voxSize = { (object.voxes[0]->sizeX / 10.f) * voxScale, (object.voxes[0]->sizeY / 10.f) * voxScale, (object.voxes[0]->sizeZ / 10.f) * voxScale };
        glm::vec3 hitPos = { rd.worldPos.x, rd.worldPos.y, rd.worldPos.z };

        if (rd.angle.x == 0.f) { rd.angle.x += 0.0001f; }
        if (rd.angle.y == 0.f) { rd.angle.y += 0.0001f; }
        if (rd.angle.z == 0.f) { rd.angle.z += 0.0001f; }
        
        glm::quat facePlayer = glm::quat(glm::vec3(0, glb::player->camYaw, 0));
        glm::vec3 vxTmp = facePlayer * glm::vec3(1, 0, 0);

        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);
        hitDir = glm::normalize(hitDir);

        glm::vec3 bodyPos = glm::vec3(params.parentBody->Position.x, params.parentBody->Position.y, params.parentBody->Position.z);
        glm::quat bodyQuat = *(glm::quat*)&params.parentBody->Rotation;
        glm::mat4 RotationMatrix = glm::toMat4(bodyQuat);

        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + -hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.

        glm::vec3 vx = q * glm::vec3(1, 0, 0);
        glm::vec3 vy = q * glm::vec3(0, 1, 0);
        glm::vec3 vz = q * glm::vec3(0, 0, 1); //(UP)

        glm::vec3 translation = ((vz * (-0.f)) + (vy * (voxSize.y / 2.f)) + (vx * (voxSize.x / 2.f)));
        glm::vec3 localPos = glm::vec3((glm::inverse(RotationMatrix)) * -glm::vec4((bodyPos.x - rd.worldPos.x) + translation.x, (bodyPos.y - rd.worldPos.y) + translation.y, (bodyPos.z - rd.worldPos.z) + translation.z, 0.f));
        glm::quat localRot = glm::inverse(bodyQuat) * q;

        object.shapes[0]->pOffset = { localPos.x, localPos.y, localPos.z };
        *(glm::quat*)&object.shapes[0]->rOffset = localRot;
        glb::oUpdateShapes((uintptr_t)params.parentBody);
        glb::tdUpdateFunc(params.parentBody, 0, 1);

        return object;
    }

    bool spawnChildEntity(std::string filepath, childObjectSpawnParams params, spawnedObject* object) {
        if (!exists(filepath)) {
            std::cout << "[E] no file" << std::endl;
            return false;
        }

        td::small_string file_path((char*)(filepath).c_str());
        std::vector<td::small_string> nameOut = {};
        glb::TDreadSubobjects(&file_path, (int*)&nameOut);
        if (nameOut.size() == 0) {
            nameOut.push_back(td::small_string(""));
        }

        for (td::small_string sub_path : nameOut) {
            uintptr_t uSHAPE = glb::oTMalloc(0x176u);
            TDShape* SHAPE = (TDShape*)uSHAPE;

            glb::oS_Constructor(uSHAPE, (uintptr_t)params.parentBody);
            uintptr_t VOX = glb::oSpawnVox(&file_path, &sub_path, voxScale);

            glb::oCreateTexture(VOX);
            glb::oCreatePhysics(VOX);

            SHAPE->pVox = (TDVox*)VOX;

            object->shapes.push_back(SHAPE);
            object->voxes.push_back((TDVox*)VOX);

            if (params.nocull) {
                *(byte*)(SHAPE + 9) |= 16;
            }

            ((TDShape*)SHAPE)->Texture = 3;
            ((TDShape*)SHAPE)->TextureIntensity = 1.f;
        }

        object->body = params.parentBody;

        glb::oUpdateShapes((uintptr_t)params.parentBody);

        return true;
    }

    KMSpawnedObject spawnEntity(std::string filepath, objectSpawnerParams osp) {
        if (!exists(filepath)) {
            std::cout << "[E] no file" << std::endl;
            return { defaultParams, false, 0, 0 };
        }

        std::cout << "======================================" << std::endl;

        uintptr_t BODY = 0;

        if (osp.parentBody) {
            std::cout << "USING EXISTING PARENT" << std::endl;
            BODY = (uintptr_t)osp.parentBody;
        }
        else {
            std::cout << "USING NEW PARENT" << std::endl;
            BODY = glb::oTMalloc(0x232u);
            glb::oB_Constructor((uintptr_t)BODY, (uintptr_t)nullptr);
            glb::oSetDynamic((uintptr_t)BODY, true);
        }

        (*(TDBody*)&BODY).isAwake = true;
        (*(TDBody*)&BODY).countDown = 0xF0;

        std::cout << "Body address:  0x" << std::hex << BODY << std::endl;

        td::small_string file_path((char*)(filepath).c_str());

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

            vox = glb::oSpawnVox(&file_path, &sub_path, voxScale);

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

            if (osp.parentBody) {
                ((TDShape*)SHAPE)->pOffset = osp.parentBodyoffset;
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

        ((TDShape*)SHAPE)->Texture = 3;
        ((TDShape*)SHAPE)->TextureIntensity = 1.f;

        glb::oUpdateShapes((uintptr_t)BODY);
        glb::tdUpdateFunc((TDBody*)BODY, 0, 1);

        return { osp, false, (TDShape*)SHAPE, (TDBody*)BODY, (TDVox*)vox };
    }

}

