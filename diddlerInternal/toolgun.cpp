#include "Global.h"
#include "Raycaster.h"
#include "drawCube.h"
#include "objectSpawner.h"
#include "toolgun.h"
#include "lantern.h"
#include "maths.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "lidar.h"
#include "dotProjector.h"
#include <glm/gtx/quaternion.hpp>


namespace toolgun {
    bool spawnOnce = false;
    const char* tgName = "toolgun";
    td::Color red{ 1.f, 0.f, 0.f, 1.f };
    td::Color green{ 0.f, 1.f, 0.f, 1.f };
    td::Color blue{ 0.f, 0.f, 1.f, 1.f };
    td::Color white{ 1.f, 1.f, 1.f, 1.f };

    tgSettings currentsetting = tgSettings::spawner;
    bool playerIsHoldingToolgun = false;
    const float pi = 3.1415926535f;

    //spawner specific vars
    spawngunMethod method = spawngunMethod::placed;
    float thrownObjectVelocityMultiplier = 10.f;
    bool createOrigionalSpawnObject = true;
    spawner::LoadedSpawnableObject currentSpawngunObject;
    bool constSpawn = false;

    //minigun specific vars
    int minigunBulletType = 0;
    float spreadVal = 0.01f;
    int bulletsPerFrame = 1;
    std::random_device rd;
    std::default_random_engine eng(rd());

    //explosion specific settings
    float EXspreadVal = 0.01f;
    int EXbulletsPerFrame = 1;
    float power = 1.f;

    //flamethrower specific vars
    float flRadius = 5.f;
    int chance = 10;

    //setAttribute specific vars
    char setAttributeFirst[128] = {};
    char setAttributeSecond[128] = {};
    std::vector<fadeShapeOutline> clickedObjects = {};
    
    //testing specific items
    bool testingFireOnce = false;
    TDShape* targetShape = 0;
    TDBody* targetBody = 0;
    int stage = 0;

    //leafblower specific items
    leafblowerModes LBMode = leafblowerModes::blow;
    int leafBlowerRayCount = 50;
    float leafBlowerFOV = 0.2f;
    bool showRayHitPos = false;

    //debug specific items
    targetDbgInfo dbgObject = { 0, 0 };

    int particalID = 0;
    bool fireConst = false;
    bool fireOnceActive = true;

    //cutter specific items
    float maxRange = 100.f;
    float holeSize = 5.f;

    //slicer specific items
    float slicer_maxDist = 10.f;
    int slicer_resolution = 500;
    bool slicerHorizontal = true;

    //camera specific items
    bool frameOnce = false;
    bool takeSnapshot = false;
    int cameraResolution = 64;
    float cameraFov = 8.f;
    float cameraFps = 0.f;

    //rope specific items
    float ropeSlack = 0.f;
    float ropeStrength = 1.f;
    float ropeMaxStretch = 0.f;
    td::Color ropeColor = { 1.f, 1.f, 1.f, 1.f };
    bool ropeAttackOnce = true;
    bool ropeToolFirstPos = true;
    td::Vec3 ropeFirstPos = {};
    td::Vec3 ropeSecondPos = {};
    TDShape* shape1 = 0;
    TDShape* shape2 = 0;
    td::Vec3 translatedPoint1 = {};
    td::Vec3 translatedPoint2 = {};
    td::Vec3 worldPos1 = {};
    td::Vec3 worldPos2 = {};

    //weld specific items
    bool weldStageOne = true;
    bool weldAttackOnce = true;

    TDVox* weldTargetVox = 0;
    TDShape* weldTargetShape = 0;
    glm::quat weldTargetWorldRotation = { 0, 0, 0, 0 };
    glm::vec3 weldTargetWorldPosition = { 0, 0, 0 };

    TDBody* weldNewBody = 0;
    glm::quat weldLocalRotationOffset = { 0, 0, 0, 0 };
    glm::vec3 weldLocalPositionOffset = { 0, 0, 0 };



    void handleToolgun() {

        if (createOrigionalSpawnObject) {
            //currentSpawngunObject = spawner::spawnerObjectsDatabase[0].objects[0];

            for (spawner::spawnerCatagory catig : spawner::spawnerObjectsDatabase) {
                for (spawner::LoadedSpawnableObject object : catig.objects) {
                    if (object.objectName == "brick_metal") {
                        currentSpawngunObject = object;
                        break;
                    }
                }
            }


            createOrigionalSpawnObject = false;
        }

        bool clearClickedObjects = true;

        if (clickedObjects.size() > 0) {
            for (fadeShapeOutline& fso : clickedObjects) {
                glb::oOutlineShape(glb::renderer, fso.shape, &red, (float)(fso.alpha / 255.f));
                if (fso.alpha > 0) {
                    fso.alpha -= 4;
                    clearClickedObjects = false;
                }
            }

            if (clearClickedObjects) {
                clickedObjects.clear();
                clearClickedObjects = false;
            }
        }


        if (memcmp(glb::player->heldItemName, tgName, 8) == 0) {
            playerIsHoldingToolgun = true;

            if (currentsetting == tgSettings::spawner) { //handle spawning objects with the toolgun
                raycaster::rayData rd = raycaster::castRayPlayer();
                td::Vec3 target = rd.worldPos;
                //drawCube({ target.x, target.y, target.z }, 0.05, white);

                spawner::objectSpawnerParams osp{};
                osp.attributes = currentSpawngunObject.attributes;
                osp.animate = true;

                if (method == spawngunMethod::placed) {

                    if (currentSpawngunObject.voxObject) {

                        td::Color boxColour = { 1.f, 1.f, 1.f, 1.f };
                        if (trunc(1000. * spawner::voxScale) != trunc(1000. * 1.f)) {
                            boxColour = { 1.f, 0.55f, 0.f, 1.f };
                        }


                        float voxSizeX = (currentSpawngunObject.voxObject->sizeX / 10.f) * spawner::voxScale;
                        float voxSizeY = (currentSpawngunObject.voxObject->sizeY / 10.f) * spawner::voxScale;
                        float voxSizeZ = (currentSpawngunObject.voxObject->sizeZ / 10.f) * spawner::voxScale;

                        td::Vec3 oSize = { voxSizeX, voxSizeY, voxSizeZ};
                        glm::vec3 hitPos = { rd.worldPos.x, rd.worldPos.y, rd.worldPos.z };

                        glm::quat facePlayer = glm::quat(glm::vec3(4.71238898025f, glb::player->camYaw, 0));
                        glm::vec3 vxTmp = facePlayer * glm::vec3(-1, 0, 0);

                        glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

                        hitDir = glm::normalize(hitDir);

                        glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.

                        glm::vec3 vx = q * glm::vec3(-1, 0, 0);
                        glm::vec3 vy = q * glm::vec3(0, -1, 0);
                        glm::vec3 vz = q * glm::vec3(0, 0, -1); //(UP)

                        glm::vec3 translationFBL = ((vz * 0.f) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
                        glm::vec3 translationBBR = ((vz * 0.f) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
                        glm::vec3 translationBBL = ((vz * 0.f) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
                        glm::vec3 translationFBR = ((vz * 0.f) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));


                        glm::vec3 translationFTL = ((vz * (voxSizeZ * -1.f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
                        glm::vec3 translationBTR = ((vz * (voxSizeZ * -1.f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
                        glm::vec3 translationBTL = ((vz * (voxSizeZ * -1.f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
                        glm::vec3 translationFTR = ((vz * (voxSizeZ * -1.f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));


                        td::Vec3 FBL = { target.x - translationFBL.x, target.y - translationFBL.y, target.z - translationFBL.z };
                        td::Vec3 BBR = { target.x - translationBBR.x, target.y - translationBBR.y, target.z - translationBBR.z };
                        td::Vec3 BBL = { target.x - translationBBL.x, target.y - translationBBL.y, target.z - translationBBL.z };
                        td::Vec3 FBR = { target.x - translationFBR.x, target.y - translationFBR.y, target.z - translationFBR.z };


                        td::Vec3 FTL = { target.x - translationFTL.x, target.y - translationFTL.y, target.z - translationFTL.z };
                        td::Vec3 BTR = { target.x - translationBTR.x, target.y - translationBTR.y, target.z - translationBTR.z };
                        td::Vec3 BTL = { target.x - translationBTL.x, target.y - translationBTL.y, target.z - translationBTL.z };
                        td::Vec3 FTR = { target.x - translationFTR.x, target.y - translationFTR.y, target.z - translationFTR.z };

                        //bottom square
                        glb::oFDL(glb::renderer, FBL, FBR, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, FBL, BBL, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, BBL, BBR, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, BBR, FBR, boxColour, boxColour, false);

                        //top square
                        glb::oFDL(glb::renderer, FTL, FTR, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, FTL, BTL, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, BTL, BTR, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, BTR, FTR, boxColour, boxColour, false);

                        //walls
                        glb::oFDL(glb::renderer, FTL, FBL, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, FTR, FBR, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, BTL, BBL, boxColour, boxColour, false);
                        glb::oFDL(glb::renderer, BTR, BBR, boxColour, boxColour, false);
                    }

                    osp.spawnType = spawner::objectSpawnType::placed;
                }
                else if (method == spawngunMethod::thrown) {
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    td::Vec3 camEul = glb::player->cameraEuler();
                    osp.rotateFacePlayer = true;
                    osp.startVelocity = { glb::player->velocity.x + (camEul.x * thrownObjectVelocityMultiplier), glb::player->velocity.y + (camEul.y * thrownObjectVelocityMultiplier) ,glb::player->velocity.z + (camEul.z * thrownObjectVelocityMultiplier) };
                }

                if (glb::player->isAttacking) {
                    if (constSpawn) {
                        spawnOnce = true;
                    }
                    if (spawnOnce) {
                        spawnOnce = false;
                        spawner::KMSpawnedObject last = spawnObjectProxy(currentSpawngunObject.voxPath, osp);
                    }
                }
                else {
                    spawnOnce = true;
                }
            }
            else if (currentsetting == tgSettings::minigun) {
                if (glb::player->isAttacking) {
                    float noiseX = 0;
                    float noiseY = 0;
                    float noiseZ = 0;

                    for (int i = 0; i < bulletsPerFrame; i++) {
                        if (spreadVal > 0.f) {
                            std::uniform_real_distribution<> noise(-spreadVal, spreadVal);
                            noiseX = noise(eng);
                            noiseY = noise(eng);
                            noiseZ = noise(eng);
                        }

                        td::Vec3 camPos = glb::player->cameraPosition;
                        camPos = { camPos.x, camPos.y - 0.05f, camPos.z };
                        td::Vec3 camRot = glb::player->cameraEuler();
                        td::Vec3 shootPos = { camRot.x + noiseX, camRot.y + noiseY, camRot.z + noiseZ };
                        glb::oPewpew(glb::scene, &camPos, &shootPos, minigunBulletType, 50000.f);
                    }
                }
            }
            else if (currentsetting == tgSettings::explosion) {
                if (glb::player->isAttacking) {
                    float noiseX = 0;
                    float noiseY = 0;
                    float noiseZ = 0;

                    for (int i = 0; i < EXbulletsPerFrame; i++) {
                        if (EXspreadVal > 0.f) {
                            std::uniform_real_distribution<> noise(-EXspreadVal, EXspreadVal);
                            noiseX = noise(eng);
                            noiseY = noise(eng);
                            noiseZ = noise(eng);
                        }

                        td::Vec3 camPos = glb::player->cameraPosition;
                        td::Vec3 camRot = glb::player->cameraEuler();

                        td::Vec3 shootPos = { camRot.x + noiseX, camRot.y + noiseY, camRot.z + noiseZ };

                        RaycastFilter rcf{ 0 };

                        raycaster::rayData rd = raycaster::castRayManual(camPos, shootPos, &rcf);
                        td::Vec3 target = rd.worldPos;
                        glb::TDcreateExplosion((uintptr_t)glb::scene, &target, power);
                    }
                }
            }
            else if (currentsetting == tgSettings::flamethrower) {
                RaycastFilter rcf = {};

                if (glb::player->isAttacking == true) {
                    uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);
                    dotProjector::pixelResponse* pixelResponse = dotProjector::projectDotMatrix(16, flRadius, 1.f, false, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
                    for (int i = 0; i < pixelResponse->size; i++) {
                        td::Vec3 firepos = pixelResponse->data[i].worldPos;
                        if (chance == 100) {
                            glb::oSpawnFire(sceneSpecial, &firepos);
                        }
                        else if ((rand() % (100 - chance)) == 0) {
                            glb::oSpawnFire(sceneSpecial, &firepos);
                        }
                        drawCube(firepos, 0.02, red);
                    }
                }
                else {
                    uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);
                    dotProjector::pixelResponse* pixelResponse = dotProjector::projectDotMatrix(16, flRadius, 1.f, false, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
                    for (int i = 0; i < pixelResponse->size; i++) {
                        drawCube(pixelResponse->data[i].worldPos, 0.02, green);
                    }
                }
            }
            else if (currentsetting == tgSettings::destroyer) {

                if (fireConst) {
                    fireOnceActive = true;
                }

                td::Color green{ 0.f, 1.f, 0.f, 1.f };
                raycaster::rayData rayDat = raycaster::castRayPlayer();
                if (rayDat.distance <= maxRange) {
                    drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, 0.05, green);
                    if (glb::player->isAttacking == true) {
                        if (fireOnceActive) {
                            fireOnceActive = false;
                            glb::oWrappedDamage(glb::scene, &rayDat.worldPos, holeSize, holeSize, 0, 0);
                        }
                    }
                    else {
                        fireOnceActive = true;
                    }
                }
                else {
                    drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, 0.05, red);
                }
            }
            else if (currentsetting == tgSettings::remover) {
                raycaster::rayData rd = raycaster::castRayPlayer();
                if (rd.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
                    glb::oHighlightShape(glb::renderer, rd.hitShape, 1.f);
                    if (glb::player->isAttacking) {
                        if (spawnOnce) {
                            spawnOnce = false;
                            std::cout << rd.hitShape << std::endl;
                            rd.hitShape->Destroy(rd.hitShape, true);

                        }
                    }
                    else {
                        spawnOnce = true;
                    }
                }
            }
            else if (currentsetting == tgSettings::setAtttibute) {
                if (glb::player->isAttacking) {
                    if (spawnOnce) {
                        spawnOnce = false;
                        raycaster::rayData rd = raycaster::castRayPlayer();

                        clickedObjects.push_back({ rd.hitShape, 255 });
                        td::small_string attrib = setAttributeFirst;
                        td::small_string attrib2 = setAttributeSecond;
                        glb::oSOA(rd.hitShape, &attrib, &attrib2);
                    }
                }
                else {
                    spawnOnce = true;
                }
            }
            else if (currentsetting == tgSettings::debugObject) {
                td::Color green{ 0.f, 1.f, 0.f, 1.f };
                td::Color blue{ 0.f, 0.f, 1.f, 1.f };

                raycaster::rayData rayDat = raycaster::castRayPlayer();
                if (rayDat.hitShape) {
                    glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);
                    dbgObject.tShape = rayDat.hitShape;
                    dbgObject.tBody = rayDat.hitShape->getParentBody();
                }
            }
            else if (currentsetting == tgSettings::leafblower) {
                if (glb::player->isAttacking == true) {
                    float noiseX = 0;
                    float noiseY = 0;
                    float noiseZ = 0;
                    RaycastFilter filter{ 0 };
                    filter.m_Mask = -1;

                    td::Vec3 playerCameraVec3 = glb::player->cameraEuler();
                    std::vector<TDBody*> hitBodies = {};
                    std::vector<TDShape*> hitShapes = {};

                    for (int i = 0; i < leafBlowerRayCount; i++) {
                        //std::uniform_real_distribution<> noise(-0.3, 0.3);
                        noiseX = -leafBlowerFOV + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (leafBlowerFOV - -leafBlowerFOV)));
                        noiseY = -leafBlowerFOV + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (leafBlowerFOV - -leafBlowerFOV)));
                        noiseZ = -leafBlowerFOV + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (leafBlowerFOV - -leafBlowerFOV)));
                        td::Vec3 blowVec3 = { playerCameraVec3.x + noiseX, playerCameraVec3.y + noiseY, playerCameraVec3.z + noiseZ };

                        raycaster::rayData rayDat = raycaster::castRayManual(glb::player->cameraPosition, blowVec3, &filter);
                        if (showRayHitPos) {
                            drawCube(rayDat.worldPos, 0.02f, red);
                        }

                        if (rayDat.hitShape) {
                            TDBody* hitBody = rayDat.hitShape->getParentBody();
                            TDShape* hitShape = rayDat.hitShape;

                            if (hitBody->Dynamic && rayDat.distance < 100.f) {
                                if ((int)LBMode == 0 || (int)LBMode == 1) {
                                    if (!(std::find(hitBodies.begin(), hitBodies.end(), hitBody) != hitBodies.end())) {
                                        hitBodies.push_back(hitBody);
                                    }
                                }
                                else {
                                    if (!(std::find(hitShapes.begin(), hitShapes.end(), hitShape) != hitShapes.end())) {
                                        hitShapes.push_back(hitShape);
                                    }
                                }
                            }
                        }
                    }

                    td::Vec3 dir = glb::player->cameraEuler();

                    if ((int)LBMode == 0 || (int)LBMode == 1) {
                        for (TDBody* current : hitBodies) {
                            current->countDown = 0x3c;
                            current->isAwake = true;
                            td::Vec3 oVec = current->Velocity;
                            td::Vec3 playerEuler = glb::player->cameraEuler();

                            td::Vec3 addVec = { dir.x * 50, dir.y * 50, dir.z * 50 };

                            switch (LBMode) {
                            case leafblowerModes::blow:
                                current->Velocity = { oVec.x + addVec.x, oVec.y + addVec.y, oVec.z + addVec.z };
                                break;

                            case leafblowerModes::succ:
                                current->Velocity = { oVec.x + -addVec.x, oVec.y + -addVec.y, oVec.z + -addVec.z };
                                break;
                            }
                        }
                    }
                    else {
                        for (TDShape* current : hitShapes) {
                            current->Destroy(current, false);
                        }
                    }
                    hitBodies.clear();
                }

            }

            else if (currentsetting == tgSettings::slicer) {

                float currentOffset = 0.f;

                for (int i = 0; i < slicer_resolution; i++) {
                    RaycastFilter filter{ 0 };
                    filter.m_Mask = -1;
                    glm::quat camera_rotation_tl = {};

                    if (slicerHorizontal) {
                        camera_rotation_tl = glm::quat(glm::vec3(0, (currentOffset + glb::player->camYaw), 0));
                    }
                    else {
                        camera_rotation_tl = glm::quat(glm::vec3((currentOffset + glb::player->camPitch), glb::player->camYaw, 0));
                    }

                    glm::vec3 raycast_dir_tl = camera_rotation_tl * glm::vec3(0, 0, -1);

                    raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tl.x, raycast_dir_tl.y, raycast_dir_tl.z }, &filter);
                    if (rd.distance <= slicer_maxDist) {
                        if (glb::player->isAttacking) {
                            glb::oWrappedDamage(glb::scene, &rd.worldPos, 0.2f, 0.2f, 0, 0);
                        }
                        drawCube(rd.worldPos, 0.05f, red);
                    }
                    else {
                        drawCube(rd.worldPos, 0.05f, white);
                    }

                    td::Vec3 target = rd.worldPos;
                    currentOffset += ((pi * 2) / slicer_resolution);
                }

            }
            else if (currentsetting == tgSettings::camera) {
                if ((glb::player->isAttacking == true && !glb::displayMenu) || !camera::staged_newFrame) {
                    cameraFps = camera::updateImageColour(cameraResolution, cameraFov);
                }
                else {
                    camera::staged_newFrame = true;
                }
                camera::drawCameraWindow(cameraFps);
            }
            else if (currentsetting == tgSettings::rope) {
                raycaster::rayData rd = raycaster::castRayPlayer();
                drawCube(rd.worldPos, 0.02f, white);
                if (ropeToolFirstPos) {
                    if (glb::player->isAttacking) {
                        if (ropeAttackOnce) {
                            ropeAttackOnce = false;
                            ropeFirstPos = rd.worldPos;
                            shape1 = rd.hitShape;
                            worldPos1 = rd.worldPos;
                            ropeToolFirstPos = false;
                        }
                    }
                    else {
                        ropeAttackOnce = true;
                    }
                }
                else {
                    drawCube(ropeFirstPos, 0.02f, white);
                    glb::oFDL(glb::renderer, ropeFirstPos, rd.worldPos, ropeColor, ropeColor, false);
                    if (glb::player->isAttacking) {
                        if (ropeAttackOnce) {
                            ropeAttackOnce = false;
                            ropeSecondPos = rd.worldPos;
                            TDJoint* newJoint = (TDJoint*)glb::oTMalloc(208);
                            glb::tdConstructJoint(newJoint, nullptr);
                            shape2 = rd.hitShape;
                            worldPos2 = rd.worldPos;
                            glb::tdInitWire(newJoint, &ropeFirstPos, &ropeSecondPos, newJoint->m_Size, ropeColor, ropeSlack, ropeStrength, ropeMaxStretch);
                            ropeToolFirstPos = true;
                        }
                    }
                    else {
                        ropeAttackOnce = true;
                    }
                }
            }
            else if (currentsetting == tgSettings::testing) {
                raycaster::rayData rd = raycaster::castRayPlayer();
                drawCube(rd.worldPos, 0.02f, white);
                if (weldStageOne) {
                    if (glb::player->isAttacking) {
                        if (weldAttackOnce) {
                            weldAttackOnce = false;

                            weldTargetVox = rd.hitShape->pVox;
                            weldTargetShape = rd.hitShape;

                            weldStageOne = false;
                        }
                    }
                    else {
                        weldAttackOnce = true;
                    }
                }
                else {
                    if (glb::player->isAttacking) {
                        if (weldAttackOnce) {
                            weldAttackOnce = false;
                            weldNewBody = rd.hitShape->getParentBody();

                            weldNewBody->isAwake = true;
                            weldNewBody->countDown = 0xF0;

                            TDShape* newShape = (TDShape*)glb::oTMalloc(0x176u);
                            glb::oS_Constructor((uintptr_t)newShape, (uintptr_t)weldNewBody);
                            *(uintptr_t*)(newShape + 0x90) = (uintptr_t)weldTargetVox;
                            glb::oCreateTexture((uintptr_t)weldTargetVox);
                            glb::oCreatePhysics((uintptr_t)weldTargetVox);
                            glb::oUpdateShapes((uintptr_t)weldNewBody);

                            //weldTargetShape->Destroy(weldTargetShape, false);

                            weldStageOne = true;
                        }
                    }
                    else {
                        weldAttackOnce = true;
                    }
                }
            }
            else {
                playerIsHoldingToolgun = false;
            }
        }
    }
}


