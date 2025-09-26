#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <typeindex>
#include <memory>
#include <utility>

using Entity = uint32_t;

class ComponentBase {
public:
    virtual ~ComponentBase() = default;
    virtual size_t size() const noexcept = 0;
    virtual const std::vector<Entity>& getEntities() const = 0;
};

template<typename T>
class Component: public ComponentBase {
  std::vector<T> components;
  std::unordered_map<Entity, size_t> entity_to_index;
  std::vector<Entity> index_to_entity;

public:
  size_t size() const noexcept override;
  const std::vector<Entity>& getEntities() const override;

  void add(Entity entity, T component);
  template<typename... Arguments> void emplace(Entity entity, Arguments&&... args);
  void remove(Entity entity);
  bool has(Entity entity) const;
  T* getPointer(Entity entity);
  T& get(Entity entity);
  const T& get(Entity entity) const;
  std::vector<T>& getComponents();
  const std::vector<T>& getComponents() const;
  void reserve(size_t n);
};

class EntityComponentSystem {
  std::unordered_map<std::type_index, std::unique_ptr<ComponentBase>> storages;

  template<typename T> Component<T>& getComponentStorage();
  // template<typename Tuple> auto* pick_smallest_storage(Tuple& storages);

public:
  template<typename T> void addComponent(Entity entity, T component);
  template<typename T, typename... Arguments> void emplaceComponent(Entity entity, Arguments&&... args);
  template<typename T> void removeComponent(Entity entity);
  template<typename T> T* getComponentPointer(Entity entity);
  template<typename T> T& getComponent(Entity entity);
  template<typename T> bool hasComponent(Entity entity);
  template<typename... ComponentTypes, typename Func>
    void forEach(Func func);
  template<typename... ComponentTypes, typename Func>
    void forEachWithEntity(Func func);
  template<typename... ComponentTypes> struct ComponentJoin {
    std::vector<Entity> entities;
    std::tuple<std::vector<ComponentTypes>...> arrays;
  };
  template<typename... ComponentTypes>
  EntityComponentSystem::ComponentJoin<ComponentTypes...> joinComponents();
};

#include "ecs.tpp"