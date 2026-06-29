#pragma once

#include <gfx/camera/Camera.h>
#include <gfx/platform/Window.h>

namespace gfx {

class OrbitController {
public:
    OrbitController(Camera* camera);
    ~OrbitController();
    
    void setWindow(Window* window);
    void update(f32 deltaTime);
    
    void setRadius(f32 radius);
    void setSpeed(f32 speed);
    void setSensitivity(f32 sensitivity);
    
    f32 getRadius() const;
    f32 getSpeed() const;
    f32 getSensitivity() const;
    
    void setPivot(const glm::vec3& pivot);
    const glm::vec3& getPivot() const;
    
private:
    Camera* m_camera;
    Window* m_window = nullptr;
    
    f32 m_radius = 5.0f;
    f32 m_speed = 1.0f;
    f32 m_sensitivity = 0.1f;
    
    glm::vec3 m_pivot = {0.0f, 0.0f, 0.0f};
    
    f32 m_theta = 0.0f;
    f32 m_phi = glm::radians(45.0f);
    
    bool m_mouseDown = false;
    f32 m_lastMouseX = 0.0f;
    f32 m_lastMouseY = 0.0f;
    
    void updateCameraPosition();
    
    void onMouseDown(int button, int action, int mods);
    void onMouseMove(double x, double y);
    void onScroll(double xoffset, double yoffset);
};

}
