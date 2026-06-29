#include <gfx/camera/OrbitController.h>
#include <GLFW/glfw3.h>

namespace gfx {

OrbitController::OrbitController(Camera* camera) : m_camera(camera) {
    updateCameraPosition();
}

OrbitController::~OrbitController() {
    if (m_window) {
    }
}

void OrbitController::setWindow(Window* window) {
    m_window = window;
    if (m_window) {
        m_window->setMouseButtonCallback(
            [this](int button, int action, int mods) { onMouseDown(button, action, mods); }
        );
        m_window->setCursorPosCallback(
            [this](double x, double y) { onMouseMove(x, y); }
        );
        m_window->setScrollCallback(
            [this](double xoffset, double yoffset) { onScroll(xoffset, yoffset); }
        );
    }
}

void OrbitController::update(f32 deltaTime) {
    (void)deltaTime;
    updateCameraPosition();
}

void OrbitController::setRadius(f32 radius) {
    m_radius = radius;
    updateCameraPosition();
}

void OrbitController::setSpeed(f32 speed) {
    m_speed = speed;
}

void OrbitController::setSensitivity(f32 sensitivity) {
    m_sensitivity = sensitivity;
}

f32 OrbitController::getRadius() const {
    return m_radius;
}

f32 OrbitController::getSpeed() const {
    return m_speed;
}

f32 OrbitController::getSensitivity() const {
    return m_sensitivity;
}

void OrbitController::setPivot(const glm::vec3& pivot) {
    m_pivot = pivot;
    updateCameraPosition();
}

const glm::vec3& OrbitController::getPivot() const {
    return m_pivot;
}

void OrbitController::updateCameraPosition() {
    glm::vec3 position;
    position.x = m_pivot.x + m_radius * cos(m_phi) * cos(m_theta);
    position.y = m_pivot.y + m_radius * sin(m_phi);
    position.z = m_pivot.z + m_radius * cos(m_phi) * sin(m_theta);
    
    m_camera->setPosition(position);
    m_camera->setTarget(m_pivot);
}

void OrbitController::onMouseDown(int button, int action, int mods) {
    (void)mods;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        m_mouseDown = (action == GLFW_PRESS);
    }
}

void OrbitController::onMouseMove(double x, double y) {
    if (m_mouseDown) {
        f32 deltaX = static_cast<f32>(x - m_lastMouseX);
        f32 deltaY = static_cast<f32>(y - m_lastMouseY);
        
        m_theta += deltaX * m_sensitivity * 0.01f;
        m_phi -= deltaY * m_sensitivity * 0.01f;
        
        m_phi = glm::clamp(m_phi, glm::radians(1.0f), glm::radians(179.0f));
        
        updateCameraPosition();
    }
    
    m_lastMouseX = static_cast<f32>(x);
    m_lastMouseY = static_cast<f32>(y);
}

void OrbitController::onScroll(double xoffset, double yoffset) {
    (void)xoffset;
    m_radius -= static_cast<f32>(yoffset) * m_speed;
    m_radius = glm::max(m_radius, 0.1f);
    updateCameraPosition();
}

}
