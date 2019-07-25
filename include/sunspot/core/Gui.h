#ifndef SST_GUI_H_
#define SST_GUI_H_

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

#include <functional>

#include <imgui/imgui.h>

namespace sunspot
{
class GlfwWindow;


class PYSPOT_EXPORT Gui
{
  public:
	void SetNextWindowPos( float x, float y, float px, float py ) const
	{
		::ImGui::SetNextWindowPos( { x, y }, 0, { px, py } );
	}
	void SetWindowBorderSize( float size ) const { ::ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, size ); }
	void SetFrameBorderSize( float size ) const { ::ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, size ); }
	void PopStyleVar( int count = 1 ) const { ::ImGui::PopStyleVar( count ); }
	void SetNextWindowBgAlpha( const float alpha ) const { ::ImGui::SetNextWindowBgAlpha( alpha ); }
	void Begin( const char* title ) const
	{
		::ImGui::Begin( title, nullptr,
		                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
		                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav );
	}
	void Text( const char* text ) const { ::ImGui::Text( "%s", text ); }
	void End() const { ::ImGui::End(); }
};


class ImGui
{
  public:
	ImGui( const GlfwWindow& g );
	~ImGui();
	void update( const float delta );

	void draw();

	std::function<void()> fPreDraw;

  private:
};


}  // namespace sunspot

#endif  // SST_GUI_H_
