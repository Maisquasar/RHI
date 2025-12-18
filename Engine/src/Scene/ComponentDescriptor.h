#pragma once
#include <string>
#include <vector>

#include <galaxymath/Maths.h>

#include "Utils/Type.h"

class IResource;

enum class PropertyType
{
    None,
    Bool,
    Float, 
    Vec2f,
    Vec3f,
    Vec4f,
    Quat,
    Color3,
    Color4,
    Mesh,
    Materials,
    Transform
};

struct Property
{
    std::string name; 
    PropertyType type;    
    void* data;
};

struct ComponentDescriptor
{
    std::vector<Property> properties;
    
    void AddProperty(const char* name, PropertyType type, void* data);
    void AddFloat(const char* name, float& value);
    void AddQuat(const char* name, Quat& value);
    void AddVec3f(const char* name, Vec3f& value);
    void AddVec4f(const char* name, Vec4f& value);
};