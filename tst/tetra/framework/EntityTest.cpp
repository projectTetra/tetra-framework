#include <tetra/framework/Entity.hpp>

#include <catch.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::framework;

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

