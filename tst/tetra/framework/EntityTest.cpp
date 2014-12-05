#include <tetra/framework/Entity.hpp>
#include <tetra/meta/MetaRepository.hpp>

#include <catch.hpp>
#include <json/json.h>
#include <test/VectorComponent.hpp>
#include <test/Widget.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;
using namespace tetra::framework;

Json::Value createEntityNode()
{
  Json::Value root;
  Json::Value components;
  Json::Value component;  
  Json::Value object;

  Variant comp =
    Variant::create( test::VectorComponent{5.0f, 1.0f, -0.3f} );
  comp.serialize( object );
  component["type"] = "vectorComponent";
  component["object"] = object;

  components.append(component);

  Json::Value widgetComponent;
  widgetComponent["type"] = "widget";
  components.append(widgetComponent);

  root["components"] = components;

  return root;
}

SCENARIO( "Serializing an entity", "[entity][serialization]" )
{
  GIVEN(
    "A MetaRepository with VectorComponent and Widget registered" )
  {
    meta::MetaRepository repository;
    repository.addType<test::VectorComponent>( "vectorComponent" );
    repository.addType<test::Widget>( "widget" );

    THEN( "We should be able to serialize an Entity with "
          "VectorComponent and Widget components" )
    {
      Entity entity{};
      entity.addComponent( test::VectorComponent{1.0f, -5.0f, 0.3f} )
        .addComponent( test::Widget{} );

      Json::Value root;
      entity.serialize( repository, root );

      Json::Value components = root.get( "components", 5 );
      REQUIRE( components.isArray() );
    }

    THEN( "We should be able to deserialize an Entity" )
    {
      Entity entity{};
      Json::Value root = createEntityNode();

      entity.deserialize( repository, root );
      
      REQUIRE( entity.hasComponents<test::VectorComponent>() );
      REQUIRE( entity.hasComponents<test::Widget>() );
      const auto& vectorComponent =
        entity.getComponent<test::VectorComponent>();

      REQUIRE( vectorComponent.getX() == 5.0f );
      REQUIRE( vectorComponent.getY() == 1.0f );
      REQUIRE( vectorComponent.getZ() == -0.3f );
    }
  }

  GIVEN( "A MetaRepository without any types registered" )
  {
    MetaRepository repository;

    THEN( "Serializing an entity should throw a "
          "TypeNotRegisteredException" )
    {
      Entity entity;
      entity.addComponent( test::VectorComponent{1.0f, 1.0f, 1.0f} );

      Json::Value root;
      REQUIRE_THROWS_AS( entity.serialize( repository, root ),
                         TypeNotRegisteredException );
    }

    THEN( "Deserializing an entity should throw a "
          "TypeNotRegisteredException" )
    {
      Entity entity;
      Json::Value root = createEntityNode();

      REQUIRE_THROWS_AS( entity.deserialize( repository, root ),
                         TypeNotRegisteredException );
    }
  }
}

SCENARIO( "Using an Entity to hold components", "[entity]" )
{
  Entity entity{};
  entity.addComponent( string{"hello"} ).addComponent( 5 );

  GIVEN( "An entity with int and string components" )
  {
    THEN( "The component should show that it contains the added "
          "components" )
    {
      REQUIRE( entity.hasComponents<string>() );
      REQUIRE( entity.hasComponents<int>() );
      bool hasIntAndString = entity.hasComponents<int, string>();
      REQUIRE( hasIntAndString );
    }

    THEN( "The entity should not claim to contain any components "
          "that it doesn't actually contain" )
    {
      REQUIRE( entity.hasComponents<double>() == false );
      bool hasIntAndDouble = entity.hasComponents<int, double>();
      REQUIRE( hasIntAndDouble == false );
    }

    THEN(
      "We should be able to retrieve references to the components" )
    {
      REQUIRE( entity.getComponent<int>() == 5 );
      REQUIRE( entity.getComponent<string>() == "hello" );

      entity.getComponent<string>() = "banana";

      REQUIRE( entity.getComponent<string>() == "banana" );
    }
  }

  GIVEN( "An two entities, one populated with components and moved "
         "into the second entity" )
  {
    Entity entity1{};
    entity1.addComponent( 5 ).addComponent( -4.0234 );

    Entity entity2{};
    entity2 = move( entity1 );

    THEN( "The moved-to entity should contain the components now" )
    {
      REQUIRE( entity2.hasComponents<int>() );
      REQUIRE( entity2.hasComponents<double>() );
    }

    THEN(
      "The moved-from entity should not contain the components now" )
    {
      REQUIRE( entity1.hasComponents<int>() == false );
      REQUIRE( entity1.hasComponents<double>() == false );
    }
  }
}

