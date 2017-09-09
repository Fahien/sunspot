#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

#include "Logger.h"
#include "WavefrontObject.h"


using namespace sunspot;
namespace mst = mathspot;


/// Creates a Wavefront Object
WavefrontObject::WavefrontObject()
	: mName{}
	, mCurrentGroupName{ "default" }
	, mPositionCount{ 0 }
	, mNormalCount{ 0 }
	, mTexCoordsCount{ 0 }
	, mVertexCount{ 0 }
	, mPositions{}
	, mTexCoords{}
	, mNormals{}
	, mVertices{}
	, mIndices{}
	, mTextures{}
	, mMeshes{}
	, mCurrentMaterial{ nullptr }
	, mMaterials{}
{
	Logger::log.info("WavefrontObject: created\n"); // TODO remove debug log
}


/// Release resources
WavefrontObject::~WavefrontObject()
{
	for (Mesh* mesh : mMeshes)
	{
		delete mesh;
	}
	Logger::log.info("WavefrontObject: destroyed %s\n", mName.c_str()); // TODO remove debug log
}


/// Draw the object
void WavefrontObject::draw(const ShaderProgram& shader) const
{
	for (Mesh *mesh : mMeshes)
	{
		mesh->draw(shader);
	}
}


/// Loads the object name
void WavefrontObject::loadName(std::stringstream& ss)
{
	std::string command{};
	ss >> command >> mName;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading name" };
	}
	Logger::log.info("Obj name: %s\n", mName.c_str()); // TODO remove debug Logger::log
}


/// Loads a vertex position from a string stream
void WavefrontObject::loadPosition(std::stringstream& ss)
{
	std::string command{};
	mst::Vec3 v{};
	ss >> command >> v.x >> v.y >> v.z;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading vertex position" };
	}
	float w;
	ss >> w;
	if (!ss.fail()) // Deomogenize
	{
		v.x /= w;
		v.y /= w;
		v.z /= w;
	}
	mPositions.push_back(v);
	++mPositionCount;
}


/// Loads a texture coordinate from a string stream
void WavefrontObject::loadTexCoords(std::stringstream& ss)
{
	std::string command{};
	mst::Vec2 t{};
	ss >> command >> t.x;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading texture coordinate" };
	}
	ss >> t.y;
	if (ss.fail())
	{
		t.y = 0.0f; // Default to 0
	}
	mTexCoords.push_back(t);
	++mTexCoordsCount;
}


/// Loads a vertex normal from a string stream
void WavefrontObject::loadNormal(std::stringstream &ss)
{
	std::string command{};
	mst::Vec3 n{};
	ss >> command >> n.x >> n.y >> n.z;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading vertex normal" };
	}
	mNormals.push_back(n);
	++mNormalCount;
}


template <char C>
std::istream& expect(std::istream &is)
{
	if ((is >> std::ws).peek() == C)
	{
		is.ignore();
	}
	else
	{
		is.setstate(std::ios_base::failbit);
	}
	return is;
}


/// Loads a face from a string stream
void WavefrontObject::loadIndices(std::stringstream& ss)
{
	std::string command{};
	Face f{};
	ss >> command >> f.indices[0];
	if (ss.fail())
	{
		throw LoadingException{ "Error loading indices" };
	}
	char next{ static_cast<char>(ss.peek()) };
	if (next == ' ') // Only faces
	{
		ss >> f.indices[1] >> f.indices[2];
		if (ss.fail())
		{
			throw LoadingException{ "Error loading indices" };
		}
		ss >> f.indices[3];
		if (ss.fail()) // Default to invalid value
		{
			f.indices[3] = 0;
		}
	}
	else if (next == '/')
	{
		ss.ignore();
		next = ss.peek();
		if (next == '/') // Only vertices and normals
		{
			ss.ignore();
			ss >> f.normals[0]
			   >> f.indices[1] >> expect<'/'> >> expect<'/'> >> f.normals[1]
			   >> f.indices[2] >> expect<'/'> >> expect<'/'> >> f.normals[2];
			if (ss.fail())
			{
				throw LoadingException{ "Error loading indices" };
			}
			ss >> f.indices[3] >> expect<'/'> >> expect<'/'> >> f.normals[3];
			if (ss.fail()) // Default to invalid value
			{
				f.indices[3] = 0;
			}
		}
		else // Vertices, coordinates and (normals)
		{
			ss >> f.textures[0];
			next = ss.peek();
			if (next == ' ') // Only vertices and coordinates
			{
				ss >> f.indices[1] >> expect<'/'> >> f.textures[1]
				   >> f.indices[2] >> expect<'/'> >> f.textures[2];
				if (ss.fail())
				{
					throw LoadingException{ "Error loading indices" };
				}
				ss >> f.indices[3] >> expect<'/'> >> f.textures[3];
				if (ss.fail()) // Default to invalid value
				{
					f.indices[3] = 0;
				}
			}
			else // Vertices, coordinates and normals
			{
				ss >> expect<'/'> >> f.normals[0]
				   >> f.indices[1] >> expect<'/'> >> f.textures[1] >> expect<'/'> >> f.normals[1]
				   >> f.indices[2] >> expect<'/'> >> f.textures[2] >> expect<'/'> >> f.normals[2];
				if (ss.fail())
				{
					throw LoadingException{ "Error loading indices" };
				}
				ss >> f.indices[3] >> expect<'/'> >> f.textures[3] >> expect<'/'> >> f.normals[3];
				if (ss.fail()) // Default to invalid value
				{
					f.indices[3] = 0;
				}
			}
		}
	}
	// Pick only first 3 indices, ignoring all the other values. -1 cause obj starts from 1
	for (int i{ 0 }; i < 3; ++i)
	{
		if (f.indices[i] == 0)
		{
			throw LoadingException{ "Invalid index" };
		}
		Vertex v{};
		// Recalculate index and put position in vertex
		unsigned positionIndex{ (f.indices[i] > 0) ? (f.indices[i] - 1) : (mPositionCount + f.indices[i]) };
		v.position = mPositions[positionIndex];
		// Put texCoords in vertex
		unsigned texCoordsIndex{ f.textures[i] > 0 ? (f.textures[i] - 1) : (mTexCoordsCount + f.indices[i]) };
		if (texCoordsIndex < mTexCoordsCount)
		{
			v.texCoords = mTexCoords[texCoordsIndex];
		}
		// Put normal in vertex
		unsigned normalIndex{ f.normals[i] > 0 ? (f.normals[i] - 1) : (mNormalCount + f.indices[i]) };
		if (normalIndex < mNormalCount)
		{
			v.normal = mNormals[normalIndex];
		}
		mIndices.push_back(mVertexCount++);
		mVertices.push_back(v);
	}
}


/// Loads a group
void WavefrontObject::loadGroup(std::stringstream& ss)
{
	loadCachedMesh();
	std::string command{};
	mCurrentGroupName = "default"; // Default name is default
	ss >> command >> mCurrentGroupName;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading group name" };
	}
}


/// Loads cached mesh
void WavefrontObject::loadCachedMesh()
{
	if (!mVertices.empty() && !mIndices.empty())
	{
		mMeshes.push_back( new Mesh{ mCurrentGroupName, mVertices, mIndices, mCurrentMaterial });
		mVertices.clear();
		mVertexCount = 0;
		mIndices.clear();
	}
}


/// Loads ambient reflectivity
void WavefrontObject::loadAmbient(std::stringstream& ss)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "Ka")
	{
		throw LoadingException{ "Error reading Ka command" };
	}
	// Load the color into the current material
	ss >> mCurrentMaterial->ambient.r >> mCurrentMaterial->ambient.g >> mCurrentMaterial->ambient.b;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading material ambient" };
	}
}


/// Loads diffuse reflectivity
void WavefrontObject::loadDiffuse(std::stringstream& ss)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "Kd")
	{
		throw LoadingException{ "Error loading Kd command" };
	}
	// Load the color into the current material
	ss >> mCurrentMaterial->diffuse.r >> mCurrentMaterial->diffuse.g >> mCurrentMaterial->diffuse.b;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading material diffuse" };
	}
}


/// Loads specular reflectivity
void WavefrontObject::loadSpecular(std::stringstream& ss)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "Ks")
	{
		throw LoadingException{ "Error loading Ks command" };
	}
	// Load the color into the current material
	ss >> mCurrentMaterial->specular.r >> mCurrentMaterial->specular.g >> mCurrentMaterial->specular.b;
	if (ss.fail())
	{
		throw LoadingException{ "Error loading material specular" };
	}
}


/// Loads ambient map
void WavefrontObject::loadAmbientMap(std::stringstream& ss, const std::string& /* path */)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "map_Ka")
	{
		throw LoadingException{ "Error reading map_Ka command" };
	}
	// Do nothing
}


/// Loads diffuse map
void WavefrontObject::loadDiffuseMap(std::stringstream& ss, const std::string& path)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "map_Kd")
	{
		throw LoadingException{ "Error loading map_Kd command" };
	}

	std::string textureName{};
	ss >> textureName; // Load the texture name
	if (ss.fail())
	{
		throw LoadingException{ "Error loading diffuse map name" };
	}

	Logger::log.info("WavefrontObject: Loading %s/%s\n", path.c_str(), textureName.c_str()); // TODO remove debug Logger::log
	Texture texture{ path
#ifndef WIN32
		+ '/'
#endif
		+ textureName, TextureType::DIFFUSE };
	mCurrentMaterial->diffuseMap    = texture.getId();
	mCurrentMaterial->hasDiffuseMap = true;
}


/// Loads specular map
void WavefrontObject::loadSpecularMap(std::stringstream& ss, const std::string& path)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "map_Ks")
	{
		throw LoadingException{ "Error loading map_Ks command" };
	}

	std::string textureName{};
	ss >> textureName; // Load the texture name
	if (ss.fail())
	{
		throw LoadingException{ "Error loading specular map name" };
	}

	Texture texture {path
#ifndef WIN32
		+ '/'
#endif
		+ textureName, TextureType::SPECULAR};
	mCurrentMaterial->specularMap    = texture.getId();
	mCurrentMaterial->hasSpecularMap = true;
}


/// Loads cached material
void WavefrontObject::loadCachedMaterial()
{
	if (mCurrentMaterial == nullptr)
	{
		throw LoadingException{ "Current material is null" };
	}
	mMaterials.push_back(mCurrentMaterial);
	mCurrentMaterial = nullptr;
}


/// Creates a new material
void WavefrontObject::createMaterial(std::stringstream& ss)
{
	if (mCurrentMaterial != nullptr)
	{
		loadCachedMaterial();
	}

	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "newmtl")
	{
		throw LoadingException{ "Error reading newmtl command" };
	}

	std::string name{}; // Read material name
	ss >> name;
	if (ss.fail())
	{
		throw LoadingException{ "Error reading material name" };
	}
	// Create new current material
	if (mCurrentMaterial != nullptr)
	{
		throw LoadingException{ "Current material is not null" };
	}
	mCurrentMaterial = new Material{ name };
}


/// Read materials from material library
void WavefrontObject::loadMaterials(Ifstream& is)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (!is.IsEof())
	{
		line = is.GetLine();
		if (line.length() <= 0) // Ignore empty lines
		{
			continue;
		}
		std::stringstream ss{ line };
		switch (line[0])
		{
		case '#': { break; } // Ignore comment
		case 'n': { // New material command
			try
			{
				createMaterial(ss);
			}
			catch (const LoadingException &e)
			{
				Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
			}
			break;
		}
		case 'K': { // K statement
			if (line.length() <= 1)
			{
				Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
				break;
			}
			switch (line[1]) {
			case 'a': { // Ambient
				try
				{
					loadAmbient(ss);
				}
				catch (const LoadingException &e)
				{
					Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
				}
				break;
			}
			case 'd': { // Diffuse
				try
				{
					loadDiffuse(ss);
				}
				catch (const LoadingException &e)
				{
					Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
				}
				break;
			}
			case 's': { // Specular
				try
				{
					loadSpecular(ss);
				}
				catch (const LoadingException &e)
				{
					Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
				}
				break;
			}
			default: {
				Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
			}
			} // End switch line[1]
			break;
		}
		case 'm': { // Map statement
			if (line.length() <= 6)
			{
				Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
				break;
			}
			switch (line[5])
			{
			case 'a': { // Ambient map
				try
				{
					loadAmbientMap(ss, is.getPath());
				}
				catch (const LoadingException &e)
				{
					Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
				}
				break;
			}
			case 'd': { // Diffuse map
				try
				{
					loadDiffuseMap(ss, is.getPath());
				}
				catch (const LoadingException &e)
				{
					Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
				}
				break;
			}
			case 's': { // Specular map
				try
				{
					loadSpecularMap(ss, is.getPath());
				}
				catch (const LoadingException &e)
				{
					Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
				}
				break;
			}
			default: {
				Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
				break;
			}
			} // End switch line[5]
			break;
		}
		default: {
			Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
			break;
		}
		} // End switch line[0]
		++lineNumber;
	}
	try
	{
		loadCachedMaterial();
	}
	catch (const LoadingException& e)
	{
		Logger::log.error("WavefrontObject: No materials loaded, %s\n", e.what());
	}
}


/// Loads material library
void WavefrontObject::loadMaterialLibrary(std::stringstream& ss, const std::string& path)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "mtllib")
	{
		throw LoadingException{ "Error reading mtllib command" };
	}
	
	std::string mtlName{}; // Read mtl file
	ss >> mtlName;
	if (ss.fail())
	{
		throw LoadingException{ "Error reading mtl name" };
	}
	Ifstream is{ path
#ifndef WIN32
		+ '/'
#endif
		+ mtlName };
	if (!is.IsOpen())
	{
		throw LoadingException{ "Could not find mtl file: " + path
#ifndef WIN32
			+ '/'
#endif
			+ mtlName };
	}
	loadMaterials(is);

	while (!ss.fail()) // Read other optional mtl
	{
		ss >> mtlName;
		if (!ss.fail())
		{
			Ifstream is{ path
#ifndef WIN32
				+ '/'
#endif
				+ mtlName };
			if (!is.IsOpen())
			{
				continue;
			}
			loadMaterials(is);
		}
	}
}


/// Uses a material
void WavefrontObject::useMaterial(std::stringstream& ss)
{
	std::string command{}; // Read command
	ss >> command;
	if (ss.fail() || command != "usemtl")
	{
		throw LoadingException{ "Error reading usemtl command"};
	}

	std::string materialName{}; // Read material name
	ss >> materialName;
	if (ss.fail())
	{
		throw LoadingException{ "Error reading material name" };
	}

	for (Material* m : mMaterials)
	{
		if (m->name == materialName)
		{
			mCurrentMaterial = m;
		}
	}
}


/// Reads a Wavefront Object
Ifstream &sunspot::operator>>(Ifstream& is, WavefrontObject& obj)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (!is.IsEof())
	{
		line = is.GetLine();
		if (line.length() <= 0) // Ignore empty lines
		{
			continue;
		}
		std::stringstream ss{ line };
		switch (line[0])
		{
		case '#': { break; } // Ignore comment
		case 'o': { // Name command
			try
			{
				obj.loadName(ss);
			}
			catch (const LoadingException& e)
			{
				Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
			}
			break;
		}
		case 'v': {
			if (line.length() < 2)
			{
				Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
				break;
			}
			switch (line[1])
			{
				case ' ': { // Vertex command
					try
					{
						obj.loadPosition(ss);
					}
					catch (const LoadingException& e)
					{
						Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
					}
					break;
				}
				case 'p': {
					Logger::log.info("Point in the parameter space of a curve or a surface not supported\n");
					break;
				}
				case 'n': { // Vertex Normal command
					try
					{
						obj.loadNormal(ss);
					}
					catch (const LoadingException& e)
					{
						Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
					}
					break;
				}
				case 't': { // Texture Coordinate command
					try
					{
						obj.loadTexCoords(ss);
					}
					catch (const LoadingException& e)
					{
						Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
					}
					break;
				}
			}
			break;
		}
		case 'f': { // Face command
			try
			{
				obj.loadIndices(ss);
			}
			catch (const LoadingException& e)
			{
				Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
			}
			break;
		}
		case 'g': { // Group command
			try
			{
				obj.loadGroup(ss);
			}
			catch (const LoadingException& e)
			{
				Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
			}
			break;
		}
		case 'm': { // Material Library Command
			try
			{
				obj.loadMaterialLibrary(ss, is.getPath());
			}
			catch (const LoadingException& e)
			{
				Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
			}
			break;
		}
		case 'u': { // Use material command
			try
			{
				obj.useMaterial(ss);
			}
			catch (const LoadingException& e)
			{
				Logger::log.error("[%d] error: %s\n", lineNumber, e.what());
			}
			break;
		}
		default: {
			Logger::log.error("[%d] ignored: %s\n", lineNumber, line.c_str());
			break;
		}}
		++lineNumber;
	}
	obj.loadCachedMesh();
	return is;
}
