#include <tetra/framework/SerializationSystem.hpp>

#include <json/json.h>

using namespace std;
using namespace tetra::meta;
using namespace tetra::framework;

SerializationSystem::SerializationSystem(
  const MetaRepository& metaRepository )
  : metaRepository{metaRepository}
{ }

void SerializationSystem::serialize( EntityManager& entityManager,
                                     std::ostream& out ) const
{
  Json::Value root;
  Json::Value entities;

  auto entityList = entityManager.getAllEntities();
  for ( const auto& entityDesc : entityList )
  {
    Json::Value entityNode;
    entityDesc.entity->serialize( metaRepository, entityNode );

    entities.append( entityNode );
  }

  root["entities"] = entities;

  Json::StyledStreamWriter writer;
  writer.write( out, root );
}

void SerializationSystem::deserialize( EntityManager& entityManager,
                                       std::istream& in ) const
{
  Json::Value root;
  Json::Reader reader;
  reader.parse( in, root, false );

  Json::Value entities = root.get( "entities", 5 );
  if ( !entities.isArray() ) return;

  for ( auto iter = begin( entities ); iter != end( entities );
        ++iter )
  {
    Entity& entity =
      entityManager.getEntity( entityManager.createEntity() );

    entity.deserialize( metaRepository, *iter );
  }
}

