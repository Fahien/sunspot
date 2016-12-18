#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

#include "WavefrontObject.h"
#include "Math.h"
#include "Mesh.h"


using namespace sunspot;


/// Creates a Wavefront Object
WavefrontObject::WavefrontObject()
	: name_{}
	, currentGroupName_{ "default" }
	, positionCount_{ 0 }
	, normalCount_{ 0 }
	, texCoordsCount_{ 0 }
	, vertices_{}
	, indices_{}
	, textures_{}
	, materials_{}
	, meshes_{}
{
	std::cout << "WavefrontObject: created\n"; // TODO remove debug log
}


/// Release resources
WavefrontObject::~WavefrontObject()
{
	for (Mesh *mesh : meshes_) { delete mesh; }
	std::cout << "WavefrontObject: destroyed\n"; // TODO remove debug log
}


/// Loads the object name
void WavefrontObject::loadName(std::stringstream &ss)
{
	std::string command{};
	ss >> command >> name_;
	if (ss.fail()) { throw LoadingException{ "Error loading name" }; }
	std::cout << "Obj name: " << name_ << std::endl; // TODO remove debug log
}


/// Loads a vertex position from a string stream
void WavefrontObject::loadPosition(std::stringstream &ss)
{
	std::string command{};
	math::Vec3 v{};
	ss >> command >> v.x >> v.y >> v.z;
	if (ss.fail()) { throw LoadingException{ "Error loading vertex" }; }
	float w;
	ss >> w;
	if (!ss.fail()) { // Deomogenize
		v.x /= w;
		v.y /= w;
		v.z /= w;
	}
	if(positionCount_ >= vertices_.size()) { vertices_.push_back(Vertex{}); } // Add a vertex
	vertices_[positionCount_].position = v; // Update new vertex
	++positionCount_; // TODO remove?
}


/// Loads a texture coordinate from a string stream
void WavefrontObject::loadTexCoords(std::stringstream &ss)
{
	std::string command{};
	math::Vec2 t{};
	ss >> command >> t.x;
	if (ss.fail()) { throw LoadingException{ "Error loading texture coordinate" }; }
	ss >> t.y;
	if (ss.fail()) { t.y = 0.0f; } // Default to 0
	texCoords_.push_back(t);
	++texCoordsCount_; // TODO remove?
}


/// Loads a vertex normal from a string stream
void WavefrontObject::loadNormal(std::stringstream &ss)
{
	std::string command{};
	math::Vec3 n{};
	ss >> command >> n.x >> n.y >> n.z;
	if (ss.fail()) { throw LoadingException{ "Error loading vertex normal" }; }
	normals_.push_back(n);
	++normalCount_; // TODO remove?
}


template <char C>
std::istream& expect(std::istream& is)
{
	if ((is >> std::ws).peek() == C) { is.ignore(); }
	else { is.setstate(std::ios_base::failbit); }
	return is;
}


/// Loads a face from a string stream
void WavefrontObject::loadIndices(std::stringstream &ss)
{
	std::string command{};
	Face f{};
	ss >> command >> f.indices[0];
	if (ss.fail()) { throw LoadingException{ "Error loading indices" }; }
	char next{ static_cast<char>(ss.peek()) };
	if (next == ' ') { // Only faces
		ss >> f.indices[1] >> f.indices[2];
		if (ss.fail()) { throw LoadingException{ "Error loading indices" }; }
		ss >> f.indices[3];
		if (ss.fail()) { f.indices[3] = 0; } // Default to invalid value
	} else if (next == '/') {
		ss.ignore();
		next = ss.peek();
		if (next == '/') { // Only vertices and normals
			ss.ignore();
			ss >> f.normals[0]
			   >> f.indices[1] >> expect<'/'> >> expect<'/'> >> f.normals[1]
			   >> f.indices[2] >> expect<'/'> >> expect<'/'> >> f.normals[2];
			if (ss.fail()) { throw LoadingException{ "Error loading indices" }; }
			ss >> f.indices[3] >> expect<'/'> >> expect<'/'> >> f.normals[3];
			if (ss.fail()) { f.indices[3] = 0; } // Default to invalid value
		}
		else { // Vertices, coordinates and (normals)
			ss >> f.textures[0];
			next = ss.peek();
			if (next == ' ') { // Only vertices and coordinates
				ss >> f.indices[1] >> expect<'/'> >> f.textures[1]
				   >> f.indices[2] >> expect<'/'> >> f.textures[2];
				if (ss.fail()) { throw LoadingException{ "Error loading indices" }; }
				ss >> f.indices[3] >> expect<'/'> >> f.textures[3];
				if (ss.fail()) { f.indices[3] = 0; } // Default to invalid value
			}
			else { // Vertices, coordinates and normals
				ss >> expect<'/'> >> f.normals[0]
				   >> f.indices[1] >> expect<'/'> >> f.textures[1] >> expect<'/'> >> f.normals[1]
				   >> f.indices[2] >> expect<'/'> >> f.textures[2] >> expect<'/'> >> f.normals[2];
				if (ss.fail()) { throw LoadingException{ "Error loading indices" }; }
				ss >> f.indices[3] >> expect<'/'> >> f.textures[3] >> expect<'/'> >> f.normals[3];
				if (ss.fail()) { f.indices[3] = 0; } // Default to invalid value
			}
		}
	}
	// Pick only first 3 indices, ignoring all the other values. -1 cause obj starts from 1
	for (int i{ 0 }; i < 3; ++i) {
		if (f.indices[i] == 0) { throw LoadingException{ "Invalid index" }; }
		// Recalculate index
		unsigned positionIndex{ (f.indices[i] > 0) ? (f.indices[i] - 1) : (positionCount_ + f.indices[i]) };
		indices_.push_back(positionIndex);
		// Put texCoords in vertex
		unsigned texCoordsIndex{ f.textures[i] > 0 ? (f.textures[i] - 1) : positionIndex };
		if (texCoordsIndex < texCoordsCount_) {
			vertices_[positionIndex].texCoords = texCoords_[texCoordsIndex];
		}
		// Put normal in vertex
		unsigned normalIndex{ f.normals[i] > 0 ? (f.normals[i] - 1) : positionIndex };
		if (normalIndex < normalCount_) {
			vertices_[positionIndex].normal = normals_[normalIndex];
		}
	}
}


/// Loads a group
void WavefrontObject::loadGroup(std::stringstream &ss)
{
	loadCachedMesh();
	std::string command{};
	currentGroupName_ = "default"; // Default name is default
	ss >> command >> currentGroupName_;
	if (ss.fail()) { throw LoadingException{ "Error loading indices" }; }
}


/// Loads cached mesh
void WavefrontObject::loadCachedMesh()
{
	if (!vertices_.empty() && !indices_.empty()) {
		meshes_.push_back( new Mesh{ currentGroupName_, vertices_, indices_, textures_ });
		vertices_.clear();
		indices_.clear();
	}
}


/// Loads material library
void WavefrontObject::loadMaterials(std::stringstream &ss)
{
	// Read command
	std::string command{};
	ss >> command;
	if (ss.fail() || command != "mtllib") { throw LoadingException { "Error loading materials" }; }
	// Read mtl file
	std::string mtlName{};
	ss >> mtlName;
	if (ss.fail()) { throw LoadingException{ "Error loading materials" }; }
	std::ifstream is{ mtlName };
	if (!is.is_open()) { throw LoadingException{ "Could not find mtl file" }; }
	Material material;
	is >> material;
	if (is.fail()) { throw LoadingException{ "Error loading mtl" }; }
	materials_.push_back(material);
	// Read other optional mtl
	while (!ss.fail()) {
		ss >> mtlName;
		if (!ss.fail()) {
			std::ifstream is{ mtlName };
			if (!is.is_open()) { continue; }
			Material material;
			is >> material;
			if (!is.fail()) { materials_.push_back(material); }
		}
	}
}


/// Reads a Wavefront Object
std::ifstream &sunspot::operator>>(std::ifstream &is, WavefrontObject &obj)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (std::getline(is, line)) {
		if (line.length() <= 0) { continue; } // Ignore empty lines
		std::stringstream is{ line };
		switch (line[0]) {
		case '#': { break; }
		case 'o': { // Name command
			try { obj.loadName(is); }
			catch (const LoadingException &e) {
				std::cerr << "[" << lineNumber << "] " << e.what() << std::endl;
			}
			break;
		}
		case 'v': {
			if (line.length() < 2) {
				std::cerr << "[" << lineNumber << "] ignored: " << line << std::endl;
				break;
			}
			switch (line[1]) {
				case ' ': { // Vertex command
					try { obj.loadPosition(is); }
					catch (const LoadingException &e) {
						std::cerr << "[" << lineNumber << "] " << e.what() << std::endl;
					}
					break;
				}
				case 'p': {
					std::cout << "Point in the parameter space of a curve or a surface not supported\n";
					break;
				}
				case 'n': { // Vertex Normal command
					try { obj.loadNormal(is); }
					catch (const LoadingException &e) {
						std::cerr << "[" << lineNumber << "] " << e.what() << std::endl;
					}
					break;
				}
				case 't': { // Texture Coordinate command
					try { obj.loadTexCoords(is); }
					catch (const LoadingException &e) {
				std::cerr << "[" << lineNumber << "] " << e.what() << std::endl;
					}
					break;
				}
			}
			break;
		}
		case 'f': { // Face command
			try { obj.loadIndices(is); }
			catch (const LoadingException &e) {
				std::cerr << "[" << lineNumber << "] " << e.what() << ": " << line << std::endl;
			}
			break;
		}
		case 'g': { // Group command
			try { obj.loadGroup(is); }
			catch (const LoadingException &e) {
				std::cerr << "[" << lineNumber << "] " << e.what() << ": " << line << std::endl;
			}
			break;
		}
		case 'm': { // Material Library Command
			try { obj.loadMaterials(is); }
			catch (const LoadingException &e) {
				std::cerr << "[" << lineNumber << "] " << e.what() << ": " << line << std::endl;
			}
			break;
		}
		default: {
			std::cerr << "[" << lineNumber << "] ignored: " << line <<  std::endl;
			break;
		}}
		++lineNumber;
	}
	obj.loadCachedMesh();
	return is;
}

