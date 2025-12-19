#include "Plane.h"

Plane::Plane(const Vec3f& normal, float distance): normal(normal), distance(distance)
{
    
}

Plane::Plane(const Vec3f& _point, const Vec3f& _normal)
{
    normal = _normal.GetNormalize();
    distance = normal.Dot(_point);
}
Vec3f Plane::ProjectPointOntoPlane(const Vec3f& point) const
{
    return point - normal * GetDistanceFromPlane(point);
}

Plane Plane::PlaneFromTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
{
    Vec3f v1v0 = v1-v0;
    Vec3f v2v0 = v2-v0;

    Vec3f normal = v1v0.Cross(v2v0);
	
    normal.Normalize();
    float d = -v0.Dot(normal);
    return Plane(normal, d);
}