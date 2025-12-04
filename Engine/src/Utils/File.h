#pragma once
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

class File
{
public:
    File(std::filesystem::path path) : m_path(std::move(path)) { }
    File(const std::string& path) : File(std::filesystem::path(path)) {}
    
    static bool ReadAllBytes(const std::filesystem::path& path, std::vector<uint8_t>& out);
    static bool ReadAllLines(const std::filesystem::path& path, std::vector<std::string>& out);
    static bool ReadAllText(const std::filesystem::path& path, std::string& out);
    static bool Exist(const std::filesystem::path& path);
    
    bool ReadAllBytes(std::vector<uint8_t>& out) const;
    bool ReadAllLines(std::vector<std::string>& out) const;
    bool ReadAllText(std::string& out) const;
    bool Exist() const;
    
private:
    std::filesystem::path m_path;
    
};
