#pragma once

#include <gfx/scene/Entity.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace gfx {

class Scene {
public:
    Scene();
    ~Scene();
    
    Entity* createEntity(const std::string& name = "");
    void destroyEntity(Entity::ID id);
    Entity* getEntity(Entity::ID id);
    const Entity* getEntity(Entity::ID id) const;
    
    template<typename T>
    std::vector<Entity*> getEntitiesWithComponent();
    
    template<typename T>
    std::vector<const Entity*> getEntitiesWithComponent() const;
    
    const std::unordered_map<Entity::ID, std::unique_ptr<Entity>>& getEntities() const;
    
    void update(float deltaTime);
    
    void clear();
    
private:
    std::unordered_map<Entity::ID, std::unique_ptr<Entity>> m_entities;
    Entity::ID m_nextID = 1;
};

template<typename T>
std::vector<Entity*> Scene::getEntitiesWithComponent() {
    std::vector<Entity*> result;
    for (auto& [id, entity] : m_entities) {
        if (entity->hasComponent<T>()) {
            result.push_back(entity.get());
        }
    }
    return result;
}

template<typename T>
std::vector<const Entity*> Scene::getEntitiesWithComponent() const {
    std::vector<const Entity*> result;
    for (auto& [id, entity] : m_entities) {
        if (entity->hasComponent<T>()) {
            result.push_back(entity.get());
        }
    }
    return result;
}

}
