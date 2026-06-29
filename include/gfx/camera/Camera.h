#pragma once

#include <gfx/core/types.h>
#include <glm/glm.hpp>
#include <string>

namespace gfx {

class Camera {
public:
    enum class ProjectionType {
        Perspective,
        Orthographic
    };
    
    Camera();
    virtual ~Camera() = default;
    
    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);
    
    const glm::vec3& getPosition() const;
    const glm::vec3& getTarget() const;
    const glm::vec3& getUp() const;
    
    void setFov(f32 fov);
    void setAspectRatio(f32 ratio);
    void setNear(f32 near);
    void setFar(f32 far);
    
    f32 getFov() const;
    f32 getAspectRatio() const;
    f32 getNear() const;
    f32 getFar() const;
    
    void setProjectionType(ProjectionType type);
    ProjectionType getProjectionType() const;
    
    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjectionMatrix() const;
    const glm::mat4& getViewProjectionMatrix() const;
    
    void update();
    
private:
    glm::vec3 m_position = {0.0f, 0.0f, 5.0f};
    glm::vec3 m_target = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    
    f32 m_fov = 45.0f;
    f32 m_aspectRatio = 16.0f / 9.0f;
    f32 m_near = 0.1f;
    f32 m_far = 1000.0f;
    
    ProjectionType m_projectionType = ProjectionType::Perspective;
    
    mutable glm::mat4 m_viewMatrix;
    mutable glm::mat4 m_projectionMatrix;
    mutable glm::mat4 m_viewProjectionMatrix;
    mutable bool m_viewDirty = true;
    mutable bool m_projectionDirty = true;
};

}
