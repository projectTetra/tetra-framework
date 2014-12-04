/**
 * /file Implementation file for the EntityManager's templated methods.
 **/

template <class... Types>
EntityManager::EntityList EntityManager::getEntities() noexcept
{
  EntityList entityList{};
  for ( auto& entry : entityMap )
  {
    Entity& entity = entry.second;
    if ( entity.hasComponents<Types...>() )
      entityList.emplace_back( entry.first, &entity );
  }

  return entityList;
}
