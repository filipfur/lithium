#include "glassetlibrary.h"
#include "gltimemeasure.h"

void lithium::AssetLibrary::loadGLTF(const std::filesystem::path& assetDir)
{
    lithium::fs::forEachInDirectory(assetDir, [this](const std::filesystem::path& p) {
        if (p.extension() == ".gltf")
        {
            lithium::TimeMeasure::Handle handle{lithium::TimeMeasure::start("Loading " + p.filename().string())};
            std::vector<std::shared_ptr<lithium::Object>> objects;
            _gltfLoader.loadObjects(objects, p);
            std::for_each(objects.begin(), objects.end(), [this](std::shared_ptr<lithium::Object> o) {
                if(!_objects.emplace(o->objectName(), o).second)
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

std::shared_ptr<lithium::Object> lithium::AssetLibrary::object(const std::string& name)
{
    auto it = _objects.find(name);
    if(it != _objects.end())
    {
        return it->second;
    }
    return nullptr;
}