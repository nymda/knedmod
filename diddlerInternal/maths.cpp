#include <cmath>
#include "types.h"
#include "maths.h"

const long pi = 3.1415926535;

td::Vec3 quat2euler(td::Vec4 quat) {
    float vecX = -(2 * (quat.x * quat.z + quat.w * quat.y));
    float vecY = -(2 * (quat.y * quat.z - quat.w * quat.x));
    float vecZ = -(1 - 2 * (quat.x * quat.x + quat.y * quat.y));
    return { vecX, vecY, vecZ };
}

td::Vec4 degreeToQuaternion(double x, double y, double z) // yaw (Z), pitch (Y), roll (X)
{
    z = z * pi / 180.;
    y = y * pi / 180.;
    x = x * pi / 180.;

    return radianToQuaternion(z, y, x);
}

td::Vec4 radianToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    float w = cy * cp * cr + sy * sp * sr;
    float x = cy * cp * sr - sy * sp * cr;
    float y = sy * cp * sr + cy * sp * cr;
    float z = sy * cp * cr - cy * sp * sr;

    return { x, y, z, w };
}