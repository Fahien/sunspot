#include "sunspot/editor/Editor.h"

#include "sunspot/core/Gui.h"

namespace sunspot
{
void Editor::draw_transform( gltfspot::Node& node )
{
	using namespace ImGui;

	if ( TreeNodeEx( &node.translation, 0, "Transform" ) )
	{
		DragFloat3( "Position", &node.translation.x, 0.25f, -32.0f, 32.0f, "%.1f" );
		DragFloat4( "Rotation", &node.rotation.x, 0.125f, -1.0f, 1.0f, "%.2f" );
		DragFloat3( "Scale", &node.scale.x, 0.05f, 0.05f, 4.0f, "%.2f" );
		TreePop();
	}
}


void Editor::draw( gltfspot::Camera& camera )
{
	using namespace ImGui;

	if ( TreeNodeEx( &camera, 0, "Camera" ) )
	{
		if ( camera.type == gltfspot::Camera::Type::Perspective )
		{
			DragFloat( "Y Fov", &camera.perspective.yfov, 0.125f, 0.0f, 3.25f, "%.2f" );
			DragFloat( "Aspect Ratio", &camera.perspective.aspect_ratio, 0.125f, 0.125f, 2.0f, "%.3f" );
			DragFloat( "Z Near", &camera.perspective.znear, 0.125f, 0.125f, 1.0f, "%.3f" );
			DragFloat( "Z Far", &camera.perspective.zfar, 1.0f, 2.0f, 256.0f, "%.0f" );
		}
		else
		{
			DragFloat( "X Mag", &camera.orthographic.xmag, 0.125f, 0.0f, 2.0f, "%.3f" );
			DragFloat( "Y Mag", &camera.orthographic.ymag, 0.125f, 0.0f, 2.0f, "%.3f" );
			DragFloat( "Z Near", &camera.orthographic.znear, 0.125f, 0.125f, 1.0f, "%.3f" );
			DragFloat( "Z Far", &camera.orthographic.zfar, 1.0f, 2.0f, 256.0f, "%.0f" );
		}

		TreePop();
	}
}


void Editor::draw( gltfspot::Light& light )
{
	using namespace ImGui;

	if ( TreeNodeEx( &light, 0, "Light" ) )
	{
		DragFloat3( "Color", &light.color.x, 0.125f, 0.0f, 32.0f, "%.3f" );
		TreePop();
	}
}


void Editor::draw( gltfspot::Mesh& mesh )
{
	using namespace ImGui;

	if ( TreeNodeEx( &mesh, 0, "Mesh" ) )
	{
		for ( auto& primitive : mesh.primitives )
		{
			if ( TreeNodeEx( &primitive, 0, "%s", gltfspot::to_string( primitive.mode ).c_str() ) )
			{
				for ( auto& attribute : primitive.attributes )
				{
					Text( "%s", gltfspot::to_string( attribute.first ).c_str() );
				}

				TreePop();
			}
		}
		TreePop();
	}
}


void Editor::draw( gltfspot::Node& node )
{
	using namespace ImGui;

	ImGuiTreeNodeFlags flags = 0;  // ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if ( selected == &node )
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	auto node_open = TreeNodeEx( &node, flags, "%s", node.name.c_str() );

	// Drag source
	if ( BeginDragDropSource( ImGuiDragDropFlags_None ) )
	{
		auto node_address = &node;
		SetDragDropPayload( "GLTF_NODE", &node_address, sizeof( node_address ) );
		Text( "Move %s", node.name.c_str() );
		EndDragDropSource();
	}

	// Drag target
	if ( BeginDragDropTarget() )
	{
		if ( auto payload = AcceptDragDropPayload( "GLTF_NODE" ) )
		{
			IM_ASSERT( payload->DataSize == sizeof( &node ) );
			auto payload_node = *reinterpret_cast<gltfspot::Node**>( payload->Data );
			payload_node->remove_from_parent();

			// Add payload node to new parent
			payload_node->parent = &node;
			node.children_indices.push_back( payload_node->index );
			node.children.push_back( payload_node );
		}
		EndDragDropTarget();
	}

	if ( IsItemClicked() )
	{
		selected = &node;
	}

	if ( node_open )
	{
		draw_transform( node );

		if ( node.camera )
		{
			draw( *node.camera );
		}

		if ( node.light )
		{
			draw( *node.light );
		}

		if ( node.mesh )
		{
			draw( *node.mesh );
		}

		// Children
		if ( !node.children.empty() )
		{
			if ( TreeNodeEx( &node.children, 0, "Children" ) )
			{
				for ( auto child : node.children )
				{
					draw( *child );
				}
				TreePop();
			}
		}

		TreePop();  // Node name
	}
}


void Editor::draw( gltfspot::Gltf& gltf )
{
	using namespace ImGui;

	// Update current gltf
	this->gltf = &gltf;

	// Scene
	SetNextWindowPos( ImVec2( 0, 0 ) );
	PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
	ImGuiWindowFlags scene_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove |
	                               ImGuiWindowFlags_AlwaysAutoResize;
	Begin( "Editor", nullptr, scene_flags );
	PopStyleVar();

	auto& scene = *gltf.GetScene();

	// Editor menu
	if ( BeginMenuBar() )
	{
		if ( BeginMenu( "Edit" ) )
		{
			if ( BeginMenu( "New" ) )
			{
				if ( MenuItem( "Node" ) )
				{
					if ( !selected || selected == &scene )
					{
						scene.create_node( "New" );
					}
					else
					{
						auto selected_node = reinterpret_cast<gltfspot::Node*>( selected );
						selected_node->create_child( "New" );
					}

					selected = nullptr;
				}
				EndMenu();
			}
			EndMenu();
		}
		EndMenuBar();
	}

	PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );

	// Scene node
	if ( TreeNodeEx( &scene, 0, "%s", scene.name.c_str() ) )
	{
		// Drag target
		if ( BeginDragDropTarget() )
		{
			if ( auto payload = AcceptDragDropPayload( "GLTF_NODE" ) )
			{
				IM_ASSERT( payload->DataSize == sizeof( void* ) );
				auto payload_node = *reinterpret_cast<gltfspot::Node**>( payload->Data );
				payload_node->remove_from_parent();

				// Add payload node to new parent (scene)
				scene.nodes_indices.push_back( payload_node->index );
				scene.nodes.push_back( payload_node );
			}
			EndDragDropTarget();
		}

		if ( IsItemClicked() )
		{
			selected = &scene;
		}

		for ( auto& node : scene.nodes )
		{
			draw( *node );
		}
		TreePop();  // Scene
	}


	PopStyleVar();

	End();  // Scene
}

}  // namespace sunspot