#pragma once
#include <filesystem>
#include <string>

#include "Core/UUID.h"

class IResource
{
public:
    IResource(std::filesystem::path path) : p_path(std::move(path)) {}
    IResource(const IResource&) = delete;
    IResource(IResource&&) = delete;
    IResource& operator=(const IResource&) = delete;
    virtual ~IResource() = default;

    virtual void Load() = 0;
    virtual void SendToGPU() = 0;
    virtual void Unload() = 0;

    UUID GetUUID() const { return p_uuid; }
    std::filesystem::path GetPath() const { return p_path; }
    bool IsLoaded() const { return p_isLoaded; }
    bool SentToGPU() const { return p_sendToGPU; }
protected:    
    std::filesystem::path p_path;
    UUID p_uuid;

    std::atomic_bool p_isLoaded;
    std::atomic_bool p_sendToGPU;
};
