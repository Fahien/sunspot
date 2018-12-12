#include "sunspot/core/Gui.h"

#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

#include "sunspot/core/GlfwWindow.h"

#include <pyspot/Module.h>
#include <pyspot/Wrapper.h>


namespace sunspot
{


ImGui::ImGui( const GlfwWindow& window )
{
	auto result = ::IMGUI_CHECKVERSION();
	assert( result && "ImGui check version failed" );

	::ImGui::CreateContext();
	assert( result && "Cannot create ImGui Context" );

	ImGui_ImplGlfw_InitForOpenGL( window.GetHandle(), true );
	assert( result && "Cannot init ImGui for Glfw" );

	ImGui_ImplOpenGL3_Init();
	assert( result && "Cannot init ImGui for OpenGL 3" );
}


ImGui::~ImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	::ImGui::DestroyContext();
}


void ImGui::Update(const float delta ) const
{
	auto& io = ::ImGui::GetIO();
	io.DeltaTime = delta;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	::ImGui::NewFrame();
}


void ImGui::Draw() const
{
	::ImGui::ShowTestWindow();

	::ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData( ::ImGui::GetDrawData() );
}


} // namespace sunspot
