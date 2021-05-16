#include "Global.h"
#include "Raycaster.h"
#include "drawCube.h"
#include "objectSpawner.h"
#include "toolgun.h"
#include "lantern.h"

namespace toolgun {
    bool spawnOnce = false;
    const char* tgName = "toolgun";
    td::Color white{ 1.f, 1.f, 1.f, 1.f };
    tgSettings currentsetting = tgSettings::spawner;
    bool playerIsHoldingToolgun = false;

    //spawner specific vars
    bool createOrigionalSpawnObject = true;
    spawner::LoadedSpawnableObject currentSpawngunObject;

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

    //debug specific items
    targetDbgInfo dbgObject = { 0, 0 };

    int particalID = 0;
    bool fireConst = false;
    bool fireOnceActive = true;

    //cutter specific items
    float maxRange = 100.f;
    float holeSize = 5.f;



    void handleToolgun() {

        if (createOrigionalSpawnObject) {
            currentSpawngunObject.voxPath = "vox\\cube_kned\\object.vox";
            createOrigionalSpawnObject = false;
        }

        bool clearClickedObjects = true;

        if (clickedObjects.size() > 0) {
            for (fadeShapeOutline& fso : clickedObjects) {
                td::Color red{ 1.f, 0.f, 0.f, 1.f };
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
                osp.parentBody = targetBody;

                if (glb::player->isAttacking) {
                    if (spawnOnce) {
                        spawnOnce = false;
                        spawner::KMSpawnedObject last = spawnObjectProxy(currentSpawngunObject.voxPath, osp);
                    }
                }
                else {
                    spawnOnce = true;
                }
            }

            if (currentsetting == tgSettings::minigun) {
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

            if (currentsetting == tgSettings::explosion) {
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

            if (currentsetting == tgSettings::flamethrower) {
                if (glb::player->isAttacking == true) {

                    //std::cout << std::to_string(glb::scene->fireSystem->m_Instances.size()) << "@" << &glb::scene->fireSystem << std::endl;

                    raycaster::rayData rayDat = raycaster::castRayPlayer();
                    td::Color red{ 1.f, 0.f, 0.f, 1.f };
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
        
            if (currentsetting == tgSettings::destroyer) {

                if (fireConst) {
                    fireOnceActive = true;
                }

                td::Color green{ 0.f, 1.f, 0.f, 1.f };
                td::Color red{ 1.f, 0.f, 0.f, 1.f };
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

            if (currentsetting == tgSettings::remover) {
                raycaster::rayData rd = raycaster::castRayPlayer();
                if (rd.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
                    td::Color red = { 1, 1, 1, 1 };
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

            if (currentsetting == tgSettings::setAtttibute) {
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

            if (currentsetting == tgSettings::debugObject) {
                td::Color green{ 0.f, 1.f, 0.f, 1.f };
                td::Color blue{ 0.f, 0.f, 1.f, 1.f };

                raycaster::rayData rayDat = raycaster::castRayPlayer();
                if (rayDat.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
                    glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);
                    dbgObject.tShape = rayDat.hitShape;
                    dbgObject.tBody = rayDat.hitShape->getParentBody();
                }
            }


            if (currentsetting == tgSettings::testing) {
                if (glb::player->isAttacking == true) {
                    td::Color green{ 0.f, 1.f, 0.f, 1.f };

                    if (testingFireOnce == true) {
                        testingFireOnce = false;

                        raycaster::rayData rayDat = raycaster::castRayPlayer();
                        if (rayDat.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {

                            glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);
                            TDBody* TmpTargetBody = rayDat.hitShape->getParentBody();
                            glb::oUpdateShapes((uintptr_t)(TmpTargetBody));

                            /*if (stage == 0) {
                                glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);
                                targetShape = rayDat.hitShape;

                                std::cout << "SET TARGET SHAPE: " << targetShape << std::endl;

                                stage = 1;
                            }
                            else if (stage == 1) {
                                glb::oOutlineShape(glb::renderer, rayDat.hitShape, &green, 1.f);
                                targetBody = rayDat.hitShape->getParentBody();

                                std::cout << "SET TARGET BODY: " << targetBody << std::endl;

                                glb::oSetShapeParentBody(targetShape, *(byte*)((uintptr_t)(targetShape + 8)), targetBody);
                                glb::oSetDynamic((uintptr_t)targetBody, true);
                                glb::oUpdateShapes((uintptr_t)targetBody);

                                targetShape = 0;
                                targetBody = 0;
                                stage = 0;
                            }*/


                        }

                    }
                }
                else {
                    testingFireOnce = true;
                }
            }
        }
        else {
            playerIsHoldingToolgun = false;
        }
    }
}


