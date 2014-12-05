#pragma once
#ifndef TETRA_FRAMEWORK_SERIALIZATIONSYSTEM_HPP
#define TETRA_FRAMEWORK_SERIALIZATIONSYSTEM_HPP

#include <tetra/meta/MetaRepository.hpp>
#include <tetra/framework/EntityManager.hpp>

namespace tetra
{
namespace framework
{

/**
 * The SerializationSystem is in charge of serializing and
 * deserializing all entities in an EntityManager.
 **/
class SerializationSystem
{
  const meta::MetaRepository& metaRepository;

public:
  SerializationSystem( const meta::MetaRepository& metaRepository );

  /**
   * Serializes the contents of the EntityManager to the output stream
   * provided.
   **/
  void serialize( EntityManager& entityManager,
                  std::ostream& out ) const;

  /**
   * Deserializes the contents of the EntityManager from the input
   * stream.
   **/
  void deserialize( EntityManager& entityManager,
                    std::istream& in ) const;
};

} /* namespace framework */
} /* namespace tetra */

#endif
