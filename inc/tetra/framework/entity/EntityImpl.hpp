template <class T, class... Types>
struct ComponentUnwinder
{
  static bool
  hasComponents( const Entity::ComponentMap& map ) noexcept
  {
    return ComponentUnwinder<T>::hasComponents( map ) &&
           ComponentUnwinder<Types...>::hasComponents( map );
  }
};

template <class T>
struct ComponentUnwinder<T>
{
  static bool
  hasComponents( const Entity::ComponentMap& map ) noexcept
  {
    return ( map.find( &meta::MetaData::create<T>() ) != map.end() );
  }
};

template <class T>
T& Entity::getComponent() const 
{
  auto iter = components.find( &meta::MetaData::create<T>() );
  if (iter == std::end(components)) 
    throw ComponentNotPresentException{};

  const meta::Variant& compVariant = iter->second;
  return compVariant.getObject<T>();
}

template <class T>
Entity& Entity::addComponent( T&& component ) noexcept
{
  auto variant =
    meta::Variant::create( std::forward<T&&>( component ) );

  return addComponent( std::move( variant ) );
}

template <class T>
void Entity::removeComponent()
{
  auto iter = components.find( &meta::MetaData::create<T>() );
  if ( iter != std::end( components ) ) components.erase( iter );
}

template <class... ComponentTypes>
bool Entity::hasComponents() const noexcept
{
  return ComponentUnwinder<ComponentTypes...>::hasComponents(
    components );
}
