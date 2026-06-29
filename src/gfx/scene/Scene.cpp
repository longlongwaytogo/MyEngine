#include <gfx/scene/Scene.h>

namespace gfx {

Scene::Scene() {}

Scene::~Scene() {
    clear();
}

Entity* Scene::createEntity(const std::string& name) {
    Entity::ID id = m_nextID++;
    auto entity = std::make_unique<Entity>(id, name);
    Entity* ptr = entity.get();
    m_entities[id] = std::move(entity);
    return ptr;
}

void Scene::destroyEntity(Entity::ID id) {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        m_entities.erase(it);
    }
}

Entity* Scene::getEntity(Entity::ID id) {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return it->second.get();
    }
    return nullptr;
}

const Entity* Scene::getEntity(Entity::ID id) const {
    auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return it->second.get();
    }
    return nullptr;
}

const std::unordered_map<Entity::ID, std::unique_ptr<Entity>>& Scene::getEntities() const {
    return m_entities;
}

void Scene::update(float deltaTime) {
    for (auto& [id, entity] : m_entities) {
        if (entity->isActive()) {
            entity->update(deltaTime);
        }
    }
}

void Scene::clear() {
    m_entities.clear();
    m_nextID = 1;
}

}
