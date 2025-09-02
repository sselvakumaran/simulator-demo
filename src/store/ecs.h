#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <typeindex>
#include <ranges>
#include <memory>

using Entity = uint32_t;

class ComponentBase {public: virtual ~ComponentBase() = default;};

template<typename T>
class Component: public ComponentBase {
  std::vector<T> components;
  std::unordered_map<Entity, size_t> entity_to_index;
  std::vector<Entity> index_to_entity;
public:
  void add(Entity entity, T component);
  void remove(Entity entity);
  bool has(Entity entity);
  T& get(Entity entity);
  std::vector<T>& getComponents();
  auto getEntities();
};

class EntityComponentSystem {
  std::unordered_map<std::type_index, std::unique_ptr<ComponentBase>> storages;
public:
  template<typename T>
  Component<T>& getStorage();

  template<typename T>
  void addComponent(Entity entity, T component);

  template<typename T>
  void removeComponent(Entity entity);

  template<typename T>
  T& getComponent(Entity entity);

  template<typename T>
  bool hasComponent(Entity entity);

  template<typename FirstComponentType, typename... OtherComponentTypes, typename Func>
  void forEach(Func func);
};

#include "ecs.tpp"