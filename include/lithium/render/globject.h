#pragma once

#include <memory>
#include "glmesh.h"
#include "glcamera.h"
#include "glimagetexture.h"
#include <glm/gtc/type_ptr.hpp>
#include "gltaskscheduler.h"
#include "glfader.h"
#include "glupdateable.h"
#include "glrenderable.h"

namespace lithium
{
    class Object : public Updateable, public Renderable
    {
		enum class RotationType
		{
			XYZ,
			QUATERNION
		};
    public:
		using TexturePointer = std::shared_ptr<lithium::Texture<unsigned char>>;

		Object(std::shared_ptr<lithium::Mesh> mesh, const std::vector<TexturePointer>& textures);
        Object(const Object& other);
        virtual ~Object() noexcept;

        virtual void draw() const override;

		virtual void shade(lithium::ShaderProgram* shaderProgram) override;

        lithium::Object* setTextures(const std::vector<TexturePointer>& textures)
		{
			_textures = textures;
			return this;
		}

		lithium::Object* setPosition(const glm::vec3& position)
		{
			_position = position;
			_modelInvalidated = true;
			return this;
		}

		lithium::Object* setPosition(float x, float y, float z)
		{
			return setPosition(glm::vec3{x, y, z});
		}

		void move(const glm::vec3& d)
		{
			_position.x += d.x;
			_position.y += d.y;
			_position.z += d.z;
			_modelInvalidated = true;
		}

		void move(const glm::vec2& d)
		{
			_position.x += d.x;
			_position.y += d.y;
			_modelInvalidated = true;
		}

		const glm::vec3& position() const
		{
			return _position;
		}

		lithium::Object* setRotation(const glm::vec3& rotation)
		{
			_rotation = rotation;
			_modelInvalidated = true;
			_rotationType = RotationType::XYZ;
			return this;
		}

		lithium::Object* setQuaternion(const glm::quat& quaternion)
		{
			_quaternion = quaternion;
			_modelInvalidated = true;
			_rotationType = RotationType::QUATERNION;
			return this;
		}

		const glm::vec3& rotation() const
		{
			return _rotation;
		}

		lithium::Object* setScale(const glm::vec3& scale)
		{
			_scale = scale;
			_modelInvalidated = true;
			return this;
		}

		lithium::Object* setScale(float scale)
		{
			return setScale(glm::vec3{scale, scale, scale});
		}

		const glm::vec3 scale() const
		{
			return _scale;
		}

		void setObjectName(const std::string& str)
		{
			_objectName = std::make_shared<std::string>(str);
		}

		std::string objectName() const
		{
			if(_objectName)
			{
				return *_objectName;
			}
			return "";
		}

		void setMesh(std::shared_ptr<lithium::Mesh> mesh)
		{
			_mesh = mesh;
		}

		lithium::Object* setOpacity(float opacity)
		{
			_color.a = opacity;
			return this;
		}

		lithium::Object* setColor(const glm::vec3 color)
		{
			setColor(glm::vec4{color, _color.a});
			return this;
		}

		lithium::Object* setColor(const glm::vec4 color)
		{
			_color = color;
			return this;
		}

		glm::vec4 color() const
		{
			return _color;
		}

		glm::vec3 rgb() const
		{
			return glm::vec3{_color.x, _color.y, _color.z};
		}

		void copyTranslation(lithium::Object* other)
		{
			for(int y{0}; y < 4; ++y)
			{
				this->_model[y][3] = other->_model[y][3];
				this->_model[3][y] = other->_model[3][y];
			}
		}

		void copyRotation(lithium::Object* other)
		{
			for(int y{0}; y < 3; ++y)
			{
				for(int x{0}; x < 3; ++x)
				{
					this->_model[y][x] = other->_model[y][x];
				}
			}
			this->_modelInvalidated = false;
		}

		void copyModelMatrix(lithium::Object* other)
		{
			for(int y{0}; y < 4; ++y)
			{
				for(int x{0}; x < 4; ++x)
				{
					this->_model[y][x] = other->_model[y][x];
				}
			}
			this->_modelInvalidated = false;
		}

		TexturePointer texture(int textureUnit) const
		{
			return _textures[textureUnit];
		}

		bool hasTexture() const
		{
			return _textures.size() > 0;
		}

		std::shared_ptr<lithium::Mesh> mesh() const
		{
			return _mesh;
		}

		float opacity() const
		{
			return _color.a;
		}

		void setModelInvalidated(bool modelInvalidated)
		{
			_modelInvalidated = modelInvalidated;
		}

		void setModel(const glm::mat4& model)
		{
			_model = model;
			_modelInvalidated = false;
		}

		bool modelInvalidated() const
		{
			return _modelInvalidated;
		}

		virtual lithium::Object* clone() const
		{
			return new lithium::Object(*this);
		}

		glm::mat4 model() const
		{
			return _model;
		}

		virtual void updateModel();

		void setParent(lithium::Object* parent)
		{
			_parent = parent;
		}

		lithium::Object* parent() const
		{
			return _parent;
		}

		void addChild(std::shared_ptr<lithium::Object> child)
		{
			_children.emplace_back(child);
		}

		void removeChild(lithium::Object* child)
		{
			auto it = _children.begin();
			while(it != _children.end())
			{
				if(it->get() == child)
				{
					it = _children.erase(it);
					break;
				}
				++it;
			}
		}

		void forEachChild(const std::function<void(lithium::Object*)>& callback)
		{
			for(auto& child : _children)
			{
				callback(child.get());
			}
		}

	protected:
        std::shared_ptr<lithium::Mesh> _mesh{nullptr};
		glm::vec3 _position{0.0f};
		glm::vec3 _rotation{0.0f};
		glm::quat _quaternion{1.0f, 0.0f, 0.0f, 0.0f};
		glm::vec3 _scale{1.0f};
		glm::mat4 _model{1.0f};
		glm::vec4 _color{1.0f};
        std::vector<TexturePointer> _textures;
		std::shared_ptr<std::string> _objectName;
		lithium::Object* _parent{nullptr};
		std::vector<std::shared_ptr<lithium::Object>> _children;

	private:
		bool _modelInvalidated{false};
		RotationType _rotationType{RotationType::XYZ};
    };
}