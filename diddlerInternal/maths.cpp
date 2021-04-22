#include <cmath>
#include "types.h"
#include "maths.h"

const long pi = 3.1415926535;

td::Vec3 quat2euler(td::Vec4 quat, int rot = 0, bool invert = false) {
    if (rot == 0) {
        float vecX = (2 * (quat.x * quat.z + quat.w * quat.y));
        float vecY = (2 * (quat.y * quat.z - quat.w * quat.x));
        float vecZ = (1 - 2 * (quat.x * quat.x + quat.y * quat.y));
        if (invert) {
            return { -vecX, -vecY, -vecZ };
        }
        else {
            return { vecX, vecY, vecZ };
        }
    }
    if (rot == 1) {

        float vecX = (1 - 2 * (quat.y * quat.y + quat.z * quat.z));
        float vecY = (    2 * (quat.x * quat.y + quat.w * quat.z));
        float vecZ = (    2 * (quat.x * quat.z - quat.w * quat.y));

        if (invert) {
            return { -vecX, -vecY, -vecZ };
        }
        else {
            return { vecX, vecY, vecZ };
        }
    }
    if (rot == 2) {
        float vecX = (2 * (quat.x * quat.z + quat.w * quat.y));
        float vecY = (1 - 2 * (quat.y * quat.z - quat.w * quat.x));
        float vecZ = (2 * (quat.x * quat.x + quat.y * quat.y));
        if (invert) {
            return { -vecX, -vecY, -vecZ };
        }
        else {
            return { vecX, vecY, vecZ };
        }
    }
    return { 0, 0, 0 };
}

//td::Vec4 degreeToQuaternion(double x, double y, double z) // yaw (Z), pitch (Y), roll (X)
//{
//    z = z * pi / 180.;
//    y = y * pi / 180.;
//    x = x * pi / 180.;
//
//    return radianToQuaternion(z, y, x);
//}


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

td::Vec4 euler2quat(float x, float y, float z) {
    float pitch = asin(-y);
    float yaw = atan2(x, z);
    float roll = 0;

    return radianToQuaternion(yaw, pitch, roll);
}

