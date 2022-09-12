#include "TDFuncs.h"
#include "global.h"

void printIntegPercentage(float p) {
    int cCode = 10;
    if (p < 100.f) {
        cCode = 14;
    }
    if (p < 75.f) {
        cCode = 12;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cCode);
    printf_s(" (%0.1f%%)\n", p);
    SetConsoleTextAttribute(hConsole, 15);
}

void sigscanItems() {
    //glb::oTdDelBdy = (deleteBody)mem::FindPattern((PBYTE)"\x48\x83\xEC\x28\x48\x8B\xCA\x33\xD2\xE8\x82\x16\x0F\x00\x48\x8B", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oMakeHole = (makeHole)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x53\x48\x81\xEC\x90\x00\x00\x00\x0F\x29\x70\xE8\x4C\x8D\x40\xA8\x48\x8B\xDA\x0F\x29\x78\xD8\x44\x0F\x29\x40\xC8\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oMAL = (modApiLinker)mem::FindPattern((PBYTE)"\x40\x55\x48\x8D\xAC\x24\x80\xFA\xFF\xFF\x48\x81\xEC\x80\x06\x00", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oTDmain = (tdMain)mem::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1D", "x????xxxxx????xx", GetModuleHandle(NULL), &percentage);
    //glb::oCreateLight = (createLight)mem::FindPattern((PBYTE)"\x48\x83\xEC\x58\x48\x8B\x05\x95\x25\x3F\x00\x80\xB8\x10\x01\x00\x00\x00\x0F\x85\xA6\x00\x00\x00\xF2\x0F\x10\x02\x8B\x42\x08\xF3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oEnvUpdate = (environmentUpdate)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x53\x56\x57\x41\x56\x48\x8D\xA8\x38\xFE\xFF\xFF\x48\x81\xEC\xA0\x02\x00\x00\x48\xC7\x45\x80\xFE\xFF\xFF\xFF\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::plankPatchFunction = (uintptr_t)mem::FindPattern((PBYTE)"\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x50\xCF\xFF\xFF\xB8\xB0\x31\x00\x00\xE8\xF2\xE8\x19\x00\x48\x2B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oHandleBombObjs = (handleBombObjs)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x57\x41\x56\x48\x8D\xA8\x18\xFF\xFF\xFF\x48\x81", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oSetShapeParentBody = (setShapeParentBody)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8D\x05\x93\x51\x24\x00\x88\x51\x08", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oAddCItem = (addContextItem)mem::FindPattern((PBYTE)"\x44\x89\x44\x24\x18\x88\x54\x24\x10\x55\x56\x57\x41\x56\x48\x8D\x6C\x24\xF8\x48\x81\xEC\x08\x01\x00\x00\x4C\x8B\x15\x5F\xE5\x3B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oIUnReadVox = (unknReadVoxData)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x88\xD9\xFF\xFF\xB8\x78\x27\x00\x00\xE8\x6D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oLtDBin = (loadTDBIN)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\xA8\x58\xFF\xFF\xFF\x48\x81\xEC\xA0\x01\x00\x00\x48\xC7\x44\x24\x68\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::o_S140152540 = (S140152540)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x48\x83\xEC\x30\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x48\x48\x89\x6C\x24\x50\x48\x89\x74\x24\x58\x41\x8B\xF0\x48\x8B\xEA\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::o_S1400C4F70 = (S1400C4F70)mem::FindPattern((PBYTE)"\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xD9\x48\x81\xEC\xB0\x00\x00\x00\x48\xC7\x45\x1F\xFE\xFF\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::o_S140152740 = (S140152740)mem::FindPattern((PBYTE)"\x40\x57\x48\x83\xEC\x30\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x40\x48\x8B\xF9\x48\x8D\x05\x92\xFD\x1F\x00\x48\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oValidate = (validateFileExistance)mem::FindPattern((PBYTE)"\x40\x57\x48\x83\xEC\x60\x48\xC7\x44\x24\x28\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x70\x49\x8B\xF8\x48\x8B\xDA\xC7\x44\x24\x20\x00\x00\x00\x00\x48\x8D\x4C\x24\x50\xE8\xF4\xFA\x02\x00\x90\x48\x8B\xD0", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oDoQuicksave = (doQuicksave)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x8B\xD9\x48\x8B\x0D\xE4\xB3\x36\x00\x48\x8B\x89\xA0", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oDoQuickload = (doQuickload)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x57\x48\x83\xEC\x70\x48\xC7\x40\xA8\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89\x70\x10\x48\x8B\xF1\x48\x8B\x0D\xEE\xB4", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::tdConstructScreen = (constructScreen)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x07\x00\x00\x00\xE8\x6A\x77\x05\x00\xC7\x43\x4C\xCD\xCC\xCC\x3D\x48\x8D\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::oUnknGraphicsInitFunction = (unknGraphicsInitFunction)mem::FindPattern((PBYTE)"\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\xE9\xBA\x7D\x14\x00", "xxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::tdInitScreenSecondary = (initScreenSecondary)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xDA\x48\x8B\xF9\xE8\x3B\xA7\xFD\xFF\x48\x8D\x57\x28\x48\x8B\xCB\xE8\x4F\x94\x01", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::tdRaycastCrashA = (raycastCrashA)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x7C\x24\x10\x55\x48\x8B\xEC\x48\x83\xEC\x70\xF3\x0F\x10\x02\x48\x8B\xF9\xF3\x41\x0F\x58\x00\xF3\x0F", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::tdRaycastCrashB = (raycastCrashB)mem::FindPattern((PBYTE)"\x48\x83\xEC\x08\x4C\x8B\xD2\x4C\x8B\xC9\x8B\x52\x70\x85\x11\x75\x07\x32\xC0\x48\x83\xC4\x08\xC3\x8B\x41\x04\x23\xC2\x3B\x41\x04", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);

    //glb::osub_140105F30 = (sub_140105F30)mem::FindPattern((PBYTE)"\x48\x8B\x41\x10\x48\x85\xC0\x74\x18\x0F\x1F\x80\x00\x00\x00\x00", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::osub_140146470 = (sub_140146470)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8D\x41\x28\x48\x8B\xDA\x48\x8B\x49", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //glb::osub_140032EA0 = (sub_140032EA0)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x68\x10\x48\x89\x70\x18\x57", "xxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);


    //sadly cant sigscan for this, yet
    //0x0041CB00 requires further investigation - looks like a table of pointers for various game functions

    //live = 0x0041CB00
    bool sigScanError = false;
    float percentage = 0;

    glb::TDOL = *(TDObjectList**)(glb::moduleBase + 0x00456140);
    glb::game = (TDGame*)glb::TDOL;
    glb::scene = (TDScene*)glb::TDOL->objs[9];
    glb::renderer = (TDRenderer*)glb::TDOL->objs[8];
    glb::player = (TDPlayer*)glb::TDOL->objs[20];

    std::cout << "" << std::endl;
    std::cout << "[Function Addresses:       ]" << std::endl;
    std::cout << "cursor               : " << std::hex << glb::ocursor << std::endl;
    std::cout << "swapbuffers          : " << std::hex << glb::owglSwapBuffers << std::endl;

    glb::oLoadResource = (loadResource)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x45\xD7\xFE\xFF\xFF\xFF\x48", "xxxxxxxxxxxxxxxxxxxx???xxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("loadResource         : %p", glb::oLoadResource);
    printIntegPercentage(percentage);
    if (!glb::oLoadResource) { sigScanError = true; }

    glb::oValidatePath = (validatePath)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\x90\x00\x00\x00\x48\xC7\x45\xDF\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxx???xxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oValidatePath        : %p", glb::oValidatePath);
    printIntegPercentage(percentage);
    if (!glb::oValidatePath) { sigScanError = true; }

    glb::oCreateTextureThing = (createTextureThing)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\xF2\x48\x8B\xD9\x8B\x51\x0C\x41\x0F\xB6\xF8\xB9\xE1\x0D\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?", GetModuleHandle(NULL), &percentage);
    printf_s("oCreateTextureThing  : %p", glb::oCreateTextureThing);
    printIntegPercentage(percentage);
    if (!glb::oCreateTextureThing) { sigScanError = true; }

    glb::tdConstructJoint = (joinConstructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x07\x00\x00", "xxxxxxxxxxxxxx??", GetModuleHandle(NULL), &percentage);
    printf_s("tdConstructJoint     : %p", glb::tdConstructJoint);
    printIntegPercentage(percentage);
    if (!glb::tdConstructJoint) { sigScanError = true; }

    glb::tdInitBall = (initBall)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x89\x51\x28\x48\x8B\xF1\x4C\x89\x41\x30\x49\x8B\xF8\x48\x8D\x4A", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdInitBall           : %p", glb::tdInitBall);
    printIntegPercentage(percentage);
    if (!glb::tdInitBall) { sigScanError = true; }

    glb::tdInitWire = (initWire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8D\x68\xC1\x48\x81\xEC\xD0\x00\x00\x00\x48\xC7\x45\x1F\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48", "xxxxxxxxxxxxxxxx???xxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdInitWire           : %p", glb::tdInitWire);
    printIntegPercentage(percentage);
    if (!glb::tdInitWire) { sigScanError = true; }

    glb::tdAttachJoint = (attachJoint)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x89\x51\x28\x48\x8B\xF1\x4C\x89\x41\x30\x49\x8B\xF8\x48\x8D\x4A", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("tdAttachJoint        : %p", glb::tdAttachJoint);
    printIntegPercentage(percentage);
    if (!glb::tdAttachJoint) { sigScanError = true; }

    glb::oPaint = (tPaint)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x55\x41\x56\x48\x8D\x68\xD8\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxx?????", GetModuleHandle(NULL), &percentage);
    printf_s("tPaint               : %p", glb::oPaint);
    printIntegPercentage(percentage);
    if (!glb::oPaint) { sigScanError = true; }

    glb::oFDL = (frameDrawLine)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x44\x8B\xB9\x10\x89\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx??x", GetModuleHandle(NULL), &percentage);
    printf_s("drawLine             : %p", glb::oFDL);
    printIntegPercentage(percentage);
    if (!glb::oFDL) { sigScanError = true; }

    glb::oRC = (rayCast)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x4C\x89\x40\x18\x48\x89\x50\x10\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x18\xF4\xFF\xFF\x48\x81\xEC", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("raycast              : %p", glb::oRC);
    printIntegPercentage(percentage);
    if (!glb::oRC) { sigScanError = true; }

    glb::TDcreateExplosion = (createExplosion)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\xA8\xFD\xFF\xFF\x48\x81\xEC\x30\x03\x00\x00\x48\xC7\x45\x20\xFE\xFF", "xxxxxxxxxxxxxxxxxxxxxxxx??xxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("create explosion     : %p", glb::TDcreateExplosion);
    printIntegPercentage(percentage);
    if (!glb::TDcreateExplosion) { sigScanError = true; }

    glb::oSpawnVox = (tSpawnVox)mem::FindPattern((PBYTE)"\x4C\x8B\xDC\x57\x48\x81\xEC\xF0\x00\x00\x00\x48\xC7\x44\x24\x60", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("spawn vox            : %p", glb::oSpawnVox);
    printIntegPercentage(percentage);
    if (!glb::oSpawnVox) { sigScanError = true; }

    glb::oWrappedDamage = (wrappedDoDamage)mem::FindPattern((PBYTE)"\x48\x83\xEC\x48\x48\x8B\x44\x24\x78\x4C\x8B\xC2\xF3\x0F\x10\x44", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("make hole            : %p", glb::oWrappedDamage);
    printIntegPercentage(percentage);
    if (!glb::oWrappedDamage) { sigScanError = true; }

    glb::oCreateTexture = (CreateTexture)mem::FindPattern((PBYTE)"\x48\x89\x4C\x24\x08\x57\x41\x54\x41\x55\x41\x57", "xxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("create texture       : %p", glb::oCreateTexture);
    printIntegPercentage(percentage);
    if (!glb::oCreateTexture) { sigScanError = true; }

    glb::oCreatePhysics = (CreatePhysics)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x50\x04\x00\x00\x48\x8B\xD9\xE8\xFF\x67\x00\x00\x48", "xxxxxxx??xxxxxx??x", GetModuleHandle(NULL), &percentage);
    printf_s("create physics       : %p", glb::oCreatePhysics);
    printIntegPercentage(percentage);
    if (!glb::oCreatePhysics) { sigScanError = true; }

    glb::oUpdateShapes = (UpdateShapes)mem::FindPattern((PBYTE)"\x40\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x10\xEB", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("update shapes        : %p", glb::oUpdateShapes);
    printIntegPercentage(percentage);
    if (!glb::oUpdateShapes) { sigScanError = true; }

    glb::oB_Constructor = (B_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x01\x00\x00\x00", "xxxxxxxxxxxxxx???", GetModuleHandle(NULL), &percentage);
    printf_s("construct body       : %p", glb::oB_Constructor);
    printIntegPercentage(percentage);
    if (!glb::oB_Constructor) { sigScanError = true; }

    glb::oS_Constructor = (S_Constructor)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x02\x00\x00\x00", "xxxxxxxxxxxxxx???", GetModuleHandle(NULL), &percentage);
    printf_s("construct shape      : %p", glb::oS_Constructor);
    printIntegPercentage(percentage);
    if (!glb::oS_Constructor) { sigScanError = true; }

    glb::oSetDynamic = (SetDynamic)mem::FindPattern((PBYTE)"\x88\x91\xDC\x00\x00\x00\x4C\x8B\xC1\x84\xD2\x74\x29\x0F\xB6\x81\xE4\x00\x00\x00\xC6\x81\xE4\x00\x00\x00\x3C\x84\xC0\x75\x17\x48", "xxx???xxxxxxxxxxx???xxx???xxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("set dynamic          : %p", glb::oSetDynamic);
    printIntegPercentage(percentage);
    if (!glb::oSetDynamic) { sigScanError = true; }

    glb::oTMalloc = (TMalloc)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x83\xF9\xE0\x77\x3C\x48\x85\xC9\xB8\x2A\x2A\x2A\x2A\x48\x0F\x44\xD8\xEB\x15\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x25\x48\x8B\xCB\xE8\x2A\x2A\x2A\x2A\x85\xC0\x74\x19\x48\x8B\x0D\x2A\x2A\x2A\x2A\x4C\x8B\xC3\x33\xD2\xFF\x15\x2A\x2A\x2A\x2A\x48\x85\xC0\x74\xD4\xEB\x0D\xE8\x2A\x2A\x2A\x2A\xC7\x00\x2A\x2A\x2A\x2A\x33\xC0\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxx????xxxxxxx????xxxxxxx????xxxxxxxx????xx????xxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("malloc               : %p", glb::oTMalloc);
    printIntegPercentage(percentage);
    if (!glb::oTMalloc) { sigScanError = true; }

    glb::oTFree = (TFree)mem::FindPattern((PBYTE)"\x48\x85\xC9\x74\x37\x53\x48\x83\xEC\x20\x4C\x8B\xC1\x33\xD2\x48\x8B\x0D\x2A\x2A\x2A\x2A\xFF\x15\x2A\x2A\x2A\x2A\x85\xC0\x75\x17\xE8\x2A\x2A\x2A\x2A\x48\x8B\xD8\xFF\x15\x2A\x2A\x2A\x2A\x8B\xC8\xE8\x2A\x2A\x2A\x2A", "xxxxxxxxxxxxxxxxxx????xx????xxxxx????xxxxx????xxx????", GetModuleHandle(NULL), &percentage);
    printf_s("free                 : %p", glb::oTFree);
    printIntegPercentage(percentage);
    if (!glb::oTFree) { sigScanError = true; }

    glb::oSpawnFire = (spawnFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\x98", "xxxxxxxxxxxxx??x", GetModuleHandle(NULL), &percentage); //td.exe+B94C0
    printf_s("spawn fire           : %p", glb::oSpawnFire);
    printIntegPercentage(percentage);
    if (!glb::oSpawnFire) { sigScanError = true; }

    glb::oPewpew = (createProjectile)mem::FindPattern((PBYTE)"\x40\x53\x48\x81\xEC\x80\x00\x00\x00\xF2\x0F\x10\x02\xF3\x0F\x10\x8C\x24\xB8\x00\x00\x00\x8B\x42\x08\xF2\x0F\x11\x44\x24\x30\xF3", "xxxxxx???xxxxxxxxxx???xxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("shoot projectile     : %p", glb::oPewpew);
    printIntegPercentage(percentage);
    if (!glb::oPewpew) { sigScanError = true; }

    glb::TDspawnParticle = (spawnParticle)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8D\x99\x80\x00\x00\x00\x49\x8B\xF1\x81\x3B", "xxxxxxxxxxxxxxxxxxxxxxxx???xxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("spawn particle       : %p", glb::TDspawnParticle);
    printIntegPercentage(percentage);
    if (!glb::TDspawnParticle) { sigScanError = true; }

    glb::oDamagePlayer = (damagePlayer)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x0F\x29\x74\x24\x30\x48\x8B\xD9\x0F\x29", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("damage player        : %p", glb::oDamagePlayer);
    printIntegPercentage(percentage);
    if (!glb::oDamagePlayer) { sigScanError = true; }

    glb::oDamageObject = (damageObject)mem::FindPattern((PBYTE)"\x48\x81\xEC\xC8\x00\x00\x00\x48\xC7\x44\x24\x48\xFE\xFF\xFF\xFF\x48\x8D\x44\x24\x60\x48\x89\x44\x24\x58\xC7\x44\x24\x54\x04\x00", "xxxx???xxxxxxxxxxxxxxxxxxxxxxxx?", GetModuleHandle(NULL), &percentage);
    printf_s("damage object 2      : %p", glb::oDamageObject);
    printIntegPercentage(percentage);
    if (!glb::oDamageObject) { sigScanError = true; }

    glb::oCamPos = (cameraPositioning)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x48\x8D\xA8\x38\xFF\xFF\xFF\x48\x81\xEC\xC0\x01\x00\x00\x48\xC7\x45\x10\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x48\x89", "xxxxxxxxxxxxxxxx??xxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("cam positioner       : %p", glb::oCamPos);
    printIntegPercentage(percentage);
    if (!glb::oCamPos) { sigScanError = true; }

    glb::oSpreadFire = (spreadFire)mem::FindPattern((PBYTE)"\x48\x8B\xC4\xF3\x0F\x11\x48\x10\x48\x89\x48\x08\x55\x53\x56\x57", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("fire spread          : %p", glb::oSpreadFire);
    printIntegPercentage(percentage);
    if (!glb::oSpreadFire) { sigScanError = true; }

    glb::oSOA = (SetObjectAttribute)mem::FindPattern((PBYTE)"\x4C\x89\x44\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x48\xC7", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("set attribute        : %p", glb::oSOA);
    printIntegPercentage(percentage);
    if (!glb::oSOA) { sigScanError = true; }

    //glb::oSOAS = (SetObjectAttributeSingle)mem::FindPattern((PBYTE)"\x40\x57\x41\x57\x48\x83\xEC\x58\xF6\x41\x09\x01\x4C\x8B\xFA\x48\x8B\xF9\x0F\x84\x21\x01\x00\x00\x48\x89\x5C\x24\x70\x48\x8D\x15", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    //printf_s("remove attribute     : %p", glb::oSOAS);
    //printIntegPercentage(percentage);
    //if (!glb::oSOAS) { sigScanError = true; }

    glb::oHighlightShape = (highlightShape)mem::FindPattern((PBYTE)"\x48\x83\xEC\x48\x48\x8B\x05\x25\xA1\x40\x00\x0F\x29\x74\x24\x30", "xxxxxxxxxx?xxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("highlight shape      : %p", glb::oHighlightShape);
    printIntegPercentage(percentage);
    if (!glb::oHighlightShape) { sigScanError = true; }

    glb::oOutlineShape = (outlineShape)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x40\x48\x8B\x05\xD3\xA0\x40\x00\x49\x8B\xD8", "xxxxxxxxxxxx?xxx", GetModuleHandle(NULL), &percentage);
    printf_s("outline shape        : %p", glb::oOutlineShape);
    printIntegPercentage(percentage);
    if (!glb::oOutlineShape) { sigScanError = true; }

    glb::oOutlineBody = (outlineBody)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x57\x48\x81\xEC\x00\x01\x00\x00\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x58\x08\x0F\x29\x70\xE8\x0F\x29\x78\xD8", "xxxxxxx?x??xxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("outline body         : %p", glb::oOutlineBody);
    printIntegPercentage(percentage);
    if (!glb::oOutlineBody) { sigScanError = true; }

    glb::oQuatEul = (apiQuatEuler)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x58\x08\x57\x48\x81\xEC\x80\x00\x00\x00\x0F\x29\x70\xE8\x48\x8B\xDA\xF3\x0F\x10\x72\x04\x48\x8B\xF9\x0F\x29", "xxxxxxxxxxxx???xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("QuatEuler            : %p", glb::oQuatEul);
    printIntegPercentage(percentage);
    if (!glb::oQuatEul) { sigScanError = true; }

    glb::oSetBody = (setBodySomething)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("setBody              : %p", glb::oSetBody);
    printIntegPercentage(percentage);
    if (!glb::oSetBody) { sigScanError = true; }

    glb::TDreadSubobjects = (ReadSubobjectsFromVox)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x57\x41\x56\x48\x8D\x68\xA1\x48\x81\xEC\xB0\x00\x00\x00\x48\xC7\x45\x3F\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89", "xxxxxxxxxxxxxxx???xxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("Read subobjects      : %p", glb::TDreadSubobjects);
    printIntegPercentage(percentage);
    if (!glb::TDreadSubobjects) { sigScanError = true; }

    glb::oIsActive = (isActiveWindow)mem::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xFF\x15\xC1\x20\x1D\x00\x48\x8B\x4B\x08\x48\x3B\x41\x10\x0F\x94\xC0\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxx?xxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("oIsActiveWindow      : %p", glb::oIsActive);
    printIntegPercentage(percentage);
    if (!glb::oIsActive) { sigScanError = true; }

    glb::tdUpdateShapeBody = (updateShapeBody)mem::FindPattern((PBYTE)"\x4C\x8B\xC1\x48\x8B\x49\x10\x48\x8B\x41\x20\x49\x3B\xC0\x75\x0A\x49\x8B\x40\x18\x48\x89\x41\x20\xEB\x21\x48\x85\xC0\x74\x1C\x90", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("update shape body    : %p", glb::tdUpdateShapeBody);
    printIntegPercentage(percentage);
    if (!glb::tdUpdateShapeBody) { sigScanError = true; }

    glb::tdUpdateFunc = (interestingUpdateFunc)mem::FindPattern((PBYTE)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("update body weird    : %p", glb::tdUpdateFunc);
    printIntegPercentage(percentage);
    if (!glb::tdUpdateFunc) { sigScanError = true; }

    glb::tdFunRuiner = (funRuiner)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x28", "xxxxxxxxxxxxxxxx", GetModuleHandle(NULL), &percentage);
    printf_s("fun ruiner           : %p", glb::tdFunRuiner);
    printIntegPercentage(percentage);
    if (!glb::tdFunRuiner) { sigScanError = true; }

    glb::tdApplyForce = (applyForce)mem::FindPattern((PBYTE)"\x48\x8B\xC4\x48\x89\x70\x18\x55\x41\x56\x41\x57\x48\x8D\x68\xA9\x48\x81\xEC\x00\x01\x00\x00\x80\xB9\xDC\x00\x00\x00\x00\x4D\x8B", "xxxxxxxxxxxxxxxxxxx?x??xxx????xx", GetModuleHandle(NULL), &percentage);
    printf_s("apply force          : %p", glb::tdApplyForce);
    printIntegPercentage(percentage);
    if (!glb::tdApplyForce) { sigScanError = true; }

    if (sigScanError) {
        std::cout << "[F] FAILED TO FIND A CRITICAL FUNCTION, EXPECT ISSUES" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "[Object Addresses:         ]" << std::endl;
    std::cout << "Game               : " << std::hex << glb::TDOL << std::endl;
    std::cout << "Player             : " << std::hex << glb::player << std::endl;
    std::cout << "Scene              : " << std::hex << glb::scene << std::endl;
    std::cout << "Renderer           : " << std::hex << glb::renderer << std::endl;
    std::cout << "" << std::endl;

    //for (int i = 0; i < 128; i++) {
    //    std::cout << i << " : " << std::hex << glb::TDOL->objs[i] << std::endl;
    //}

}
