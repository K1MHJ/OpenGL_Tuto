#include "Test.hpp"
#include "imgui/imgui.h"

namespace test{
}
test::TestMenu::TestMenu(Test*& currentTestPointer)
  : m_CurrentTest(currentTestPointer)
{

}
test::TestMenu::~TestMenu()
{

}
void test::TestMenu::OnImGuiRender()
{
  for(auto& test : m_Tests)
  {
    if(ImGui::Button(test.first.c_str()))
      m_CurrentTest = test.second();
  }
}
