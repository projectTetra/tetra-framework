#include <tetra/framework/SerializationSystem.hpp>

#include <catch.hpp>
#include <test/VectorComponent.hpp>
#include <test/Widget.hpp>

#include <iostream>
#include <sstream>

using namespace std;
using namespace test;
using namespace tetra;
using namespace tetra::meta;
using namespace tetra::framework;

void addEntityWithVectorAndWidget( EntityManager& entityManager )
{
  entityManager.getEntity( entityManager.createEntity() )
    .addComponent( Widget{} )
    .addComponent( VectorComponent{1.0f, 2.0f, 3.0f} );
}

SCENARIO( "Using the SerializationSystem to serialize and "
          "deserialize the contents of an entity manager" )
{
  MetaRepository repository;
  SerializationSystem serializer{repository};

  repository.addType<VectorComponent>( "vector3d" );
  repository.addType<Widget>( "Widget" );

  GIVEN( "A MetaRepository and a SerializationSystem" )
  {
    EntityManager manager{};
    for ( int i = 0; i < 10; ++i )
      addEntityWithVectorAndWidget( manager );

    THEN( "Serializing an EntityManager should be successful" )
    {
      stringstream ss;
      serializer.serialize( manager, ss ); 
    }

    THEN( "Deserializing the EntityManager should be successful" )
    {
      stringstream sstream;  
      serializer.serialize( manager, sstream );

      EntityManager toDeserialize;
      serializer.deserialize( toDeserialize, sstream );

      auto entityList =
        toDeserialize.getEntities<Widget, VectorComponent>();
      REQUIRE( entityList.size() == 10 );
    }
  }
}











