// ecs.tpp
#pragma once
#include "ecs.h"
#include <typeindex>
#include <memory>


template<typename T>
void Component<T>::add(Entity entity, T component) {
  entity_to_index[entity] = components.size();
  components.push_back(component);
  index_to_entity.push_back(entity);
}

template<typename T>
void Component<T>::remove(Entity entity) {
  size_t index = entity_to_index[entity];
  size_t last_index = components.size() - 1;

  components[index] = components[last_index];
  entity_to_index[index_to_entity[last_index]] = index;
  index_to_entity[index] = index_to_entity[last_index];

  components.pop_back();
  index_to_entity.pop_back();
  entity_to_index.erase(entity);
}

template<typename T>
T& Component<T>::get(Entity entity) {
  return components[entity_to_index[entity]];
}



template<typename T>
Component<T>& EntityComponentSystem::getStorage() {
  auto tid = std::type_index{typeid(T)};
  if (storages.find(tid) == storages.end())
    storages[tid] = std::make_unique<Component<T>>();
  return *static_cast<Component<T>*>(storages[tid].get());
}

template<typename T>
void EntityComponentSystem::addComponent(Entity entity, T component) {
    getStorage<T>().add(entity, component);
}

template<typename T>
void EntityComponentSystem::removeComponent(Entity entity) {
    getStorage<T>().remove(entity);
}

template<typename T>
T& EntityComponentSystem::getComponent(Entity entity) {
    return getStorage<T>().get(entity);
}

template<typename ComponentType, typename Func>
void EntityComponentSystem::forEach(Func func) {
  auto& storage = getStorage<ComponentType>();

  for (ComponentType component : storage.components)
    func(component);
}
