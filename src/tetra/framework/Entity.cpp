#include <tetra/framework/Entity.hpp>

#include <json/json.h>

using namespace std;
using namespace tetra;
using namespace tetra::meta;
using namespace tetra::framework;

void Entity::serialize( const MetaRepository& repository,
                        Json::Value& root ) const
{
  Json::Value componentList;
  for ( const auto& component : this->components )
  {
    Json::Value componentNode;
    repository.serialize( component.second, componentNode );
    componentList.append( componentNode );
  }

  root["components"] = componentList;
}

void Entity::deserialize( const MetaRepository& repository,
                          Json::Value& root )
{
  Json::Value components = root.get( "components", 5 );
  if ( !components.isArray() ) return;

  auto iter = components.begin();
  for ( ; iter != components.end(); ++iter )
  {
    this->addComponent( repository.deserialize(*iter) );
  }
}

Entity& Entity::addComponent( Variant&& component ) noexcept
{
  components.emplace( &component.getMetaData(),
                      std::move( component ) );
  return *this;
}
