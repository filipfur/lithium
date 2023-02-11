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
		enum class State
		{
			VISIBLE,
			HIDDEN
		};

    public:

		using TexturePointer = std::shared_ptr<lithium::Texture<unsigned char>>;

		Object(std::shared_ptr<lithium::Mesh> mesh, const std::vector<TexturePointer>& textures);
        Object(const Object& other);
        virtual ~Object() noexcept;

        virtual void draw() const override;

		virtual void shade(lithium::ShaderProgram* shaderProgram) override;

        void setTextures(const std::vector<TexturePointer>& textures)
		{
			_textures = textures;
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

		glm::vec3* positionPtr()
		{
			return &_position;
		}

		lithium::Object* setRotation(const glm::vec3& rotation)
		{
			_rotation = rotation;
			_modelInvalidated = true;
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
			return *_objectName;
		}

		void setMesh(std::shared_ptr<lithium::Mesh> mesh)
		{
			_mesh = mesh;
		}

		virtual lithium::Object* setVisible(bool visible)
		{
			_visible = visible;
			return this;
		}

		bool visible() const
		{
			return _visible;
		}

		lithium::Object* setOpacity(float opacity)
		{
			_color.a = opacity;
			return this;
		}
		
		void fadeOpacity(float fadeTime, float fadeFrom, float fadeTo, const std::function<void()>& callback)
		{
			assert(_opacityFader == nullptr);
			_opacityFader = new lithium::Fader(fadeTime, fadeFrom, fadeTo, [this, callback](){
				callback();
				_opacityFader = nullptr;
			});
		}

		void fadeOut(float fadeTime)
		{
			fadeOpacity(fadeTime, opacity(), 0.0f, [this](){
				setVisible(false);
			});
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

		void setTextureRegions(const glm::vec2& textureRegions)
		{
			_textureRegions = textureRegions;
		}

		void setCurrentTextureRegion(const glm::vec2& currentTextureRegion)
		{
			_currentTextureRegion = currentTextureRegion;
		}

		void setCurrentTextureRegion(float currentTextureRegionX, float currentTextureRegionY=0.0f)
		{
			_currentTextureRegion.x = currentTextureRegionX;
			_currentTextureRegion.y = currentTextureRegionY;
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

		std::shared_ptr<lithium::Mesh> mesh() const
		{
			return _mesh;
		}

		glm::vec2 textureRegions() const
		{
			return _textureRegions;
		}

		glm::vec2 currentTextureRegion() const
		{
			return _currentTextureRegion;
		}

		float opacity() const
		{
			return _color.a;
		}

		float shininess() const
		{
			return _shininess;
		}

		void setShininess(float shininess)
		{
			_shininess = shininess;
		}

		void setModelInvalidated(bool modelInvalidated)
		{
			_modelInvalidated = modelInvalidated;
		}

		bool modelInvalidated() const
		{
			return _modelInvalidated;
		}

		void setDepthTest(bool depthTest)
		{
			_depthTest = depthTest;
		}

		glm::vec4 fadedColor() const
		{
			glm::vec4 color{_color};
			if(_opacityFader)
			{
				color.a = _opacityFader->value();
			}
			return color;
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

	protected:
        std::shared_ptr<lithium::Mesh> _mesh{nullptr};

		bool _depthTest{true};
		glm::vec3 _position{0.0f};
		glm::vec3 _rotation{0.0f};
		glm::vec3 _scale{1.0f};
		glm::mat4 _model{1.0f};
		float _shininess{32.0f};
		bool _visible{true};
		glm::vec4 _color{1.0f};
        std::vector<TexturePointer> _textures;
		
	private:
		std::shared_ptr<std::string> _objectName;

		glm::vec2 _textureRegions{1.0f, 1.0f};
		glm::vec2 _currentTextureRegion{0.0f, 0.0f};

		lithium::Fader* _opacityFader{nullptr};
		bool _modelInvalidated{false};
    };
}