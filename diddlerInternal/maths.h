#pragma once
#include <cmath>
#include "types.h"

td::Vec3 quat2euler(td::Vec4 quat);

td::Vec4 degreeToQuaternion(double x, double y, double z);

td::Vec4 radianToQuaternion(double yaw, double pitch, double roll);