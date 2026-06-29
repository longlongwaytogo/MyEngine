#include <gfx/scene/Entity.h>

namespace gfx {

Entity::Entity(ID id, const std::string& name) 
    : m_id(id), m_name(name) {}

Entity::ID Entity::getID() const {
    return m_id;
}

const std::string& Entity::getName() const {
    return m_name;
}

void Entity::setName(const std::string& name) {
    m_name = name;
}

Transform& Entity::getTransform() {
    return m_transform;
}

const Transform& Entity::getTransform() const {
    return m_transform;
}

const std::vector<std::unique_ptr<Component>>& Entity::getComponents() const {
    return m_components;
}

Entity* Entity::getParent() {
    return m_parent;
}

const Entity* Entity::getParent() const {
    return m_parent;
}

void Entity::setParent(Entity* parent) {
    if (m_parent) {
        auto it = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
        if (it != m_parent->m_children.end()) {
            m_parent->m_children.erase(it);
        }
    }
    
    m_parent = parent;
    
    if (m_parent) {
        m_parent->m_children.push_back(this);
    }
}

const std::vector<Entity*>& Entity::getChildren() const {
    return m_children;
}

void Entity::addChild(Entity* child) {
    if (child && child != this) {
        child->setParent(this);
    }
}

void Entity::removeChild(Entity* child) {
    if (child) {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            (*it)->m_parent = nullptr;
            m_children.erase(it);
        }
    }
}

bool Entity::isActive() const {
    return m_active;
}

void Entity::setActive(bool active) {
    m_active = active;
}

void Entity::update(float deltaTime) {
    for (auto& component : m_components) {
        if (component->isEnabled()) {
            component->update(deltaTime);
        }
    }
    
    for (auto child : m_children) {
        if (child->isActive()) {
            child->update(deltaTime);
        }
    }
}

}
