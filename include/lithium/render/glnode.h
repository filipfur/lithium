#pragma once

#include <string>
#include <set>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <map>
#include "gltrs.h"

namespace lithium
{
    class Node : public TRS
    {
    public:
        Node(const int id, const std::string& name, const glm::vec3& position,
            const glm::quat& rotation, const glm::vec3 scale);
        virtual ~Node() noexcept;

        void setParent(Node* parent);
        void updateWorldMatrix(glm::mat4 parentWorldMatrix, bool doStuff=true);

        std::string name() const
        {
            return _name;
        }

        lithium::Node* parent() const
        {
            return _parent;
        }

        glm::mat4 worldMatrix() const
        {
            return _worldMatrix;
        }

        void forAllChildren(const std::function<void(lithium::Node*)>& callback)
        {
            for(lithium::Node* child : _children)
            {
                callback(child);
            }
        }

        void setLocalMatrix(const glm::mat4 matrix)
        {
            _model = matrix;
            _modelInvalidated = false;
        }

        int id() const
        {
            return _id;
        }

        void setMeshId(int meshId)
        {
            _meshId = meshId;
        }

        int meshId() const
        {
            return _meshId;
        }

        void addPropertyF(const std::string& key, float f)
        {
            _propertiesF.emplace(key, f);
        }

        void addPropertyArrayF(const std::string& key, const std::vector<float>& f)
        {
            _propertiesArrayF.emplace(key, f);
        }

        bool hasProperties() const
        {
            return _propertiesF.size() || _propertiesArrayF.size();
        }

        void forEachPropertyF(const std::function<void(const std::string&, float)>& callback)
        {
            for(auto entry : _propertiesF)
            {
                callback(entry.first, entry.second);
            }
        }

        bool hasPropertyF(const std::string& key) const
        {
            return _propertiesF.find(key) != _propertiesF.end();
        }

        bool hasPropertyArrayF(const std::string& key) const
        {
            return _propertiesArrayF.find(key) != _propertiesArrayF.end();
        }

        float propertyF(const std::string& key) const
        {
            return _propertiesF.find(key)->second;
        }

        int propertyArrayFSize(const std::string& key) const
        {
            return _propertiesArrayF.find(key)->second.size();
        }

        float propertyArrayF(const std::string& key, int index) const
        {
            return _propertiesArrayF.find(key)->second[index];
        }

        const std::vector<float>& propertyArrayF(const std::string& key) const
        {
            return _propertiesArrayF.find(key)->second;
        }

    private:
        void addChild(Node* child);
        void removeChild(Node* child);

        const int _id;
        const std::string _name;
        glm::mat4 _worldMatrix{1.0f};
        Node* _parent{nullptr};
        std::set<Node*> _children;
        int _meshId{-1};
        std::map<std::string, float> _propertiesF;
        std::map<std::string, std::vector<float>> _propertiesArrayF;
    };
}