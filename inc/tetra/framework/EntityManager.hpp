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
class EntityDoesNotExistException : public std::runtime_error
{
public:
  inline EntityDoesNotExistException()
    : std::runtime_error{"Entity does not exist"}
  { }
};

class DestroyEntityMessage; 

/**
 * The EntityManager is the core of the tetra-framework.
 * The EntityManager manages a set of entities and facilitates the
 * execution of "systems" on those entities.
 **/
class EntityManager
{
public:
  using EntityId = std::size_t;
  using EntityMap = std::unordered_map<EntityId, Entity>;
  using EntityList = std::vector<Entity*>;

private:
  EntityMap entityMap;

public:
  /**
   * Creates a new entity and adds it to the EntityMap.
   **/
  EntityId createEntity() noexcept;

  /**
   * Retrieves the entity with the EntityId given.
   * @throws EntityDoesNotExistException if the entity does not exist.
   **/
  Entity& getEntity() noexcept;

  /**
   * Returns a vector of all entities currently in the EntityManager.
   **/
  EntityList getAllEntities() noexcept;

  /**
   * Publishes a DestroyEntityMessage which will cause the entity to
   * be destroyed on the next call to update(). (if the entity is
   * already gone or non-existent then proccessing this message is
   * effectively a no-op).
   **/
  void postDestroyEntityMessage( EntityId id ) noexcept;

private:
  /**
   * Handles a destroyed entity message, private so that systems don't
   * call this directly, but friended to the MessageManager so that we
   * can create a delegate.
   **/
  void handleDestroyEntity( const DestroyEntityMessage& message );
};

class DestroyEntityMessage
{
  const EntityManager::EntityId id;

public:
  inline DestroyEntityMessage( EntityManager::EntityId id )
    : id{id} {};

  inline EntityManager::EntityId getEntityId() const noexcept
  {
    return id;
  };
};

} /* namespace framework */
} /* namespace tetra */

#endif
