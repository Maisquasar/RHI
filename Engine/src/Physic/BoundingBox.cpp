#include "BoundingBox.h"

#include "AABB.h"
#include "Plane.h"
#include "Component/TransformComponent.h"
#include "Render/Camera.h"


BoundingBox::BoundingBox(const AABB& aabb)
{
    min = aabb.Min;
    max = aabb.Max;
}

Vec3f BoundingBox::GetCenter() const
{
    return (min + max) * 0.5f;
}

Vec3f BoundingBox::GetExtents() const
{
    return max - GetCenter();
}

bool BoundingBox::IsOnFrustum(Camera* camera, const TransformComponent* objectTransform) const
{
    Frustum frustum = camera->GetFrustum();

    Vec3f position = objectTransform->GetWorldPosition();
    Quat rotation = objectTransform->GetWorldRotation();
    Vec3f scale = objectTransform->GetWorldScale();

    Vec3f localCenter = (min + max) * 0.5f;
    Vec3f localExtents = (max - min) * 0.5f;

    Vec3f worldCenter = position + rotation * (localCenter * scale);

    Vec3f worldExtents(0.0f);
    for (int i = 0; i < 8; i++)
    {
        Vec3f corner;
        corner.x = (i & 0x1) ? localExtents.x : -localExtents.x;
        corner.y = (i & 0x2) ? localExtents.y : -localExtents.y;
        corner.z = (i & 0x4) ? localExtents.z : -localExtents.z;

        corner = rotation * (corner * scale);

        for (int j = 0; j < 3; j++)
        {
            worldExtents[j] = fmaxf(worldExtents[j], fabsf(corner[j]));
        }
    }

    BoundingBox globalAABB = AABB(worldCenter, worldExtents, true);
    
    for (const auto& plane : frustum.planes)
    {
        if (!globalAABB.isOnOrForwardPlane(plane))
        {
            return false;
        }
    }
    return true;
}


bool BoundingBox::isOnOrForwardPlane(const Plane& plane) const
{
    const auto center = GetCenter();
    const auto extents = GetExtents();

    const float r = 
          extents.x * std::abs(plane.normal.x) 
        + extents.y * std::abs(plane.normal.y) 
        + extents.z * std::abs(plane.normal.z);

    const float distance = plane.GetDistanceToPlane(center);

    return -r <= distance;
}