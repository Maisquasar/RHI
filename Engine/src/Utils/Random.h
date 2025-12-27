#pragma once
#include <galaxymath/Maths.h>

namespace Random
{
    float Float(float min, float max);
    int Int(int min, int max);
    Vec3f Vector3(float min, float max);
    
    Vec3f PointOnSphere(float radius = 1.f);
};

