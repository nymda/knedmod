#include "TDFuncs.h"
#include "global.h"

void sigscanItems() {
    glb::oPaint = (tPaint)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x55\x41\x56\x48\x8D\x68\xD8\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxx?????", GetModuleHandle(NULL));
    glb::oFDL = (frameDrawLine)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x2A\x48\x89\x6C\x24\x2A\x48\x89\x74\x24\x2A\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x80\x7C\x24\x2A\x2A", "xxxx?xxxx?xxxx?xxxxxxxxxxxx??", GetModuleHandle(NULL));
    glb::oRC = (rayCast)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x4C\x89\x40\x18\x48\x89\x50\x10\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x38\xFC\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDcreateExplosion = (createExplosion)mem::FindPattern((PBYTE)"\x48\x8B\xC4\xF3\x0F\x11\x50\x18\x55", "xxxxxxxxx", GetModuleHandle(NULL));
    glb::oSpawnVox = (tSpawnVox)mem::FindPattern((PBYTE)"\x4C\x8B\xDC\x57\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\xC7\x44\x24\x2A\x2A\x2A\x2A\x2A", "xxxxxxx????xxxx?????", GetModuleHandle(NULL));
    glb::killme = (idfk)mem::FindPattern((PBYTE)"\x48\x89\x54\x24\x10\x55\x53\x56\x41\x54\x41\x56\x48\x8D\xAC\x24\x40\xCB\xFF\xFF\xB8\xC0\x35\x00\x00\xE8\x52\x1A\x15\x00\x48\x2B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));

    glb::oMakeHole = (makeHole)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x53\x48\x81\xEC\x90\x00\x00\x00\x0F\x29\x70\xE8\x4C\x8D\x40\xA8\x48\x8B\xDA\x0F\x29\x78\xD8\x44\x0F\x29\x40\xC8\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreateTexture = (CreateTexture)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x41\x54\x41\x55\x41\x57", "xxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCreatePhysics = (CreatePhysics)mem::FindPattern((PBYTE)"\x40\x53\x56\x57\x41\x55", "xxxxxx", GetModuleHandle(NULL));
    glb::oUpdateShapes = (UpdateShapes)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x2A\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x2A\x2A\x2A\x2A\xB8\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x48\x2B\xE0\x48\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x48\x89\x9C\x24\x2A\x2A\x2A\x2A\x0F\x29\xB4\x24\x2A\x2A\x2A\x2A\x0F\x29\xBC\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\x84\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\x8C\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\x94\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\x9C\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\xA4\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\xAC\x24\x2A\x2A\x2A\x2A\x44\x0F\x29\xB4\x24", "xxxx?xxxxxxxxxxxxxxx????x????x????xxxxxx????????xxxx????xxxx????xxxx????xxxxx????xxxxx????xxxxx????xxxxx????xxxxx????xxxxx????xxxxx", GetModuleHandle(NULL));
    glb::oB_Constructor = (B_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oS_Constructor = (S_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x02\x00\x00\x00", "xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oSetDynamic = (SetDynamic)mem::FindPattern((PBYTE)"\x88\x91\x2A\x2A\x2A\x2A\x4C\x8B\xC1\x84\xD2\x74\x29\x0F\xB6\x81\x2A\x2A\x2A\x2A\xC6\x81\x2A\x2A\x2A\x2A\x2A\x84\xC0\x75\x17\x48\x8B\x05\x2A\x2A\x2A\x2A\x49\x8B\xD0\x48\x8B\x48\x40\x48\x8B\x49\x08\xE9\x2A\x2A\x2A\x2A\xC3", "xx????xxxxxxxxxx????xx?????xxxxxxx????xxxxxxxxxxxx????x", GetModuleHandle(NULL));
    glb::oTMalloc = (TMalloc)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xF9\xE0\x77\x3C\x48\x85\xC9\xB8\x2A\x2A\x2A\x2A\x48\x0F\x44\xD8\xEB\x15\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x25\x48\x8B\xCB\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x19\x48\x8B\x0D\x2A\x2A\x2A\x2A\x4C\x8B\xC3\x33\xD2\xFF\x15\x2A\x2A\x2A\x2A\x48\x85\xC0\x74\xD4\xEB\x0D\xE8\x2A\x2A\x2A\x2A\xC7\x00\x2A\x2A\x2A\x2A\x33\xC0\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxx????xxxxxxx????xxxxxxx????xxxxxxxx????xx????xxxxxxxx", GetModuleHandle(NULL));
    glb::oDamageStuff = (damageStuff)mem::FindPattern((PBYTE)"\x4c\x89\x44\x24\x18\x48\x89\x4c\x24\x08\x55\x56\x57\x41\x54", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL)); //td.exe+EFE60
    glb::oSpawnFire = (spawnFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xC8\x48\x81\xEC\x30\x01\x00\x00", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL)); //td.exe+B94C0
    glb::oPewpew = (pewpew)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x57\x48\x81\xEC\x80\x00\x00\x00", "xxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDcreateExplosionWrapped = (createExplosionWrapped)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x48\x8B\xDA\x0F\x29\x74\x24\x30\x48\x8B\xCB\x48\xC7\x44\x24\x20\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDspawnParticleWrapped = (spawnParticleWrapped)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\x68\xA1\x48\x81\xEC\xE0\x00\x00\x00\x48\xC7\x45\x27\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x0F\x29\x70\xE8\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::TDspawnParticle = (spawnParticle)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x68\x10\x48\x89\x70\x18\x48\x89\x78\x20\x41\x56\x48\x81\xEC\x10\x06\x00\x00\x81\x39\xFF\xFF\x00\x00\x49\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDamagePlayer = (damagePlayer)mem::FindPattern((PBYTE)"\xF3\x0F\x11\x91\x5C\x01\x00\x00\xC3\xCC\xCC\xCC\xCC\x48\x8B\xC4", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oMAL = (modApiLinker)mem::FindPattern((PBYTE)"\x40\x55\x48\x8D\xAC\x24\x90\xFB\xFF\xFF\x48\x81\xEC\x70\x05\x00\x00\x48\xC7\x85\x60\x04\x00\x00\xFE\xFF\xFF\xFF\x48\x89\x9C\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oDamageObject = (damageObject)mem::FindPattern((PBYTE)"\x4C\x89\x44\x24\x18\x48\x89\x4C\x24\x08\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x20\xE4\xFF\xFF\xB8\xE0\x1C", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));
    glb::oCamPos = (cameraPositioning)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\xA8\x58\xFF\xFF\xFF\x48\x81\xEC\xA0\x01\x00\x00\x48\xC7\x45\x10\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL));

    //sadly cant sigscan for this, yet
    //0x0041CB00 requires further investigation - looks like a table of pointers for various game functions

    glb::TDOL = *(TDObjectList**)(glb::moduleBase + 0x0041CB00);
    glb::scene = (TDScene*)glb::TDOL->objs[8];
    glb::renderer = (TDRenderer*)glb::TDOL->objs[7];
    glb::player = (TDPlayer*)glb::TDOL->objs[19];

         
    std::cout << "[Function Addresses:       ]" << std::endl;
    std::cout << "SwapBuffers:           " << std::hex << glb::owglSwapBuffers << std::endl;
    std::cout << "Paint:                 " << std::hex << glb::oPaint << std::endl;
    std::cout << "Draw line:             " << std::hex << glb::oFDL << std::endl;
    std::cout << "Ray cast:              " << std::hex << glb::oRC << std::endl;
    std::cout << "Spawn vox:             " << std::hex << glb::oSpawnVox << std::endl;
    std::cout << "Create texture:        " << std::hex << glb::oCreateTexture << std::endl;
    std::cout << "Update ents:           " << std::hex << glb::oUpdateShapes << std::endl;
    std::cout << "Body constructor:      " << std::hex << glb::oB_Constructor << std::endl;
    std::cout << "Shape constructor:     " << std::hex << glb::oS_Constructor << std::endl;
    std::cout << "Set dynamic:           " << std::hex << glb::oSetDynamic << std::endl;
    std::cout << "Malloc:                " << std::hex << glb::oTMalloc << std::endl;
    std::cout << "create fire:           " << std::hex << glb::oSpawnFire << std::endl;
    std::cout << "do damage:             " << std::hex << glb::oDamageStuff << std::endl;
    std::cout << "spawn particle:        " << std::hex << glb::TDspawnParticle << std::endl;
    std::cout << "create explosion:      " << std::hex << glb::TDcreateExplosion << std::endl;
    std::cout << "kill me:               " << std::hex << glb::killme << std::endl;
    std::cout << "makeHole:              " << std::hex << glb::oMakeHole << std::endl;
    std::cout << "Mod linker:            " << std::hex << glb::oMAL << std::endl;
    std::cout << "Camera positioner:     " << std::hex << glb::oCamPos << std::endl;
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
    //for (int i = 0; i < 128; i++) {
    //    std::cout << i << " : " << std::hex << glb::TDOL->objs[i] << std::endl;
    //}

}
