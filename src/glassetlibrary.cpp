#include "glassetlibrary.h"

void lithium::AssetLibrary::loadGLTF(const std::filesystem::path& assetDir)
{
    lithium::fs::forEachInDirectory(assetDir, [](const std::filesystem::path& p) {
        AssetLibrary& lib = AssetLibrary::getInstance();
        if (p.extension() == ".gltf")
        {
            std::vector<std::shared_ptr<lithium::Object>> objects;
            lib._gltfLoader.loadObjects(objects, p);
            std::for_each(objects.begin(), objects.end(), [&lib](std::shared_ptr<lithium::Object> o) {
                if(!lib._objects.emplace(o->objectName(), o).second)
                {
                    std::cerr << "WARNING: Object with name " << o->objectName() << " already exists in the asset library." << std::endl;
                }
            });
        }
    }, true);
}

lithium::AssetLibrary& lithium::AssetLibrary::getInstance()
{
    static AssetLibrary instance;
    return instance;
}