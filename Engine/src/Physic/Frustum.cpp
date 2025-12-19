#include "Frustum.h"

#include "Component/TransformComponent.h"
#include "Render/Camera.h"

void Frustum::Create(const Camera* camera)
{
    const auto _far = camera->GetFar();
    const auto _near = camera->GetNear();

    const float fovY = -camera->GetFOV() * DegToRad;
    const float halfVSide = _far * tanf(fovY * .5f);
    const float halfHSide = halfVSide * camera->GetAspectRatio();
    const auto camPos = camera->GetTransform()->GetWorldPosition();
    const auto camFront = camera->GetTransform()->GetForward();
    const auto camUp = camera->GetTransform()->GetUp();
    const auto camRight = camera->GetTransform()->GetRight();
    const Vec3f frontMultFar = camFront * _far;

    planes[0] = Plane(camPos + camFront * _near, camFront);
    planes[1] = Plane(camPos + frontMultFar, -camFront);

    planes[2] = Plane(camPos, camUp.Cross(camPos + frontMultFar - camRight * halfHSide));
    planes[3] = Plane(camPos, (camPos + frontMultFar + camRight * halfHSide).Cross(camUp));

    planes[4] = Plane(camPos, (camPos + frontMultFar - camUp * halfVSide).Cross(camRight));
    planes[5] = Plane(camPos, camRight.Cross(camPos + frontMultFar + camUp * halfVSide));
}
