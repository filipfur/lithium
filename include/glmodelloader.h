
#pragma once

#include <filesystem>

#define filename(path) path.substr(path.find_last_of('/'), std::string::npos);

namespace lithium
{
    class ModelLoader
    {
    public:
        ModelLoader(const std::string& modelDirectory)
        {
            for(const auto& entry : fs::directory_iterator(path))
            {
                //entry.path()
            }
        }

        lithium::Model* getModel(const std::string& modelName)
        {
            lithium::Model* model{nullptr};
            auto it = _models.find(modelName);
            if(it != _models.end())
            {
                model = it->second;
            }
            return model;
        }
        
        virtual ~ModelLoader() noexcept
        {

        }

    private:

        std::map<std::string, lithium::Model*> _models;

    };
}