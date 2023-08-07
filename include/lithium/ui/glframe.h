#pragma once

#include "globject.h"
#include "glrenderpipeline.h"
#include "glframerenderer.h"
#include "glframelayout.h"

namespace lithium
{
    class Frame : public Object, public FrameRenderer, public IFrameLayout
    {
    public:
        Frame(FrameLayout* frameLayout);

        virtual ~Frame() noexcept;

        static std::shared_ptr<ShaderProgram> shaderProgram();

        /*void addFrame(const std::shared_ptr<Frame>& frame)
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
        }*/

        void forEachChild(std::function<void(Frame*)> callback) const
        {
            _frameLayout->forEachChild([callback](lithium::FrameLayout& child){
                callback(dynamic_cast<lithium::Frame*>(child.iFrameLayout()));
            });
        }

        virtual void onLayoutChanged(FrameLayout* frameLayout) override
        {
            _frameLayout = frameLayout;
        }

        virtual void onLayoutUpdated(const glm::vec2& position, const glm::vec2& dimension) override
        {
            _changed = true;
        }

        bool hasChildren()
        {
            return _frameLayout->count() > 0;
        }

        Frame* parent()
        {
            if(_frameLayout->parent() == nullptr)
            {
                return nullptr;
            }
            return dynamic_cast<lithium::Frame*>(_frameLayout->parent()->iFrameLayout());
        }

        Frame* frameById(const std::string& id);

        bool renderFrames();

        void refresh()
        {
            _changed = true;
        }

        lithium::FrameLayout* layout()
        {
            return _frameLayout;
        }

        size_t childCount()
        {
            return _frameLayout->count();
        }

        Frame* child(size_t index)
        {
            return dynamic_cast<Frame*>(_frameLayout->child(index)->iFrameLayout());
        }

    private:
        FrameLayout* _frameLayout;
        bool _changed{true};
    };
}