#include "ClassDescriptor.h"

Property& ClassDescriptor::AddProperty(const char* name, PropertyType type, void* data)
{
    Property property = {name, type, data };
    properties.push_back(property);
    return properties.back();
}

Property& ClassDescriptor::AddFloat(const char* name, float& value)
{
    return AddProperty(name, PropertyType::Float, &value);
}

Property& ClassDescriptor::AddInt(const char* name, int& value)
{
    return AddProperty(name, PropertyType::Int, &value);
}

Property& ClassDescriptor::AddQuat(const char* name, Quat& value)
{
    return AddProperty(name, PropertyType::Quat, &value);
}

Property& ClassDescriptor::AddVec2f(const char* name, Vec2f& value)
{
    return AddProperty(name, PropertyType::Vec2f, &value);
}

Property& ClassDescriptor::AddVec3f(const char* name, Vec3f& value)
{
    return AddProperty(name, PropertyType::Vec3f, &value);
}

Property& ClassDescriptor::AddVec4f(const char* name, Vec4f& value)
{
    return AddProperty(name, PropertyType::Vec4f, &value);
}

Property& ClassDescriptor::AddTexture(const char* name, SafePtr<Texture>& value)
{
    return AddProperty(name, PropertyType::Texture, &value);
}
