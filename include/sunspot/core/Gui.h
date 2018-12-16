#pragma once
#pragma warning(disable: 5030) // unknown attribute

#include <imgui/imgui.h>


namespace sunspot
{

class GlfwWindow;


class [[pyspot::export]] Gui
{
  public:

	void SetNextWindowPos( float x, float y, float px, float py ) const { ::ImGui::SetNextWindowPos( { x, y }, 0, { px, py } ); }
	void SetWindowBorderSize( float size ) const { ::ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, size ); }
	void SetFrameBorderSize( float size ) const { ::ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, size ); }
	void PopStyleVar( int count = 1 ) const { ::ImGui::PopStyleVar( count ); }
	void SetNextWindowBgAlpha( const float alpha ) const { ::ImGui::SetNextWindowBgAlpha( alpha ); }
	void Begin( const char* title ) const { ::ImGui::Begin( title, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ); }
	void Text( const char* text ) const { ::ImGui::Text( "%s", text ); }
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
