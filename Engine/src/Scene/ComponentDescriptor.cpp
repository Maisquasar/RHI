#include "ComponentDescriptor.h"

void ComponentDescriptor::AddProperty(const char* name, PropertyType type, void* data)
{
    properties.push_back({ name, type, data });
}

void ComponentDescriptor::AddFloat(const char* name, float& value)
{
    AddProperty(name, PropertyType::Float, &value);
}

void ComponentDescriptor::AddQuat(const char* name, Quat& value)
{
    AddProperty(name, PropertyType::Quat, &value);
}

void ComponentDescriptor::AddVec3f(const char* name, Vec3f& value)
{
    AddProperty(name, PropertyType::Vec3f, &value);
}

void ComponentDescriptor::AddVec4f(const char* name, Vec4f& value)
{
    AddProperty(name, PropertyType::Vec4f, &value);
}