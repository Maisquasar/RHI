#pragma once
#include <galaxymath/Maths.h>

#include "Plane.h"


class Camera;
struct CameraData;

struct Frustum
{
    Frustum() = default;

    Plane planes[6];

    void Create(const Camera* camera);
};
