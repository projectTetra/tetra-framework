#pragma once
#ifndef TETRA_FRAMEWORK_ENTITY_HPP
#define TETRA_FRAMEWORK_ENTITY_HPP

#include <tetra/meta/Variant.hpp>

#include <unordered_map>
#include <iostream>

namespace tetra
{
namespace framework
{

/**
 * Thrown by the Entity class when an operation is taken on a
 * component that doesn't exist.
 **/
class ComponentNotPresentException : public std::runtime_error
{
public:
  inline ComponentNotPresentException()
    : std::runtime_error{"Component not present"} {};
};

/**
 * An entity is essentially just a container for "components" which
 * are arbitrariy blobs of data held in Variants.
 **/
class Entity
{
public:
  using ComponentMap =
    std::unordered_map<const meta::MetaData*, meta::Variant>;

private:
  ComponentMap components;

public:
  Entity() = default;

  /**
   * Entities are not copyable because the Variants that hold the
   * components are not copyable.
   **/
  Entity( const Entity& ) = delete;
  Entity& operator=( const Entity& ) = delete;

  Entity( Entity&& ) noexcept = default;
  Entity& operator=( Entity&& ) noexcept = default;

  /**
   * Adds a component of the type specified, overwrites any components
   * of the same type already on this Entity.
   * @return *this
   **/
  template <class T>
  Entity& addComponent( T&& component ) noexcept;

  /**
   * Returns true if the entity contains all of the components
   * requested.
   **/
  template <class... ComponentTypes>
  bool hasComponents() const noexcept;

  /**
   * Retrieves the component with the requested type.
   * @throws ComponentNotPresent if Entity does not have the component
   * @templateParam ComponentType The type of component to retrieve.
   * @return A reference to the component.
   **/
  template <class ComponentType>
  ComponentType& getComponent() const;
};

#include <tetra/framework/entity/EntityImpl.hpp>
} /* namespace framework */
} /* namespace tetra */

#endif
