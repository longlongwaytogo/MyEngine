#pragma once

#include <gfx/core/types.h>
#include <string>
#include <functional>

namespace gfx {

class Window {
public:
    struct Desc {
        std::string title = "MyEngine";
        Extent2D extent = {800, 600};
        bool resizable = true;
        bool fullscreen = false;
        bool vsync = true;
    };
    
    virtual ~Window() = default;
    
    virtual void* getNativeHandle() const = 0;
    
    virtual void resize(const Extent2D& extent) = 0;
    
    virtual Extent2D getExtent() const = 0;
    
    virtual bool shouldClose() const = 0;
    
    virtual void pollEvents() = 0;
    
    virtual void swapBuffers() = 0;
    
    virtual void setTitle(const std::string& title) = 0;
    
    virtual void setVsync(bool enabled) = 0;
    
    virtual bool isVsync() const = 0;
    
    using ResizeCallback = std::function<void(Extent2D)>;
    using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
    using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
    using CursorPosCallback = std::function<void(double x, double y)>;
    using ScrollCallback = std::function<void(double xoffset, double yoffset)>;
    
    virtual void setResizeCallback(ResizeCallback callback) = 0;
    virtual void setKeyCallback(KeyCallback callback) = 0;
    virtual void setMouseButtonCallback(MouseButtonCallback callback) = 0;
    virtual void setCursorPosCallback(CursorPosCallback callback) = 0;
    virtual void setScrollCallback(ScrollCallback callback) = 0;
    
    static std::unique_ptr<Window> create(const Desc& desc);
};

}
