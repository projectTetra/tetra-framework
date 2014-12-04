#pragma once
#ifndef TETRA_META_TEST_WIDGET_HPP
#define TETRA_META_TEST_WIDGET_HPP

#include <string>

namespace test
{

/**
 * Used by the various MetaData tests.
 **/
class Widget
{
  static int instanceCount;
  
  std::string myName; 

public:
  static int getInstanceCount() noexcept;

public:
  Widget();
  ~Widget();

  const std::string& getMyName() const noexcept;
};

} /* namespace test */

#endif
