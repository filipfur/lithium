#pragma once

#include <memory>

#include "glrenderable.h"
#include "ecscomponent.h"
#include "ecsentity.h"
#include "glmesh.h"

namespace lithium
{
    class Entity : public ecs::Entity, public lithium::Renderable
    {
    public:
        using Translation = ecs::Component<glm::vec3,0>;
        using Rotation = ecs::Component<glm::quat>;
        using Scale = ecs::Component<glm::vec3,1>;
        using ModelMatrix = ecs::Component<glm::mat4,0>;

        using TexturePointer = std::shared_ptr<lithium::Texture<unsigned char>>;

        Entity(std::shared_ptr<lithium::Mesh> mesh, const std::vector<TexturePointer>& textures) : ecs::Entity{}, _mesh{mesh}, _textures{textures}
        {
            ecs::attach<lithium::Entity::Translation, lithium::Entity::Rotation, lithium::Entity::Scale, lithium::Entity::ModelMatrix>(*this);
            set<lithium::Entity::Translation>(glm::vec3{0.0f});
            set<lithium::Entity::Rotation>(glm::quat{1.0f, 0.0f, 0.0f, 0.0f});
            set<lithium::Entity::Scale>(glm::vec3{1.0f});
            set<lithium::Entity::ModelMatrix>(glm::mat4{1.0f});
        }

        virtual ~Entity() noexcept
        {

        }

        virtual void draw() const override
        {
            if(_visible)
            {
                for(int i{0}; i < _textures.size(); ++i)
                {
                    _textures[i]->bind(GL_TEXTURE0 + i);
                }
                _mesh->bind();
                _mesh->draw();
            }
        }

		virtual void shade(lithium::ShaderProgram* shaderProgram) override
        {
            if(_visible)
            {
                shaderProgram->use();
                shaderProgram->setUniform("u_color", _color);
                glm::mat4& modelMatrix = *get<lithium::Entity::ModelMatrix>();
                //std::cout << "modelMatrix: " << modelMatrix[3][0] << ", " << modelMatrix[3][1] << ", " << modelMatrix[3][2] << std::endl;
                shaderProgram->setUniform("u_model", modelMatrix);
            }
        }

        void setTextures(const std::vector<TexturePointer>& textures)
		{
			_textures = textures;
		}

        TexturePointer texture(int textureUnit) const
		{
			return _textures[textureUnit];
		}

		std::shared_ptr<lithium::Mesh> mesh() const
		{
			return _mesh;
		}

        void setPosition(const glm::vec3& position)
        {
            set<lithium::Entity::Translation>(position);
        }

        void setRotation(const glm::quat& rotation)
        {
            set<lithium::Entity::Rotation>(rotation);
        }

        void setScale(const glm::vec3& scale)
        {
            set<lithium::Entity::Scale>(scale);
        }

        glm::vec3 position() const
        {
            return *get<lithium::Entity::Translation>();
        }

        glm::quat rotation() const
        {
            return *get<lithium::Entity::Rotation>();
        }

        glm::vec3 scale() const
        {
            return *get<lithium::Entity::Scale>();
        }

        void setVisible(bool visible)
        {
            _visible = visible;
        }

        bool visible() const
        {
            return _visible;
        }

        void setColor(const glm::vec4& color)
        {
            _color = color;
        }

        void setRGB(const glm::vec3& rgb)
        {
            _color.r = rgb.r;
            _color.g = rgb.g;
            _color.b = rgb.b;
        }

        void setAlpha(float alpha)
        {
            _color.a = alpha;
        }

    private:
        std::shared_ptr<lithium::Mesh> _mesh{nullptr};
		bool _visible{true};
		glm::vec4 _color{1.0f};
        std::vector<TexturePointer> _textures;
    };
}