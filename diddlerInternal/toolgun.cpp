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
    float flRadius = 1.5f;
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

    //camera specific items
    bool frameOnce = false;
    float* pixels = nullptr;
    float minDist = 1000.f;
    float maxDist = 0.f;
    int cameraResolution = 32;
    int commit_resolution = 32;

    void handleToolgun() {

        if (createOrigionalSpawnObject) {
            currentSpawngunObject.voxPath = "vox\\Default\\dbgcube_metal\\object.vox";
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
                drawCube({ target.x, target.y, target.z }, 0.05, white);

                spawner::objectSpawnerParams osp{};
                osp.attributes = currentSpawngunObject.attributes;
                osp.animate = true;

                if (method == spawngunMethod::placed) {
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

                        RaycastFilter rcf{0};
                        raycaster::rayData rd = raycaster::castRayManual(camPos, shootPos, &rcf);
                        td::Vec3 target = rd.worldPos;
                        glb::TDcreateExplosion((uintptr_t)glb::scene, &target, power);
                    }
                }
            }
            else if (currentsetting == tgSettings::flamethrower) {
                if (glb::player->isAttacking == true) {

                    //std::cout << std::to_string(glb::scene->fireSystem->m_Instances.size()) << "@" << &glb::scene->fireSystem << std::endl;

                    raycaster::rayData rayDat = raycaster::castRayPlayer();
                    drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, flRadius, red);

                    for (float ty = (rayDat.worldPos.y - flRadius); ty < (rayDat.worldPos.y + flRadius); ty += 0.1f) {
                        for (float tx = (rayDat.worldPos.x - flRadius); tx < (rayDat.worldPos.x + flRadius); tx += 0.1f) {
                            for (float tz = (rayDat.worldPos.z - flRadius); tz < (rayDat.worldPos.z + flRadius); tz += 0.1f) {
                                uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);

                                if (chance == 100) {
                                    td::Vec3 firepos = { tx, ty, tz };
                                    glb::oSpawnFire(sceneSpecial, &firepos);
                                }
                                else if ((rand() % (100 - chance)) == 0) {
                                    td::Vec3 firepos = { tx, ty, tz };
                                    glb::oSpawnFire(sceneSpecial, &firepos);
                                }

                                //std::cout << "Most recent fire: " << glb::scene->fireSystem->m_Instances.end() << " Lifetime: " << (((FireSystem::Fire*)((uintptr_t)(glb::scene->fireSystem->m_Instances.end())))->m_TimeRemaining) << std::endl;
                            }
                        }
                    }
                }
                else {
                    raycaster::rayData rayDat = raycaster::castRayPlayer();
                    td::Color green{ 0.f, 1.f, 0.f, 1.f };
                    drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, flRadius, green);
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
                if (rayDat.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
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

                        if (rayDat.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
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
                    glm::quat camera_rotation_tl = glm::quat(glm::vec3(0, (currentOffset + glb::player->camYaw), 0));
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


			else if (currentsetting == tgSettings::testing) {
               
			    RaycastFilter filter{ 0 };
			    filter.m_Mask = -1;

			    float pitchMin = glb::player->camPitch - 0.40f;
			    float pitchMax = glb::player->camPitch + 0.40f;
			    float yawMin = glb::player->camYaw - 0.40f;
			    float yawMax = glb::player->camYaw + 0.40f;

			    glm::quat camera_rotation_tl = glm::quat(glm::vec3(pitchMax, yawMax, 0));
			    glm::vec3 raycast_dir_tl = camera_rotation_tl * glm::vec3(0, 0, -1);

			    glm::quat camera_rotation_tr = glm::quat(glm::vec3(pitchMax, yawMin, 0));
			    glm::vec3 raycast_dir_tr = camera_rotation_tr * glm::vec3(0, 0, -1);

			    glm::quat camera_rotation_bl = glm::quat(glm::vec3(pitchMin, yawMin, 0));
			    glm::vec3 raycast_dir_bl = camera_rotation_bl * glm::vec3(0, 0, -1);

			    glm::quat camera_rotation_br = glm::quat(glm::vec3(pitchMin, yawMax, 0));
			    glm::vec3 raycast_dir_br = camera_rotation_br * glm::vec3(0, 0, -1);


			    raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tl.x, raycast_dir_tl.y, raycast_dir_tl.z }, &filter);
			    td::Vec3 target = rd.worldPos;
			    drawCube({ target.x, target.y, target.z }, 0.05, red);

			    rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tr.x, raycast_dir_tr.y, raycast_dir_tr.z }, &filter);
			    target = rd.worldPos;
			    drawCube({ target.x, target.y, target.z }, 0.05, white);

			    rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, &filter);
			    target = rd.worldPos;
			    drawCube({ target.x, target.y, target.z }, 0.05, green);

			    rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_br.x, raycast_dir_br.y, raycast_dir_br.z }, &filter);
			    target = rd.worldPos;
			    drawCube({ target.x, target.y, target.z }, 0.05, blue);

                int pixelOffset = 0;

                if (glb::player->isAttacking == true && !glb::displayMenu) {
                    if (frameOnce) {
                        minDist = 1000.f;
                        maxDist = 0.f;

                        commit_resolution = cameraResolution;

                        //frameOnce = false;
                        free(pixels);
                        pixels = new float[commit_resolution * commit_resolution];

                        for (int x = commit_resolution; x > 0; x--) {
                            for (int y = commit_resolution; y > 0; y--) {

                                float pitchStep = (pitchMax - pitchMin) / commit_resolution;
                                float thisPitch = pitchMin + (x * pitchStep);

                                float yawStep = (yawMax - yawMin) / commit_resolution;
                                float thisYaw = yawMin + (y * yawStep);

                                glm::quat camera_rot = glm::quat(glm::vec3(thisPitch, thisYaw, 0));
                                glm::vec3 raycast_dir = camera_rot * glm::vec3(0, 0, -1);

                                raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir.x, raycast_dir.y, raycast_dir.z }, &filter);
                                float thisDist = rd.distance;

                                //drawCube(rd.worldPos, 0.2f, red);
                                
                                if (thisDist < minDist) {
                                    minDist = thisDist;
                                }
                                if (thisDist > maxDist && thisDist < 1000.f) {
                                    maxDist = thisDist;
                                }

                                if (thisDist < 1000.f) {
                                    pixels[pixelOffset] = thisDist;
                                }
                                else {
                                    pixels[pixelOffset] = -1.f;
                                }

                                pixelOffset++;
                            }
                        }

                        camera::updateCameraFrame(pixels, commit_resolution, minDist, maxDist);

                    }
                }
                else {
                    frameOnce = true;
                }
                
                
                camera::drawCameraWindow();

			}

        }
        else {
            playerIsHoldingToolgun = false;
        }
    }
}


