#include <gfx/hal/opengl/GLInstance.h>
#include <gfx/hal/opengl/GLDevice.h>
#include <gfx/core/logging.h>
#include <GLFW/glfw3.h>

namespace gfx {

GLInstance::GLInstance(const std::string& appName, u32 appVersion, const std::vector<std::string>& extensions)
    : m_appName(appName), m_appVersion(appVersion) {
    GFX_LOG_INFO("OpenGL Instance created: %s v%d", appName.c_str(), appVersion);
}

GLInstance::~GLInstance() {
    GFX_LOG_INFO("OpenGL Instance destroyed");
}

std::vector<std::unique_ptr<Device>> GLInstance::enumerateDevices() {
    std::vector<std::unique_ptr<Device>> devices;
    devices.push_back(std::make_unique<GLDevice>());
    return devices;
}

void* GLInstance::getNativeHandle() const {
    return nullptr;
}

std::unique_ptr<Instance> Instance::create(const std::string& appName, u32 appVersion, const std::vector<std::string>& extensions) {
    return std::make_unique<GLInstance>(appName, appVersion, extensions);
}

}
