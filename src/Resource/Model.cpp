#include "Model.h"

#include "Loader/OBJLoader.h"

void Model::Load()
{
    if (p_path.extension() == ".obj")
    {
        OBJLoader::Model model;
        if (!OBJLoader::Load(p_path, model))
        {
            throw std::runtime_error("Failed to load model");
        }
    }
    else
    {
        throw std::runtime_error("Unsupported file format" + p_path.extension().generic_string());
    }
}

void Model::SendToGPU()
{
    
}

void Model::Unload()
{
}
