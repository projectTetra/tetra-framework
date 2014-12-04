#pragma once
#ifndef TETRA_FRAMEWORK_ENTITYMANAGER_HPP
#define TETRA_FRAMEWORK_ENTITYMANAGER_HPP

#include <tetra/framework/Entity.hpp>

#include <unordered_map>
#include <vector>

namespace tetra
{
namespace framework
{

/**
 * Thrown by the EntityManager when an operation is done on an entity
 * that does not exist.
 **/
class EntityDoesNotExistException;

/**
 * The EntityManager is the core of the tetra-framework.
 * The EntityManager manages a set of entities and facilitates the
 * execution of "systems" on those entities.
 **/
class EntityManager
{
public:
  struct EntityDescriptor;

  using EntityId = std::size_t;
  using EntityMap = std::unordered_map<EntityId, Entity>;
  using EntityList = std::vector<EntityDescriptor>;

private:
  EntityMap entityMap;
  std::vector<EntityId> entitiesToDestroy;

public:
  /**
   * Creates a new entity and adds it to the EntityMap.
   **/
  EntityId createEntity() noexcept;

  /**
   * Retrieves the entity with the EntityId given.
   * @throws EntityDoesNotExistException if the entity does not exist.
   * @param entityId The Id of the entity to retrieve.
   * @return referencet to the entity with this id, the reference is
   *         only valid until the next call to update()
   **/
  Entity& getEntity( EntityId entityId );

  /**
   * Returns a vector of all entities currently in the EntityManager.
   **/
  EntityList getAllEntities() noexcept;

  /**
   * Returns a vector of all entities which have the requested
   * component types.
   **/
  template <class... ComponentTypes>
  EntityList getEntities() noexcept;

  /**
   * Publishes a DestroyEntityMessage which will cause the entity to
   * be destroyed on the next call to update(). (if the entity is
   * already gone or non-existent then proccessing this message is
   * effectively a no-op).
   * (note, this can be done manually by calling getMessageManager()
   * and publishing a DestroyEntityMessage)
   **/
  void postDestroyEntityMessage( EntityId id ) noexcept;

  /**
   * Processes all pending messages. (this is when destroy entity
   * messages are processed)
   **/
  void update();
};

class EntityDoesNotExistException : public std::runtime_error
{
public:
  EntityDoesNotExistException();
};

struct EntityManager::EntityDescriptor
{
  EntityDescriptor() = default;
  EntityDescriptor( EntityId, Entity* );

  EntityId entityId;
  Entity* entity;
};

#include <tetra/framework/entitymanager/EntityManagerImpl.hpp>
} /* namespace framework */
} /* namespace tetra */

#endif
