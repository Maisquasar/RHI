#pragma once
#include <memory>
#include <unordered_map>

#include "Core/UUID.h"
#include "Core/ThreadPool.h"

#include "Utils/Type.h"

#include "Resource/IResource.h"
#include "Resource/Model.h"


class ResourceManager
{
public:
    template<typename T>
    std::shared_ptr<T> GetResource(UUID uuid)
    {
        auto it = m_resources.find(uuid);
        if (it != m_resources.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    template<typename T>
    void AddResource(std::shared_ptr<T> resource)
    {
        m_resources[GetHash(resource->GetPath())] = resource;
    }

    template<typename T>
    void RemoveResource(UUID uuid)
    {
        auto it = m_resources.find(uuid);
        if (it != m_resources.end())
        {
            m_resources.erase(it);
        }
    }

    template<typename T>
    SafePtr<T> Load(const std::filesystem::path& resourcePath)
    {
        static_assert(std::is_base_of_v<IResource, T>, "T must inherit from IResource");

        uint64_t hash = GetHash(resourcePath);

        auto it = m_resources.find(hash);
        if (it != m_resources.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        std::shared_ptr<T> resource = std::make_shared<T>(resourcePath);
        ThreadPool::Enqueue([resource]() {
            resource->Load();
        });
        AddResource(resource);
        return resource;
    }


    void Clear()
    {
        m_resources.clear();
    }
private:
    static uint64_t GetHash(const std::filesystem::path& resourcePath)
    {
        return std::filesystem::hash_value(resourcePath);
    }
private:
    std::unordered_map<uint64_t, std::shared_ptr<IResource>> m_resources;
};
