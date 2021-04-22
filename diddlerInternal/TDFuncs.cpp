#include "TDFuncs.h"
#include "global.h"

void sigscanItems() {
    glb::oPaint = (tPaint)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x55\x41\x56\x48\x8D\x68\xD8\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxx?????", GetModuleHandle(NULL));
    glb::oFDL = (frameDrawLine)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x2A\x48\x89\x6C\x24\x2A\x48\x89\x74\x24\x2A\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x80\x7C\x24\x2A\x2A", "xxxx?xxxx?xxxx?xxxxxxxxxxxx??", GetModuleHandle(NULL));
    glb::oRC = (rayCast)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x4C\x89\x40\x18\x48\x89\x50\x10\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x38\xFC\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDcreateExplosion = (createExplosion)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x98\xF9\xFF\xFF\x48\x81\xEC\x30\x07\x00\x00\x48\xC7\x45\x68", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSpawnVox = (tSpawnVox)mem::FindPattern((PBYTE)"\x4C\x8B\xDC\x57\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\xC7\x44\x24\x2A\x2A\x2A\x2A\x2A", "xxxxxxx????xxxx?????", GetModuleHandle(NULL));
    glb::killme = (idfk)mem::FindPattern((PBYTE)"\x48\x89\x54\x24\x10\x55\x53\x56\x41\x54\x41\x56\x48\x8D\xAC\x24\x40\xCB\xFF\xFF\xB8\xC0\x35\x00\x00\xE8\x52\x1A\x15\x00\x48\x2B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oTdDelBdy = (deleteBody)mem::FindPattern((PBYTE)"\x48\x83\xEC\x28\x48\x8B\xCA\x33\xD2\xE8\x82\x16\x0F\x00\x48\x8B", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oMakeHole = (makeHole)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x53\x48\x81\xEC\x90\x00\x00\x00\x0F\x29\x70\xE8\x4C\x8D\x40\xA8\x48\x8B\xDA\x0F\x29\x78\xD8\x44\x0F\x29\x40\xC8\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreateTexture = (CreateTexture)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x41\x54\x41\x55\x41\x57", "xxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreatePhysics = (CreatePhysics)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x50\x04\x00\x00\x48\x8B\xD9\xE8\xFF\x56\x00\x00\x48\x8B\x4B\x18\xC7\x43\x5C\x00\x00\x00\x00\xE8\x6F\x0E\x06", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oUpdateShapes = (UpdateShapes)mem::FindPattern((PBYTE)"\x40\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x10\xEB\xFF\xFF\xB8\xF0\x15\x00\x00\xE8\x24\xE0\x24\x00\x48\x2B\xE0\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oB_Constructor = (B_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oS_Constructor = (S_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x02\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSetDynamic = (SetDynamic)mem::FindPattern((PBYTE)"\x88\x91\x2A\x2A\x2A\x2A\x4C\x8B\xC1\x84\xD2\x74\x29\x0F\xB6\x81\x2A\x2A\x2A\x2A\xC6\x81\x2A\x2A\x2A\x2A\x2A\x84\xC0\x75\x17\x48\x8B\x05\x2A\x2A\x2A\x2A\x49\x8B\xD0\x48\x8B\x48\x40\x48\x8B\x49\x08\xE9\x2A\x2A\x2A\x2A\xC3", "xx????xxxxxxxxxx????xx?????xxxxxxx????xxxxxxxxxxxx????x", GetModuleHandle(NULL));
    glb::oTMalloc = (TMalloc)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xF9\xE0\x77\x3C\x48\x85\xC9\xB8\x2A\x2A\x2A\x2A\x48\x0F\x44\xD8\xEB\x15\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x25\x48\x8B\xCB\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x19\x48\x8B\x0D\x2A\x2A\x2A\x2A\x4C\x8B\xC3\x33\xD2\xFF\x15\x2A\x2A\x2A\x2A\x48\x85\xC0\x74\xD4\xEB\x0D\xE8\x2A\x2A\x2A\x2A\xC7\x00\x2A\x2A\x2A\x2A\x33\xC0\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxx????xxxxxxx????xxxxxxx????xxxxxxxx????xx????xxxxxxxx", GetModuleHandle(NULL));
    glb::oTFree = (TFree)mem::FindPattern((PBYTE)"\x48\x85\xC9\x74\x37\x53\x48\x83\xEC\x20\x4C\x8B\xC1\x33\xD2\x48\x8B\x0D\x2A\x2A\x2A\x2A\xFF\x15\x2A\x2A\x2A\x2A\x85\xC0\x75\x17\xE8\x2A\x2A\x2A\x2A\x48\x8B\xD8\xFF\x15\x2A\x2A\x2A\x2A\x8B\xC8\xE8\x2A\x2A\x2A\x2A", "xxxxxxxxxxxxxxxxxx????xx????xxxxx????xxxxx????xxx????", GetModuleHandle(NULL));
    glb::oSpawnFire = (spawnFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xB8\x48\x81\xEC\x40\x01\x00\x00\x48\xC7\x45\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89\x70\x10\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL)); //td.exe+B94C0
    glb::oPewpew = (createProjectile)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x57\x48\x81\xEC\x80\x00\x00\x00", "xxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDcreateExplosionWrapped = (createExplosionWrapped)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x30\x48\x8B\xDA\x48\xC7\x44\x24\x20\x00\x00", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDspawnParticleWrapped = (spawnParticleWrapped)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xA1\x48\x81\xEC\xE0\x00\x00\x00\x48\xC7\x45\x27\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x0F\x29\x70\xE8\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDspawnParticle = (spawnParticle)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x68\x10\x48\x89\x70\x18\x48\x89\x78\x20\x41\x56\x48\x81\xEC\x10\x06\x00\x00\x81\x39\xFF\xFF\x00\x00\x49\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDamagePlayer = (damagePlayer)mem::FindPattern((PBYTE)"\xF3\x0F\x11\x91\x5C\x01\x00\x00\xC3\xCC\xCC\xCC\xCC\x48\x8B\xC4", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oMAL = (modApiLinker)mem::FindPattern((PBYTE)"\x40\x55\x48\x8D\xAC\x24\x80\xFA\xFF\xFF\x48\x81\xEC\x80\x06\x00", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDamageObject = (damageObject)mem::FindPattern((PBYTE)"\x48\x81\xEC\xC8\x00\x00\x00\x48\xC7\x44\x24\x48\xFE\xFF\xFF\xFF\x48\x8D\x44\x24\x60\x48\x89\x44\x24\x58\xC7\x44\x24\x54\x04\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCamPos = (cameraPositioning)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\xA8\x58\xFF\xFF\xFF\x48\x81\xEC\xA0\x01\x00\x00\x48\xC7\x45\x10\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oTDmain = (tdMain)mem::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1D", "x????xxxxx????xx", GetModuleHandle(NULL));
    glb::oCreateLight = (createLight)mem::FindPattern((PBYTE)"\x48\x83\xEC\x58\x48\x8B\x05\x95\x25\x3F\x00\x80\xB8\x10\x01\x00\x00\x00\x0F\x85\xA6\x00\x00\x00\xF2\x0F\x10\x02\x8B\x42\x08\xF3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oEnvUpdate = (environmentUpdate)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x53\x56\x57\x41\x56\x48\x8D\xA8\x38\xFE\xFF\xFF\x48\x81\xEC\xA0\x02\x00\x00\x48\xC7\x45\x80\xFE\xFF\xFF\xFF\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSpreadFire = (spreadFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\xF3\x0F\x11\x48\x10\x48\x89\x48\x08\x55\x53\x56\x57", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSOA = (SetObjectAttribute)mem::FindPattern((PBYTE)"\x4C\x89\x44\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x48\xC7", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oOutlineshape = (outlineShape)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x48\x8B\x05\x63\x79\x3E\x00\x49\x8B\xD8", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::plankPatchFunction = mem::FindPattern((PBYTE)"\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x40\xCF\xFF\xFF\xB8\xC0\x31\x00\x00\xE8\xB2\x33\x19\x00\x48\x2B\xE0\x48\xC7\x45\x00\xFE\xFF\xFF\xFF\x48\x89\x9C\x24\x08\x32\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oMovLoop = (movementLoop)mem::FindPattern((PBYTE)"\x48\x89\x54\x24\x10\x55\x53\x41\x56\x48\x8D\xAC\x24\x20\xFD\xFF\xFF\x48\x81\xEC\xE0\x03\x00\x00\x48\x63\x81\x80\x01\x00\x00\x4C", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    //sadly cant sigscan for this, yet
    //0x0041CB00 requires further investigation - looks like a table of pointers for various game functions

    //live = 0x0041CB00

    glb::TDOL = *(TDObjectList**)(glb::moduleBase + 0x0042DF70);
    glb::game = (TDGame*)glb::TDOL;
    glb::scene = (TDScene*)glb::TDOL->objs[8];
    glb::renderer = (TDRenderer*)glb::TDOL->objs[7];
    glb::player = (TDPlayer*)glb::TDOL->objs[19];

         
    std::cout << "[Function Addresses:       ]" << std::endl;
    std::cout << "Create point light:    " << std::hex << glb::oCreateLight << std::endl;
    std::cout << "outlineShape:          " << std::hex << glb::oOutlineshape << std::endl;
    std::cout << "SetObjectAttribute:    " << std::hex << glb::oSOA << std::endl;
    std::cout << "Spread Fire:           " << std::hex << glb::oSpreadFire << std::endl;
    std::cout << "Envupdate:             " << std::hex << glb::oEnvUpdate << std::endl;
    std::cout << "SwapBuffers:           " << std::hex << glb::owglSwapBuffers << std::endl;
    std::cout << "Paint:                 " << std::hex << glb::oPaint << std::endl;
    std::cout << "Draw line:             " << std::hex << glb::oFDL << std::endl;
    std::cout << "Ray cast:              " << std::hex << glb::oRC << std::endl;
    std::cout << "Spawn vox:             " << std::hex << glb::oSpawnVox << std::endl;
    std::cout << "Create texture:        " << std::hex << glb::oCreateTexture << std::endl;
    std::cout << "Update shapes:         " << std::hex << glb::oUpdateShapes << std::endl;
    std::cout << "Body constructor:      " << std::hex << glb::oB_Constructor << std::endl;
    std::cout << "Shape constructor:     " << std::hex << glb::oS_Constructor << std::endl;
    std::cout << "Physics constructor:   " << std::hex << glb::oCreatePhysics << std::endl;
    std::cout << "Set dynamic:           " << std::hex << glb::oSetDynamic << std::endl;
    std::cout << "Malloc:                " << std::hex << glb::oTMalloc << std::endl;
    std::cout << "create fire:           " << std::hex << glb::oSpawnFire << std::endl;
    std::cout << "do damage:             " << std::hex << glb::oDamageObject << std::endl;
    std::cout << "spawn particle:        " << std::hex << glb::TDspawnParticle << std::endl;
    std::cout << "create explosion:      " << std::hex << glb::TDcreateExplosion << std::endl;
    std::cout << "kill me:               " << std::hex << glb::killme << std::endl;
    std::cout << "makeHole:              " << std::hex << glb::oMakeHole << std::endl;
    std::cout << "Mod linker:            " << std::hex << glb::oMAL << std::endl;
    std::cout << "Camera positioner:     " << std::hex << glb::oCamPos << std::endl;
    std::cout << "Shoot projectile:      " << std::hex << glb::oPewpew << std::endl;
    std::cout << std::endl;
    std::cout << "[API function Addresses:   ]" << std::endl;
    std::cout << "create explosion API:  " << std::hex << glb::TDcreateExplosionWrapped << std::endl;
    std::cout << "create particle API:   " << std::hex << glb::TDspawnParticleWrapped << std::endl;
    std::cout << std::endl;
    std::cout << "[Object Addresses:         ]" << std::endl;
    std::cout << "object list:           " << std::hex << glb::TDOL << std::endl;
    std::cout << "player:                " << std::hex << glb::player << std::endl;
    std::cout << "scene :                " << std::hex << glb::scene << std::endl;
    std::cout << "renderer :             " << std::hex << glb::renderer << std::endl;
    std::cout << "playerX:               " << &glb::player->cameraPosition.x << std::endl;
    std::cout << "playerY:               " << &glb::player->cameraPosition.y << std::endl;
    std::cout << "playerZ:               " << &glb::player->cameraPosition.z << std::endl;

    //for (int i = 0; i < 128; i++) {
    //    std::cout << i << " : " << std::hex << glb::TDOL->objs[i] << std::endl;
    //}

}
