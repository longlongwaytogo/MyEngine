#include <gfx/scene/Transform.h>

namespace gfx {

void Transform::setPosition(const glm::vec3& pos) {
    m_position = pos;
    m_dirty = true;
}

const glm::vec3& Transform::getPosition() const {
    return m_position;
}

void Transform::setRotation(const glm::quat& rot) {
    m_rotation = rot;
    m_dirty = true;
}

void Transform::setRotationEuler(const glm::vec3& eulerDegrees) {
    m_rotation = glm::quat(glm::radians(eulerDegrees));
    m_dirty = true;
}

const glm::quat& Transform::getRotation() const {
    return m_rotation;
}

glm::vec3 Transform::getRotationEuler() const {
    return glm::degrees(glm::eulerAngles(m_rotation));
}

void Transform::setScale(const glm::vec3& scale) {
    m_scale = scale;
    m_dirty = true;
}

const glm::vec3& Transform::getScale() const {
    return m_scale;
}

const glm::mat4& Transform::getModelMatrix() const {
    if (m_dirty) {
        m_modelMatrix = glm::mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        m_modelMatrix = m_modelMatrix * glm::mat4_cast(m_rotation);
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        m_dirty = false;
    }
    return m_modelMatrix;
}

void Transform::translate(const glm::vec3& delta) {
    m_position += delta;
    m_dirty = true;
}

void Transform::rotate(float angleDegrees, const glm::vec3& axis) {
    glm::quat rot = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));
    m_rotation = rot * m_rotation;
    m_dirty = true;
}

void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
    glm::mat4 lookMatrix = glm::lookAt(m_position, target, up);
    m_rotation = glm::quat_cast(glm::inverse(lookMatrix));
    m_dirty = true;
}

void Transform::reset() {
    m_position = glm::vec3(0.0f);
    m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_scale = glm::vec3(1.0f);
    m_dirty = true;
}

}
