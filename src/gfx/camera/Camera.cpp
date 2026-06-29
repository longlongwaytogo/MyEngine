#include <gfx/camera/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace gfx {

Camera::Camera() {}

void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
    m_viewDirty = true;
}

void Camera::setTarget(const glm::vec3& target) {
    m_target = target;
    m_viewDirty = true;
}

void Camera::setUp(const glm::vec3& up) {
    m_up = up;
    m_viewDirty = true;
}

const glm::vec3& Camera::getPosition() const {
    return m_position;
}

const glm::vec3& Camera::getTarget() const {
    return m_target;
}

const glm::vec3& Camera::getUp() const {
    return m_up;
}

void Camera::setFov(f32 fov) {
    m_fov = fov;
    m_projectionDirty = true;
}

void Camera::setAspectRatio(f32 ratio) {
    m_aspectRatio = ratio;
    m_projectionDirty = true;
}

void Camera::setNear(f32 near) {
    m_near = near;
    m_projectionDirty = true;
}

void Camera::setFar(f32 far) {
    m_far = far;
    m_projectionDirty = true;
}

f32 Camera::getFov() const {
    return m_fov;
}

f32 Camera::getAspectRatio() const {
    return m_aspectRatio;
}

f32 Camera::getNear() const {
    return m_near;
}

f32 Camera::getFar() const {
    return m_far;
}

void Camera::setProjectionType(ProjectionType type) {
    m_projectionType = type;
    m_projectionDirty = true;
}

Camera::ProjectionType Camera::getProjectionType() const {
    return m_projectionType;
}

const glm::mat4& Camera::getViewMatrix() const {
    if (m_viewDirty) {
        m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
        m_viewDirty = false;
    }
    return m_viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const {
    if (m_projectionDirty) {
        if (m_projectionType == ProjectionType::Perspective) {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
        } else {
            f32 halfWidth = m_far * 0.1f;
            f32 halfHeight = halfWidth / m_aspectRatio;
            m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_near, m_far);
        }
        m_projectionDirty = false;
    }
    return m_projectionMatrix;
}

const glm::mat4& Camera::getViewProjectionMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}

void Camera::update() {
    getViewMatrix();
    getProjectionMatrix();
}

}
