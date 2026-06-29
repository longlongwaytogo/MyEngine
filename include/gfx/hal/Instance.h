#pragma once

#include <gfx/core/types.h>
#include <vector>
#include <memory>
#include <string>

namespace gfx {

class Device;

class Instance {
public:
    virtual ~Instance() = default;
    
    virtual std::vector<std::unique_ptr<Device>> enumerateDevices() = 0;
    
    virtual void* getNativeHandle() const = 0;
    
    static std::unique_ptr<Instance> create(const std::string& appName = "MyEngine",
                                             u32 appVersion = 1,
                                             const std::vector<std::string>& extensions = {});
};

}
