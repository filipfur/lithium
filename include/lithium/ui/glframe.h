#pragma once

#include "globject.h"
#include "glrenderpipeline.h"
#include "glframerenderer.h"

namespace lithium
{
    class Frame : public Object, public FrameRenderer
    {
    public:
        Frame(Frame* parent, const glm::vec2& dimension);

        virtual ~Frame() noexcept;

        static std::shared_ptr<ShaderProgram> shaderProgram();

        void addFrame(const std::shared_ptr<Frame>& frame)
        {
            _children.push_back(frame);
            FrameRenderer::attach(frame.get());
            frame->stage();
        }

        std::shared_ptr<Frame> createFrame(const glm::vec2& dimension)
        {
            auto frame = std::make_shared<Frame>(this, dimension);
            addFrame(frame);
            return frame;
        }

        void forEachChild(std::function<void(Frame*)> callback) const
        {
            for(const auto& child : _children)
            {
                callback(child.get());
            }
        }

        bool hasChildren()
        {
            return _children.size() > 0;
        }

        bool renderFrames();

        void refresh()
        {
            _changed = true;
        }

    private:
        Frame* _parent;
        std::vector<std::shared_ptr<Frame>> _children;
        glm::vec2 _padding;
        glm::vec2 _margin;
        bool _changed{true};
    };
}