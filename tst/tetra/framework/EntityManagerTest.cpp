#include <tetra/framework/EntityManager.hpp>

#include <catch.hpp>
#include <test/Widget.hpp>
#include <test/VectorComponent.hpp>

using namespace std;
using namespace test;
using namespace tetra;
using namespace tetra::meta;
using namespace tetra::framework;

SCENARIO( "Using an EntityManager to manage a set of entities",
          "[EntityManager]" )
{
  GIVEN( "An empty EntityManager" )
  {
    EntityManager entityManager{};

    THEN( "We should be able to create a new Entity" )
    {
      auto id = entityManager.createEntity();

      Entity& entity = entityManager.getEntity( id );
    }

    THEN( "Attempting to access an entity which does not exist "
          "should throw an EntityDoesNotExistException" )
    {
      REQUIRE_THROWS_AS( entityManager.getEntity( 5 ),
                         EntityDoesNotExistException );
    }

    THEN( "We should be able to get a list of all existing entities "
          "and access them" )
    {
      auto id1 = entityManager.createEntity();
      auto id2 = entityManager.createEntity();
      auto entityList = entityManager.getAllEntities();

      REQUIRE( entityList.size() == 2 );

      for( const auto& entityDescriptor : entityList )
      {
        // The entities in the list should match their ids
        auto entityId = entityDescriptor.entityId;
        REQUIRE( &entityManager.getEntity( entityId ) ==
                 entityDescriptor.entity );
      }
    }

    THEN( "We should be able to destroy entities by posting destroy "
          "entity messages" )
    {
      auto id1 = entityManager.createEntity();
      auto id2 = entityManager.createEntity();
      auto id3 = entityManager.createEntity();
      REQUIRE( entityManager.getAllEntities().size() == 3 );

      entityManager.postDestroyEntityMessage( id2 );
      REQUIRE( entityManager.getAllEntities().size() == 3 );

      entityManager.update();

      auto allEntitiesAfterUpdate = entityManager.getAllEntities();
      REQUIRE( allEntitiesAfterUpdate.size() == 2 );

      bool id1Present = false, id3Present = false;
      for ( const auto& entityDesc : allEntitiesAfterUpdate )
      {
        if ( entityDesc.entityId == id1 ) id1Present = !id1Present;
        if ( entityDesc.entityId == id3 ) id3Present = !id3Present;
      }
      REQUIRE( id1Present );
      REQUIRE( id3Present );
    }

  }

  GIVEN( "An EntityManager that has entities with widget and vector "
         "components" )
  {
    EntityManager entityManager{};
    /** Populate the entity manager with entities **/
    auto createEntityWithWidgetAndVector =
      [&]( float x, float y, float z ) -> EntityManager::EntityId
    {
      auto id = entityManager.createEntity();
      entityManager.getEntity( id )
        .addComponent( Widget{} )
        .addComponent( VectorComponent{x, y, z} );
      return id;
    };

    auto createEntityWithOnlyWidget = [&]() -> EntityManager::EntityId
    {
      auto id = entityManager.createEntity();
      entityManager.getEntity( id ).addComponent( Widget{} );
      return id;
    };

    auto createEntityWithOnlyVector =
      [&]( float x, float y, float z ) -> EntityManager::EntityId
    {
      auto id = entityManager.createEntity();
      entityManager.getEntity( id )
        .addComponent( VectorComponent{x, y, z} );
      return id;
    };

    auto entitiesWithWidgetsAndVectors =
      vector<EntityManager::EntityId>{};
    for ( int i = 0; i < 10; ++i )
      entitiesWithWidgetsAndVectors.push_back(
        createEntityWithWidgetAndVector( 1.0f, 2.0f, 3.0f ) );

    auto entitiesWithOnlyWidgets = vector<EntityManager::EntityId>{};
    for ( int i = 0; i < 6; ++i )
      entitiesWithOnlyWidgets.push_back(
        createEntityWithOnlyWidget() );

    auto entitiesWithOnlyVectors = vector<EntityManager::EntityId>{};
    for ( int i = 0; i < 7; ++i )
      entitiesWithOnlyVectors.push_back(
        createEntityWithOnlyVector( 3.0f, 2.0f, 1.0f ) );

    /** entity manager populated, now lets to the checking **/
    int totalSize = entitiesWithWidgetsAndVectors.size() +
                    entitiesWithOnlyWidgets.size() +
                    entitiesWithOnlyVectors.size();
    REQUIRE( entityManager.getAllEntities().size() == totalSize );

    THEN( "We should be able to get all entities with both Widget "
          "and VectorComponents" )
    {
      auto entityList =
        entityManager.getEntities<Widget, VectorComponent>();
      REQUIRE( entityList.size() ==
               entitiesWithWidgetsAndVectors.size() );
    }

    THEN(
      "We should be able to get all entities with at least a Widget" )
    {
      REQUIRE( entityManager.getEntities<Widget>().size() ==
               entitiesWithWidgetsAndVectors.size() +
                 entitiesWithOnlyWidgets.size() );
    }

    THEN( "We should be able to get all entities with at least a "
          "VectorComponent" )
    {
      REQUIRE( entityManager.getEntities<VectorComponent>().size() ==
               entitiesWithWidgetsAndVectors.size() +
                 entitiesWithOnlyVectors.size() );
    }

    THEN( "calling getEntities with a type that no entities have "
          "should return zero entities" )
    {
      REQUIRE( entityManager.getEntities<string>().size() == 0 );
    }
  }
}

