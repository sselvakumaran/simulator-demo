#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <typeindex>
#include <memory>

#include "ecs.h"

using Entity = uint32_t;

class EntityComponentSystem {
  std::unordered_map<std::type_index, std::unique_ptr<ComponentBase>> storages;
public:
  template<typename T>
  Component<T>& getStorage() {
    auto tid = std::type_index {typeid(T)};
    if (storages.find(tid) == storages.end())
      storages[tid] = std::make_unique<Component<T>>;
    return *static_cast<Component<T>*>(storages[tid].get());
  }

  template<typename T>
  void addComponent(Entity entity, T component) {
    getStorage<T>().add(entity, component);
  }

  template<typename T>
  void removeComponent(Entity entity) {
    getStorage<T>().remove(entity);
  }

  template<typename T>
  T& getComponent(Entity entity) {
    return getStorage<T>().get(entity);
  }

  template<typename ComponentType, typename Func>
  void forEach(Func func) {
    auto& storage = getStorage<ComponentType>();
    for (Entity entity : storage.index_to_entity)
      func(entity);
  }

  // template<typename FirstComponentType, typename... ComponentTypes, typename Func>
  // void forEach(Func func) {
  //   auto& first_storage = getStorage<FirstComponentType>();
  //   for (Entity entity : first_storage.index_to_entity) {
  //     if ((getStorage<ComponentTypes>().entity_to_index.count(entity) && ...)) {
  //       func(first_storage.get(entity), getStorage<ComponentTypes>().get(entity)...);
  //     }
  //   }
  // }
};

class ComponentBase {public: virtual ~ComponentBase() = default;};

template<typename T>
class Component: public ComponentBase {
  std::vector<T> components;
  std::unordered_map<Entity, size_t> entity_to_index;
  std::vector<Entity> index_to_entity;
public:
  void add(Entity entity, T component) {
    entity_to_index[entity] = components.size();
    components.push_back(component);
    index_to_entity.push_back(entity);
  }
  void remove(Entity entity) {
    size_t index = entity_to_index[entity], last_index = components.size() - 1;
    components[index] = components[last];
    entity_to_index[index_to_entity[last]] = index;
    index_to_entity[index] = index_to_entity[last];
    components.pop_back();
    index_to_entity.pop_back();
    entity_to_index.erase(entity);
  }
  T& get(Entity entity) {
    return components[entity_to_index[entity]];
  }
};