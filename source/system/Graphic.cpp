#include "sunspot/system/Graphic.h"

#include "sunspot/component/Camera.h"

namespace sunspot::graphic
{

void System::Draw()
{
	glEnable( GL_DEPTH_TEST );
	glViewport( m_Viewport.origin.x, m_Viewport.origin.y, m_Viewport.size.width, m_Viewport.size.height );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear color and depth buffer
	m_pProgram->Use();
	m_pLight->Update( *m_pProgram );

	m_pCamera->Get<component::Camera>()->get().Update( *m_pProgram );

	for ( auto pModel : m_Models )
	{
		pModel->GetRenderer().Draw( *m_pProgram, &pModel->GetNode() );
	}
}

} // namespace sunspot::system
