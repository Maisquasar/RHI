#pragma once
#include <cfloat>
#include <galaxymath/Maths.h>

#include "Frustum.h"

class Camera;
class TransformComponent;
struct Plane;
struct AABB;

struct BoundingBox
{
    BoundingBox() : min(FLT_MAX), max(-FLT_MAX) {}
    BoundingBox(Vec3f min, Vec3f max) : min(min), max(max) {}
    BoundingBox(const AABB& aabb);

    Vec3f min;
    Vec3f max;

    Vec3f GetCenter() const;
    Vec3f GetExtents() const;

    bool IsOnFrustum(const Frustum& frustum, const TransformComponent* objectTransform) const;
    bool isOnOrForwardPlane(const Plane& plane) const;
};
