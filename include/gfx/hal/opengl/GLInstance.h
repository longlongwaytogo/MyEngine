#pragma once

#include <gfx/hal/Instance.h>

namespace gfx {

class GLInstance : public Instance {
public:
    GLInstance(const std::string& appName, u32 appVersion, const std::vector<std::string>& extensions);
    ~GLInstance() override;
    
    std::vector<std::unique_ptr<Device>> enumerateDevices() override;
    void* getNativeHandle() const override;
    
private:
    std::string m_appName;
    u32 m_appVersion;
};

}
