#pragma once
#ifndef TEST_VECTORCOMPONENT_HPP
#define TEST_VECTORCOMPONENT_HPP

#include <json/json.h>

namespace test
{

class VectorComponent
{
  float x, y, z;

public:
  VectorComponent() = default;
  inline VectorComponent( float x, float y, float z )
    : x{x}, y{y}, z{z} {};

  inline float getX() const { return x; } 
  inline float getY() const { return y; }
  inline float getZ() const { return z; }

  inline void setX(float x) { this->x = x; }
  inline void setY(float y) { this->y = y; }
  inline void setZ(float z) { this->z = z; }
};

// Serialize and Deserialize in the same namespace as VectorComponent
// so that ADL will find them.

inline 
bool serialize( const VectorComponent& vcomp, Json::Value& root )
{
  root["x"] = vcomp.getX();
  root["y"] = vcomp.getY();
  root["z"] = vcomp.getZ();
  return true;
}

inline 
bool deserialize( VectorComponent& vcomp, const Json::Value& root )
{
  vcomp.setX( root.get("x", 0.0f).asFloat() );
  vcomp.setY( root.get("y", 0.0f).asFloat() );
  vcomp.setZ( root.get("z", 0.0f).asFloat() );

  return true;
}

} /* namespace test */

#endif
