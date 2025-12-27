#include "Random.h"
#include <random>
#include <cmath>
#include <SDL_stdinc.h>

static std::mt19937& GetGenerator()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

float Random::Float(float min, float max)
{
    std::uniform_real_distribution<float> dis(min, max);
    return dis(GetGenerator());
}

int Random::Int(int min, int max)
{
    std::uniform_int_distribution<int> dis(min, max);
    return dis(GetGenerator());
}

Vec3f Random::Vector3(float min, float max)
{
    return Vec3f(Float(min, max), Float(min, max), Float(min, max));
}

Vec3f Random::PointOnSphere(float radius)
{
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    float theta = dist(GetGenerator()) * 2.f * M_PI;   // azimuthal angle
    float phi   = acos(1.f - 2.f * dist(GetGenerator())); // polar angle

    float x = sin(phi) * cos(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(phi);

    return Vec3f(x, y, z) * radius;
}
