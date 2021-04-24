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
    int particalID = 0;

    //cutter specific items
    float maxRange = 10.f;
    float holeSize = 0.25f;

    void handleToolgun() {

        if (createOrigionalSpawnObject) {
            currentSpawngunObject.voxPath = "vox\\cube_kned\\object.vox";
            createOrigionalSpawnObject = false;
        }

        bool clearClickedObjects = true;
        if (clickedObjects.size() > 0) {
            for (fadeShapeOutline& fso : clickedObjects) {
                td::Color tmp = { 1.f, 1.f, 1.f, (float)(fso.alpha / 255.f) };
                glb::oOutlineshape(glb::renderer, fso.shape, &tmp);
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
            if (currentsetting == tgSettings::spawner) { //handle spawning objects with the toolgun
                raycaster::rayData rd = raycaster::castRayPlayer();
                td::Vec3 target = rd.worldPos;
                drawCube({ target.x, target.y, target.z }, 0.05, white);

                spawner::objectSpawnerParams osp{};
                osp.attributes = currentSpawngunObject.attributes;
                osp.customRotation = rd.angle;

                if (glb::player->isAttacking) {
                    if (spawnOnce) {
                        spawnOnce = false;
                        spawnObjectProxy(currentSpawngunObject.voxPath, osp);
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
                        glb::oPewpew(glb::scene, &camPos, &shootPos, minigunBulletType);
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
                        raycaster::rayData rd = raycaster::castRayManual(camPos, shootPos, rcf);
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
        
            if (currentsetting == tgSettings::testing) {
                td::Color green{ 0.f, 1.f, 0.f, 1.f };
                td::Color red{ 1.f, 0.f, 0.f, 1.f };
                raycaster::rayData rayDat = raycaster::castRayPlayer();
                if (rayDat.distance <= maxRange) {
                    drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, 0.05, green);
                    if (glb::player->isAttacking == true) {
                        glb::oWrappedDamage(glb::scene, &rayDat.worldPos, holeSize, holeSize, 0, 0);
                    }
                }
                else {
                    drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, 0.05, red);
                }  
            }

            if (currentsetting == tgSettings::remover) {
                raycaster::rayData rd = raycaster::castRayPlayer();
                if (rd.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
                    td::Color red = { 1, 0, 0, 1 };
                    glb::oOutlineshape(glb::renderer, rd.hitShape, &red);
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
        }
    }
}


