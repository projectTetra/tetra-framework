#include <tetra/framework/EntityManager.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;
using namespace tetra::framework;

using EntityId = EntityManager::EntityId;
using EntityList = EntityManager::EntityList;

EntityDoesNotExistException::EntityDoesNotExistException()
  : runtime_error{"Entity does not exist!"} {};

EntityManager::EntityDescriptor::EntityDescriptor( EntityId id,
                                                   Entity* pEnt )
  : entityId{id}, entity{pEnt}
{ }

EntityId EntityManager::createEntity() noexcept
{
  static EntityId index{0};
  EntityId id = ++index;

  entityMap.emplace( id, Entity{} );

  return id;
}

bool EntityManager::entityExists( EntityId id ) const noexcept
{
  auto iter = entityMap.find( id );
  return iter != end( entityMap );
}

Entity& EntityManager::getEntity( EntityId id )
{
  if (!entityExists(id))
    throw EntityDoesNotExistException{};

  return entityMap.find( id )->second;
}

EntityList EntityManager::getAllEntities() noexcept
{
  EntityList entityList;
  entityList.reserve( entityMap.size() );

  for( auto& iter : entityMap )
  {
    entityList.emplace_back( iter.first, &iter.second );
  }

  return entityList;
}

void EntityManager::postDestroyEntityMessage( EntityId id ) noexcept
{
  entitiesToDestroy.push_back( id );
}

void EntityManager::update()
{
  for ( const auto& id : entitiesToDestroy )
  {
    auto iter = entityMap.find( id );
    if ( iter != end( entityMap ) ) entityMap.erase( iter );
  }
  entitiesToDestroy.clear();
}




