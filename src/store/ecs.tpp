#pragma once
#include "ecs.h"
#include <stdexcept>

// ---------- Component ----------


// void add(Entity entity, T component);
template<typename T>
void Component<T>::add(Entity entity, T component) {
  if (has(entity)) {
    components[entity_to_index[entity]] = std::move(component);
    return;
  }

  entity_to_index.emplace(entity, components.size());
  index_to_entity.push_back(entity);
  components.push_back(std::move(component));
}

// void emplace(Entity entity, Arguments&&... args);
template<typename T>
template<typename... Arguments>
void Component<T>::emplace(Entity entity, Arguments&&... args) {
  if (has(entity)) {
    components[entity_to_index[entity]] = T(std::forward<Arguments>(args)...);
    return;
  }

  entity_to_index.emplace(entity, components.size());
  index_to_entity.push_back(entity);
  components.emplace_back(std::forward<Arguments>(args)...);
}

// void remove(Entity entity);
template<typename T>
void Component<T>::remove(Entity entity) {
  auto index_iterator = entity_to_index.find(entity);
  if (index_iterator = entity_to_index.end()) return;

  size_t index = index_iterator->second;
  size_t last_index = components.size() - 1;

  if (index != last_index) {
    components[index] = std::move(components[last_index]);
    Entity moved_entity = index_to_entity[last_index];
    entity_to_index[moved_entity] = index;
    index_to_entity[index] = moved_entity;
  }

  components.pop_back();
  index_to_entity.pop_back();
  entity_to_index.erase(index_iterator);
}

// bool has(Entity entity);
template<typename T>
bool Component<T>::has(Entity entity) const {
  return entity_to_index.find(entity) != entity_to_index.end();
}

// T* getPointer(Entity entity);
template<typename T>
T* Component<T>::getPointer(Entity entity) {
  auto index_iterator = entity_to_index.find(entity);
  if (index_iterator == entity_to_index.end()) return nullptr;
  return &components[index_iterator->second];
}

// T& get(Entity entity);
template<typename T>
T& Component<T>::get(Entity entity) {
  T* ptr = getPointer(entity);
  if (!ptr) throw std::runtime_error("entity not found");
  return *ptr;
}

// const T& get(Entity entity) const;
template<typename T>
const T& Component<T>::get(Entity entity) const {
  auto index_iterator = entity_to_index.find(entity);
  if (index_iterator == entity_to_index.end()) throw std::runtime_error("entity not found");
  return components[index_iterator->second];
}

// std::vector<T>& getComponents();
template<typename T>
std::vector<T>& Component<T>::getComponents() {
  return components;
}

// const std::vector<T>& getComponents() const;
template<typename T>
const std::vector<T>& Component<T>::getComponents() const {
  return components;
}

// const std::vector<Entity>& getEntities() const override;
template<typename T>
const std::vector<Entity>& Component<T>::getEntities() const {
  return index_to_entity;
}

// size_t size() const noexcept override;
template<typename T>
size_t Component<T>::size() const noexcept {
  return components.size();
}

// void reserve(size_t n);
template<typename T>
void Component<T>::reserve(size_t n) {
  components.reserve(n);
  index_to_entity.reserve(n);
  entity_to_index.reserve(n*2);
}

// ---------- EntityComponentSystem ----------

// Component<T>& getComponentStorage();
template<typename T>
Component<T>& EntityComponentSystem::getComponentStorage() {
  auto tid = std::type_index{typeid(T)};
  auto iter = storages.find(tid);
  if (iter == storages.end()) {
    auto res = storages.try_emplace(tid, std::make_unique<Component<T>>());
    iter = res.first;
  }
  return *static_cast<Component<T>*>(iter->second.get());
}

// void addComponent(Entity entity, T component);
template<typename T>
void EntityComponentSystem::addComponent(Entity entity, T component) {
  getComponentStorage<T>().add(entity, component);
}

// void emplaceComponent(Entity entity, Arguments&&... args);
template<typename T, typename... Arguments>
void EntityComponentSystem::emplaceComponent(Entity entity, Arguments&&... args) {
  getComponentStorage<T>().emplace(entity, std::forward<Arguments>(args)...);
}

// void removeComponent(Entity entity);
template<typename T>
void EntityComponentSystem::removeComponent(Entity entity) {
  getComponentStorage<T>().remove(entity);
}

// T* getComponentPointer(Entity entity);
template<typename T>
T* EntityComponentSystem::getComponentPointer(Entity entity) {
  return getComponentStorage<T>().getPointer(entity);
}

// T& getComponent(Entity entity);
template<typename T>
T& EntityComponentSystem::getComponent(Entity entity) {
  return getComponentStorage<T>().get(entity);
}

// bool hasComponent(Entity entity);
template<typename T>
bool EntityComponentSystem::hasComponent(Entity entity) {
  return getComponentStorage<T>().has(entity);
}


// auto& pick_smallest_storage(Tuple& storages)
namespace ecs_detail {
  inline ComponentBase* pick_smallest_storage(const std::vector<ComponentBase*>& storages) {
    if (storages.empty()) return nullptr;
    ComponentBase* smallest = storages[0];
    for (ComponentBase* c : storages)
      if (c->size() < smallest->size()) smallest = c;
    return smallest;
  }
}


// void forEach(Func func);
template<typename... ComponentTypes, typename Func>
void EntityComponentSystem::forEach(Func func) {
  static_assert(sizeof...(ComponentTypes) > 0, "forEach must take at least one component type");

  std::vector<ComponentBase*> storages_vec = { &getComponentStorage<ComponentTypes>()... };
  ComponentBase* smallest_storage = ecs_detail::pick_smallest_storage(storages_vec);
  if (!smallest_storage) return;

  for (Entity e : smallest_storage->getEntities()) {
    if ((getComponentStorage<ComponentTypes>().has(e) && ...)) {
      func(getComponentStorage<ComponentTypes>().get(e)...);
    }
  }
}

// void forEachWithEntity(Func func);
template<typename... ComponentTypes, typename Func>
void EntityComponentSystem::forEachWithEntity(Func func) {
  static_assert(sizeof...(ComponentTypes) > 0, "forEach must take at least one component type");

  std::vector<ComponentBase*> storages_vec = { &getComponentStorage<ComponentTypes>()... };
  ComponentBase* smallest_storage = ecs_detail::pick_smallest_storage(storages_vec);
  if (!smallest_storage) return;

  for (Entity e : smallest_storage->getEntities()) {
    if ((getComponentStorage<ComponentTypes>().has(e) && ...)) {
      func(e, getComponentStorage<ComponentTypes>().get(e)...);
    }
  }
}