#pragma once

#include <memory>

#include "glrenderable.h"
#include "ecsentity.h"
#include "glmesh.h"

namespace lithium
{
    class Entity : public ecs::Entity, public lithium::Renderable
    {
    public:
        using TexturePointer = std::shared_ptr<lithium::Texture<unsigned char>>;

        Entity(std::shared_ptr<lithium::Mesh> mesh, const std::vector<TexturePointer>& textures) : ecs::Entity{}
        {

        }

        virtual ~Entity{} noexcept
        {

        }

        virtual void draw() const override
        {
            if(_visible)
            {
                _mesh->draw();
            }
        }

		virtual void shade(lithium::ShaderProgram* shaderProgram) override
        {

        }

        void setTextures(const std::vector<TexturePointer>& textures)
		{
			_textures = textures;
		}

    private:
        std::shared_ptr<lithium::Mesh> _mesh{nullptr};
		bool _visible{true};
		glm::vec4 _color{1.0f};
        std::vector<TexturePointer> _textures;
    }
}