#include "glfilesystem.h"

void lithium::fs::forEachInDirectory(const std::filesystem::path& directory,
        std::function<void(const std::filesystem::path&)> callback,
        bool recursive)
{
    for(auto& p : std::filesystem::directory_iterator(directory))
    {
        if(p.is_directory())
        {
            callback(p.path());
            if(recursive)
            {
                forEachInDirectory(p.path(), callback);
            }
        }
        else
        {
            callback(p.path());
        }
    }
}