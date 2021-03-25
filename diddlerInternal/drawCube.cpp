#include "types.h"
#include "Global.h"

void drawCube(td::Vec3 position, float size, td::Color color) {
    //0.25
    td::Vec3 pos1 = { position.x - size, position.y - size, position.z - size };
    td::Vec3 pos2 = { position.x - size, position.y - size, position.z + size };
    td::Vec3 pos3 = { position.x + size, position.y - size, position.z + size };
    td::Vec3 pos4 = { position.x + size, position.y - size, position.z - size };

    td::Vec3 pos1_high = { position.x - size, position.y + size, position.z - size };
    td::Vec3 pos2_high = { position.x - size, position.y + size, position.z + size };
    td::Vec3 pos3_high = { position.x + size, position.y + size, position.z + size };
    td::Vec3 pos4_high = { position.x + size, position.y + size, position.z - size };

    //bottom square
    glb::oFDL(glb::renderer, pos1, pos2, color, color, false);
    glb::oFDL(glb::renderer, pos2, pos3, color, color, false);
    glb::oFDL(glb::renderer, pos3, pos4, color, color, false);
    glb::oFDL(glb::renderer, pos4, pos1, color, color, false);

    //top square
    glb::oFDL(glb::renderer, pos1_high, pos2_high, color, color, false);
    glb::oFDL(glb::renderer, pos2_high, pos3_high, color, color, false);
    glb::oFDL(glb::renderer, pos3_high, pos4_high, color, color, false);
    glb::oFDL(glb::renderer, pos4_high, pos1_high, color, color, false);

    //walls
    glb::oFDL(glb::renderer, pos1, pos1_high, color, color, false);
    glb::oFDL(glb::renderer, pos2, pos2_high, color, color, false);
    glb::oFDL(glb::renderer, pos3, pos3_high, color, color, false);
    glb::oFDL(glb::renderer, pos4, pos4_high, color, color, false);
}