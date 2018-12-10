#include "sunspot/core/Gui.h"

#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include "sunspot/core/GlfwWindow.h"

namespace sunspot
{


Gui::Gui( const GlfwWindow& window )
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL( window.GetHandle(), true );
	ImGui_ImplOpenGL3_Init( window.GetGlslVersion().c_str() );
}


Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void Gui::NewFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}


void Gui::Render() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}

} // namespace sunspot
