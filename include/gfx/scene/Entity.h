#pragma once

#include <gfx/scene/Transform.h>
#include <gfx/scene/Component.h>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <typeindex>
#include <unordered_map>

namespace gfx {

class Entity {
public:
    using ID = uint64_t;
    
    Entity(ID id, const std::string& name = "");
    ~Entity() = default;
    
    ID getID() const;
    const std::string& getName() const;
    void setName(const std::string& name);
    
    Transform& getTransform();
    const Transform& getTransform() const;
    
    template<typename T, typename... Args>
    T* addComponent(Args&&... args);
    
    template<typename T>
    T* getComponent();
    
    template<typename T>
    const T* getComponent() const;
    
    template<typename T>
    bool hasComponent() const;
    
    template<typename T>
    void removeComponent();
    
    const std::vector<std::unique_ptr<Component>>& getComponents() const;
    
    Entity* getParent();
    const Entity* getParent() const;
    void setParent(Entity* parent);
    
    const std::vector<Entity*>& getChildren() const;
    void addChild(Entity* child);
    void removeChild(Entity* child);
    
    bool isActive() const;
    void setActive(bool active);
    
    void update(float deltaTime);
    
private:
    ID m_id;
    std::string m_name;
    Transform m_transform;
    std::vector<std::unique_ptr<Component>> m_components;
    std::unordered_map<std::type_index, Component*> m_componentMap;
    
    Entity* m_parent = nullptr;
    std::vector<Entity*> m_children;
    bool m_active = true;
};

template<typename T, typename... Args>
T* Entity::addComponent(Args&&... args) {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    
    if (hasComponent<T>()) {
        return getComponent<T>();
    }
    
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = component.get();
    m_components.push_back(std::move(component));
    m_componentMap[std::type_index(typeid(T))] = ptr;
    ptr->onAttach(this);
    
    return ptr;
}

template<typename T>
T* Entity::getComponent() {
    auto it = m_componentMap.find(std::type_index(typeid(T)));
    if (it != m_componentMap.end()) {
        return static_cast<T*>(it->second);
    }
    return nullptr;
}

template<typename T>
const T* Entity::getComponent() const {
    auto it = m_componentMap.find(std::type_index(typeid(T)));
    if (it != m_componentMap.end()) {
        return static_cast<const T*>(it->second);
    }
    return nullptr;
}

template<typename T>
bool Entity::hasComponent() const {
    return m_componentMap.find(std::type_index(typeid(T))) != m_componentMap.end();
}

template<typename T>
void Entity::removeComponent() {
    auto it = m_componentMap.find(std::type_index(typeid(T)));
    if (it == m_componentMap.end()) {
        return;
    }
    
    Component* component = it->second;
    component->onDetach();
    
    for (auto compIt = m_components.begin(); compIt != m_components.end(); ++compIt) {
        if (compIt->get() == component) {
            m_components.erase(compIt);
            break;
        }
    }
    
    m_componentMap.erase(it);
}

}
