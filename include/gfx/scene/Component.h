#pragma once

#include <typeindex>

namespace gfx {

class Entity;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;
    
    virtual void onAttach(Entity* entity) { m_owner = entity; }
    virtual void onDetach() { m_owner = nullptr; }
    virtual void update(float deltaTime) { (void)deltaTime; }
    
    Entity* getOwner() { return m_owner; }
    const Entity* getOwner() const { return m_owner; }
    
    virtual std::type_index getType() const = 0;
    
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    
protected:
    Entity* m_owner = nullptr;
    bool m_enabled = true;
};

template<typename T>
class TypedComponent : public Component {
public:
    std::type_index getType() const override {
        return std::type_index(typeid(T));
    }
};

}
