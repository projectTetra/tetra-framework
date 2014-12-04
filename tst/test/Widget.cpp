#include <test/Widget.hpp>

using namespace test;

int Widget::instanceCount{0};

Widget::Widget()
{
  ++instanceCount;
  this->myName = "Widget{" + std::to_string( instanceCount ) + "}";
}

Widget::~Widget()
{
  --instanceCount;
}

const std::string& Widget::getMyName() const noexcept
{
  return this->myName;
}

int Widget::getInstanceCount() noexcept
{
  return instanceCount;
}
