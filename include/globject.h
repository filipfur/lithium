#pragma once

#include "glmesh.h"
#include "glcamera.h"
#include "gltexture.h"
#include "iobject.h"
#include <glm/gtc/type_ptr.hpp>
#include "gltaskscheduler.h"
#include "glfader.h"

namespace lithium
{
    class Object
    {
		enum class State
		{
			VISIBLE,
			HIDDEN
		};

    public:
        Object(lithium::Mesh* mesh, lithium::Texture* texture, lithium::Texture* normal=nullptr);
        Object(const Object& other);
        virtual ~Object() noexcept;

        virtual void draw();

		virtual void shade(lithium::ShaderProgram* shaderProgram) const;

        void setTexture(lithium::Texture* texture)
		{
			_texture = texture;
		}
		
		void setSpecular(lithium::Texture* specular)
		{
			_specular = specular;
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

		void setMesh(lithium::Mesh* mesh)
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

		virtual void update(float dt);

		lithium::Object* setFlicker(float flicker)
		{
			_flicker = flicker;
			return this;
		}

		void setIObject(IObject* iObject)
		{
			_iObject = iObject;
		}

		lithium::Object* setOpacity(float opacity)
		{
			setColor(glm::vec4{_color.r, _color.g, _color.b, opacity});
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

		void setNormalMap(lithium::Texture* normalMap)
		{
			_normalMap = normalMap;
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

		lithium::Texture* texture() const
		{
			return _texture;
		}

		lithium::Mesh* mesh() const
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

	protected:
		virtual void onDraw();
        lithium::Mesh* _mesh;

		bool _depthTest{true};
		virtual void updateModel();
		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;
		glm::mat4 _model;
		float _shininess{32.0f};
		float _flicker{0.0f};
		bool _visible{true};
		float _time{0.0f};
		glm::vec4 _color{1.0f};
        lithium::Texture* _texture{nullptr};
        lithium::Texture* _specular{nullptr};
		lithium::Texture* _normalMap{nullptr};
		
	private:
		std::shared_ptr<std::string> _objectName;

		glm::vec2 _textureRegions{1.0f, 1.0f};
		glm::vec2 _currentTextureRegion{0.0f, 0.0f};

		IObject* _iObject;
		lithium::Fader* _opacityFader{nullptr};
		bool _modelInvalidated{false};
    };
}