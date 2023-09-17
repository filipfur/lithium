#pragma once

#include <map>
#include "globject.h"
#include "gltfloader.h"
#include "glfilesystem.h"

namespace lithium
{

    class AssetLibrary
    {
    public:
        static void loadGLTF(const std::filesystem::path& assetDir);

        std::map<std::string, std::shared_ptr<lithium::Object>>::iterator begin() { return _objects.begin(); }
        std::map<std::string, std::shared_ptr<lithium::Object>>::iterator end() { return _objects.end(); }

        size_t size() const { return _objects.size(); }

        static AssetLibrary& getInstance();

    private:
        std::map<std::string, std::shared_ptr<lithium::Object>> _objects;
        gltf::Loader _gltfLoader;
    };

}