#include <tetra/framework/EntityManager.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;
using namespace tetra::framework;

using EntityId = EntityManager::EntityId;
using EntityList = EntityManager::EntityList;

EntityId EntityManager::createEntity() noexcept
{
  static EntityId index{0};
  EntityId id = ++index;  
  
  entityMap.emplace( id, Entity{} );

  return id;
}

Entity& EntityManager::getEntity( EntityId id )
{
  auto iter = entityMap.find( id );
  if (iter == end(entityMap))
    throw EntityDoesNotExistException{};

  return iter->second;
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




