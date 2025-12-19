#pragma once
#include <galaxymath/Maths.h>

struct Plane
{
    Vec3f normal = Vec3f::Up();
    float distance = 0.f;

    Plane() = default;
    Plane(const Vec3f& normal, float distance);
    Plane(const Vec3f& _point, const Vec3f& _normal);
    float GetDistanceToPlane(const Vec3f& _point) const { return normal.Dot(_point) - distance; }
    float GetDistanceFromPlane(const Vec3f& _point) const { return _point.Dot(normal) + distance; }
    Vec3f ProjectPointOntoPlane(const Vec3f& point) const;

    static Plane PlaneFromTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2);
};
