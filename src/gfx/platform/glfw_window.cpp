#include <gfx/platform/Window.h>
#include <GLFW/glfw3.h>
#include <cassert>

namespace gfx {

class GLFWWindow : public Window {
public:
    GLFWWindow(const Desc& desc);
    ~GLFWWindow() override;
    
    void* getNativeHandle() const override;
    void resize(const Extent2D& extent) override;
    Extent2D getExtent() const override;
    bool shouldClose() const override;
    void pollEvents() override;
    void swapBuffers() override;
    void setTitle(const std::string& title) override;
    void setVsync(bool enabled) override;
    bool isVsync() const override;
    
    void setResizeCallback(ResizeCallback callback) override;
    void setKeyCallback(KeyCallback callback) override;
    void setMouseButtonCallback(MouseButtonCallback callback) override;
    void setCursorPosCallback(CursorPosCallback callback) override;
    void setScrollCallback(ScrollCallback callback) override;
    
private:
    GLFWwindow* m_window = nullptr;
    Extent2D m_extent;
    bool m_vsync;
    
    ResizeCallback m_resizeCallback;
    KeyCallback m_keyCallback;
    MouseButtonCallback m_mouseButtonCallback;
    CursorPosCallback m_cursorPosCallback;
    ScrollCallback m_scrollCallback;
    
    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void glfwCursorPosCallback(GLFWwindow* window, double x, double y);
    static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

void GLFWWindow::glfwResizeCallback(GLFWwindow* window, int width, int height) {
    void* userData = glfwGetWindowUserPointer(window);
    if (userData) {
        GLFWWindow* glfwWindow = static_cast<GLFWWindow*>(userData);
        glfwWindow->m_extent = {static_cast<u32>(width), static_cast<u32>(height)};
        if (glfwWindow->m_resizeCallback) {
            glfwWindow->m_resizeCallback(glfwWindow->m_extent);
        }
    }
}

void GLFWWindow::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    void* userData = glfwGetWindowUserPointer(window);
    if (userData) {
        GLFWWindow* glfwWindow = static_cast<GLFWWindow*>(userData);
        if (glfwWindow->m_keyCallback) {
            glfwWindow->m_keyCallback(key, scancode, action, mods);
        }
    }
}

void GLFWWindow::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    void* userData = glfwGetWindowUserPointer(window);
    if (userData) {
        GLFWWindow* glfwWindow = static_cast<GLFWWindow*>(userData);
        if (glfwWindow->m_mouseButtonCallback) {
            glfwWindow->m_mouseButtonCallback(button, action, mods);
        }
    }
}

void GLFWWindow::glfwCursorPosCallback(GLFWwindow* window, double x, double y) {
    void* userData = glfwGetWindowUserPointer(window);
    if (userData) {
        GLFWWindow* glfwWindow = static_cast<GLFWWindow*>(userData);
        if (glfwWindow->m_cursorPosCallback) {
            glfwWindow->m_cursorPosCallback(x, y);
        }
    }
}

void GLFWWindow::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    void* userData = glfwGetWindowUserPointer(window);
    if (userData) {
        GLFWWindow* glfwWindow = static_cast<GLFWWindow*>(userData);
        if (glfwWindow->m_scrollCallback) {
            glfwWindow->m_scrollCallback(xoffset, yoffset);
        }
    }
}

GLFWWindow::GLFWWindow(const Desc& desc) : m_extent(desc.extent), m_vsync(desc.vsync) {
    if (!glfwInit()) {
        assert(false && "Failed to initialize GLFW");
        return;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
    
    m_window = glfwCreateWindow(
        static_cast<int>(desc.extent.width),
        static_cast<int>(desc.extent.height),
        desc.title.c_str(),
        desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );
    
    if (!m_window) {
        glfwTerminate();
        assert(false && "Failed to create GLFW window");
        return;
    }
    
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(desc.vsync ? 1 : 0);
    
    glfwSetWindowUserPointer(m_window, this);
    
    glfwSetFramebufferSizeCallback(m_window, glfwResizeCallback);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(m_window, glfwCursorPosCallback);
    glfwSetScrollCallback(m_window, glfwScrollCallback);
}

GLFWWindow::~GLFWWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}

void* GLFWWindow::getNativeHandle() const {
    return m_window;
}

void GLFWWindow::resize(const Extent2D& extent) {
    m_extent = extent;
    glfwSetWindowSize(m_window, static_cast<int>(extent.width), static_cast<int>(extent.height));
}

Extent2D GLFWWindow::getExtent() const {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return {static_cast<u32>(width), static_cast<u32>(height)};
}

bool GLFWWindow::shouldClose() const {
    return glfwWindowShouldClose(m_window) != 0;
}

void GLFWWindow::pollEvents() {
    glfwPollEvents();
}

void GLFWWindow::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void GLFWWindow::setTitle(const std::string& title) {
    glfwSetWindowTitle(m_window, title.c_str());
}

void GLFWWindow::setVsync(bool enabled) {
    m_vsync = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

bool GLFWWindow::isVsync() const {
    return m_vsync;
}

void GLFWWindow::setResizeCallback(ResizeCallback callback) {
    m_resizeCallback = callback;
}

void GLFWWindow::setKeyCallback(KeyCallback callback) {
    m_keyCallback = callback;
}

void GLFWWindow::setMouseButtonCallback(MouseButtonCallback callback) {
    m_mouseButtonCallback = callback;
}

void GLFWWindow::setCursorPosCallback(CursorPosCallback callback) {
    m_cursorPosCallback = callback;
}

void GLFWWindow::setScrollCallback(ScrollCallback callback) {
    m_scrollCallback = callback;
}

std::unique_ptr<Window> Window::create(const Desc& desc) {
    return std::make_unique<GLFWWindow>(desc);
}

}
