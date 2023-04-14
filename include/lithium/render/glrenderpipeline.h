#pragma once

#include <vector>
#include "glskinnedobject.h"
#include "glupdateable.h"
#include "glrendergroup.h"
#include "glrenderstage.h"
#include <memory>

namespace lithium
{
    class RenderPipeline
    {
    public:
        RenderPipeline(const glm::ivec2& resolution) : _resolution{resolution}
        {

        }

        std::shared_ptr<lithium::RenderGroup> createRenderGroup(const lithium::RenderGroup::ContainerType& renderables)
        {
            std::shared_ptr<lithium::RenderGroup> renderGroup = std::make_shared<RenderGroup>([](lithium::Renderable* renderable){return false;}, renderables);
            _renderGroups.push_back(renderGroup);
            return renderGroup;
        }

        std::shared_ptr<lithium::RenderGroup> createRenderGroup(const lithium::RenderGroup::FilterType& filter)
        {
            std::shared_ptr<lithium::RenderGroup> renderGroup = std::make_shared<RenderGroup>(filter);
            _renderGroups.push_back(renderGroup);
            return renderGroup;
        }

        std::shared_ptr<lithium::RenderStage> addRenderStage(std::shared_ptr<lithium::RenderStage> renderStage)
        {
            _renderStages.push_back(renderStage);
            return renderStage;
        }
        
        virtual void render()
        {
            for(auto it = _renderStages.begin(); it != _renderStages.end(); ++it)
            {
                if((*it)->enabled()){ (*it)->render(); }
            }
        }

        void setViewportToResolution()
        {
            glViewport(0, 0, _resolution.x, _resolution.y);
        }

        void addRenderable(lithium::Renderable* renderable)
        {
            for(auto renderGroup : _renderGroups)
            {
                renderGroup->filteredPushBack(renderable);
            }
        }

        void addRenderables(const std::initializer_list<lithium::Renderable*>& renderables)
        {
            for(auto renderable : renderables)
            {
                addRenderable(renderable);
            }
        }

        size_t renderableCount()
        {
            size_t count = 0;
            for(auto renderGroup : _renderGroups)
            {
                count += renderGroup->count();
            }
            return count;
        }

    protected:
        RenderPipeline* enableDepthTesting()
        {
            glEnable(GL_DEPTH_TEST);
            return this;
        }

        RenderPipeline* disableDepthTesting()
        {
            glDisable(GL_DEPTH_TEST);
            return this;
        }

        RenderPipeline* enableDepthWriting()
        {
            glDepthMask(GL_TRUE);
            return this;
        }

        RenderPipeline* disableDepthWriting()
        {
            glDepthMask(GL_FALSE);
            return this;
        }

        RenderPipeline* depthFunc(GLenum func)
        {
            glDepthFunc(func);
            return this;
        }

        RenderPipeline* enableBlending()
        {
            glEnable(GL_BLEND);
            return this;
        }

        RenderPipeline* disableBlending()
        {
            glDisable(GL_BLEND);
            return this;
        }

        RenderPipeline* blendFunc(GLenum src=GL_SRC_ALPHA, GLenum dst=GL_ONE_MINUS_SRC_ALPHA)
        {
            glBlendFunc(src, dst);
            return this;
        }

        RenderPipeline* enableFaceCulling()
        {
            glEnable(GL_CULL_FACE);
            return this;
        }

        RenderPipeline* disableCulling()
        {
            glDisable(GL_CULL_FACE);
            return this;
        }

        RenderPipeline* cullFrontFace()
        {
            glCullFace(GL_FRONT);
            return this;
        }

        RenderPipeline* cullBackFace()
        {
            glCullFace(GL_BACK);
            return this;
        }

        RenderPipeline* cullFrontAndBackFaces()
        {
            glCullFace(GL_FRONT_AND_BACK);
            return this;
        }

        RenderPipeline* enableStencilTesting()
        {
            glEnable(GL_STENCIL_TEST);
            return this;
        }

        RenderPipeline* disableStencilTesting()
        {
            glDisable(GL_STENCIL_TEST);
            return this;
        }

        RenderPipeline* enableMultisampling()
        {
            glEnable(GL_MULTISAMPLE);
            return this;
        }

        RenderPipeline* stencilFunc(GLenum func, GLint ref, GLuint mask=0xFF)
        {
            glStencilFunc(func, ref, mask);
            return this;
        }

        RenderPipeline* stencilOp(GLenum sfail=GL_KEEP, GLenum dpfail=GL_KEEP, GLenum dppass=GL_REPLACE)
        {
            glStencilOp(sfail, dpfail, dppass);
            return this;
        }

        RenderPipeline* stencilMaskSeparate(GLenum face, GLuint mask)
        {
            glStencilMaskSeparate(face, mask);
            return this;
        }

        RenderPipeline* stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
        {
            glStencilFuncSeparate(face, func, ref, mask);
            return this;
        }

        RenderPipeline* stencilOpSeparate(GLenum face, GLenum sfail=GL_KEEP, GLenum dpfail=GL_KEEP, GLenum dppass=GL_REPLACE)
        {
            glStencilOpSeparate(face, sfail, dpfail, dppass);
            return this;
        }

        RenderPipeline* stencilMask(GLuint mask)
        {
            glStencilMask(mask);
            return this;
        }

        RenderPipeline* enableStencilWriting(GLuint mask=0xFF)
        {
            return stencilMask(mask);
        }

        RenderPipeline* disableStencilWriting()
        {
            return stencilMask(0x00);
        }

        RenderPipeline* clearColor(float r=0.0f, float g=0.0f, float b=0.0f, float a=0.0f)
        {
            glClearColor(r, g, b, a);
            return this;
        }

        RenderPipeline* clearDepth(float depth=1.0f)
        {
            glClearDepth(depth);
            return this;
        }

        RenderPipeline* clearStencil(int stencil=0)
        {
            glClearStencil(stencil);
            return this;
        }

        RenderPipeline* clear(GLbitfield mask=GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
        {
            glClear(mask);
            return this;
        }

        RenderPipeline* colorMask(bool r, bool g, bool b, bool a)
        {
            glColorMask(r, g, b, a);
            return this;
        }

        RenderPipeline* colorMask(bool color=true)
        {
            glColorMask(color, color, color, color);
            return this;
        }
        
        RenderPipeline* disableColorWriting()
        {
            return colorMask(false);
        }

        RenderPipeline* enableColorWriting()
        {
            return colorMask(true);
        }

        void activeTexture(GLuint textureUnit)
        {
            if(!lithium::Texture<unsigned char>::activate(textureUnit)) // Lets not trust this so if it fail we set it anyway.
            {
                glActiveTexture(textureUnit);
            }
        }

        glm::ivec2 _resolution;
        std::vector<std::shared_ptr<lithium::RenderGroup>> _renderGroups;
        std::vector<std::shared_ptr<lithium::RenderStage>> _renderStages;
    };
}