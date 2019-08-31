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

	ImGui_ImplGlfw_InitForOpenGL( window.get_handle(), true );
	assert( result && "Cannot init ImGui for Glfw" );

	ImGui_ImplOpenGL3_Init( "#version 150" );
	assert( result && "Cannot init ImGui for OpenGL 3" );
}


ImGui::~ImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	::ImGui::DestroyContext();
}


void ImGui::update( const float delta )
{
	auto& io     = ::ImGui::GetIO();
	io.DeltaTime = delta;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	::ImGui::NewFrame();
}


void ImGui::draw()
{
	if ( fPreDraw )
	{
		fPreDraw();
	}

	::ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData() );
}


}  // namespace sunspot
