#pragma once

#include <fstream>
#include "glframe.h"
#include "gllayoutsystem.h"
#include "gljson.h"

namespace lithium
{
    class Canvas : public lithium::Updateable
    {
    public:
        Canvas(const glm::vec2& resolution);

        virtual ~Canvas() noexcept;

        void setFrame(std::shared_ptr<lithium::Frame> frame);

        void move(const glm::vec2& delta);

        void loadUI(const std::string& path);

        // This creates frame recursively
        std::shared_ptr<lithium::Frame> addFrame(FrameLayout* frameLayout);

        virtual void update(float dt) override;

        Frame* frameById(const std::string& id);
        
        void render();

        void refreshUI()
        {
            _layoutSystem.updateLayouts();
        }

    private:

        std::shared_ptr<lithium::Frame> _frame;
        std::vector<std::shared_ptr<lithium::Frame>> _frames;
        glm::vec2 _resolution;
        SimpleCamera _camera;
        UniformBufferObject _canvasUBO;
        std::shared_ptr<Mesh> _frameMesh;
        glm::vec3 _lookTarget{0.0f};
        glm::vec3 _lookPosition{0.0f};
        LayoutSystem _layoutSystem;
    };
}