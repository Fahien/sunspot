#pragma once

#include <imgui/imgui.h>


namespace sunspot
{

class GlfwWindow;


class [[pyspot::export]] Gui
{
  public:

	void Begin( const char* title ) const { ::ImGui::Begin( title ); }
	void End() const { ::ImGui::End(); }
};


class ImGui
{
  public:
	ImGui( const GlfwWindow& );
	~ImGui();
	void Update( const float delta ) const;

	void Draw() const;
};


} // namespace sunspot
