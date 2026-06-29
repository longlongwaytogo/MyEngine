#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gfx {

class Transform {
public:
    Transform() = default;
    
    void setPosition(const glm::vec3& pos);
    const glm::vec3& getPosition() const;
    
    void setRotation(const glm::quat& rot);
    void setRotationEuler(const glm::vec3& eulerDegrees);
    const glm::quat& getRotation() const;
    glm::vec3 getRotationEuler() const;
    
    void setScale(const glm::vec3& scale);
    const glm::vec3& getScale() const;
    
    const glm::mat4& getModelMatrix() const;
    
    void translate(const glm::vec3& delta);
    void rotate(float angleDegrees, const glm::vec3& axis);
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));
    
    void reset();
    
private:
    mutable glm::mat4 m_modelMatrix{1.0f};
    glm::vec3 m_position{0.0f};
    glm::quat m_rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale{1.0f};
    mutable bool m_dirty = true;
};

}
