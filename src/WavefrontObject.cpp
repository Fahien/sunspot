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
	: positionCount_{ 0 }
	, normalCount_{ 0 }
	, texCoordsCount_{ 0 }
	, vertices_{}
	, indices_{}
	, textures_{}
{
	std::cout << "WavefrontObject: created\n"; // TODO remove debug log
}


/// Release resources
WavefrontObject::~WavefrontObject()
{
	std::cout << "WavefrontObject: destroyed\n"; // TODO remove debug log
}


/// Loads the object name
void WavefrontObject::loadName(std::stringstream &is)
{
	std::string command{};
	is >> command >> name_;
	if (is.fail()) { throw LoadingException{ "Error loading name" }; }
	std::cout << "Obj name: " << name_ << std::endl; // TODO remove debug log
}


/// Loads a vertex position from a string stream
void WavefrontObject::loadPosition(std::stringstream &is)
{
	std::string command{};
	math::Vec3 v{};
	is >> command >> v.x >> v.y >> v.z;
	if (is.fail()) { throw LoadingException{ "Error loading vertex" }; }
	float w;
	is >> w;
	if (!is.fail()) { // Deomogenize
		v.x /= w;
		v.y /= w;
		v.z /= w;
	}
	if(positionCount_ >= vertices_.size()) { vertices_.push_back(Vertex{}); } // Add a vertex
	vertices_[positionCount_].position = v; // Update new vertex
	++positionCount_; // TODO remove?
}


/// Loads a texture coordinate from a string stream
void WavefrontObject::loadTexCoords(std::stringstream &is)
{
	std::string command{};
	math::Vec2 t{};
	is >> command >> t.x;
	if (is.fail()) { throw LoadingException{ "Error loading texture coordinate" }; }
	is >> t.y;
	if (is.fail()) { t.y = 0.0f; } // Default to 0
	texCoords_.push_back(t);
	++texCoordsCount_; // TODO remove?
}


/// Loads a vertex normal from a string stream
void WavefrontObject::loadNormal(std::stringstream &is)
{
	std::string command{};
	math::Vec3 n{};
	is >> command >> n.x >> n.y >> n.z;
	if (is.fail()) { throw LoadingException{ "Error loading vertex normal" }; }
	normals_.push_back(n);
	++normalCount_; // TODO remove?
}


template <char C>
std::istream& expect(std::istream& in)
{
	if ((in >> std::ws).peek() == C) { in.ignore(); }
	else { in.setstate(std::ios_base::failbit); }
	return in;
}


/// Loads a face from a string stream
void WavefrontObject::loadIndices(std::stringstream &is)
{
	std::string command{};
	Face f{};
	is >> command >> f.indices[0];
	if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
	char next{ static_cast<char>(is.peek()) };
	if (next == ' ') { // Only faces
		is >> f.indices[1] >> f.indices[2];
		if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
		is >> f.indices[3];
		if (is.fail()) { f.indices[3] = 0; } // Default to invalid value
	} else if (next == '/') {
		is.ignore();
		next = is.peek();
		if (next == '/') { // Only vertices and normals
			is.ignore();
			is >> f.normals[0]
			   >> f.indices[1] >> expect<'/'> >> expect<'/'> >> f.normals[1]
			   >> f.indices[2] >> expect<'/'> >> expect<'/'> >> f.normals[2];
			if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
			is >> f.indices[3] >> expect<'/'> >> expect<'/'> >> f.normals[3];
			if (is.fail()) { f.indices[3] = 0; } // Default to invalid value
		}
		else { // Vertices, coordinates and (normals)
			is >> f.textures[0];
			next = is.peek();
			if (next == ' ') { // Only vertices and coordinates
				is >> f.indices[1] >> expect<'/'> >> f.textures[1]
				   >> f.indices[2] >> expect<'/'> >> f.textures[2];
				if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
				is >> f.indices[3] >> expect<'/'> >> f.textures[3];
				if (is.fail()) { f.indices[3] = 0; } // Default to invalid value
			}
			else { // Vertices, coordinates and normals
				is >> expect<'/'> >> f.normals[0]
				   >> f.indices[1] >> expect<'/'> >> f.textures[1] >> expect<'/'> >> f.normals[1]
				   >> f.indices[2] >> expect<'/'> >> f.textures[2] >> expect<'/'> >> f.normals[2];
				if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
				is >> f.indices[3] >> expect<'/'> >> f.textures[3] >> expect<'/'> >> f.normals[3];
				if (is.fail()) { f.indices[3] = 0; } // Default to invalid value
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
			vertices_[texCoordsIndex].texCoords = texCoords_[texCoordsIndex];
		}
		// Put normal in vertex
		unsigned normalIndex{ f.normals[i] > 0 ? (f.normals[i] - 1) : positionIndex };
		if (normalIndex < normalCount_) {
			vertices_[positionIndex].normal = normals_[normalIndex];
		}
	}
}


/// Reads a Wavefront Object
std::ifstream &operator>>(std::ifstream &is, WavefrontObject &obj)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (std::getline(is, line)) {
		if (line.length() <= 0) { continue; }
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
					catch (LoadingException &e) {
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
					catch (LoadingException &e) {
						std::cerr << "[" << lineNumber << "] " << e.what() << std::endl;
					}
					break;
				}
				case 't': { // Texture Coordinate command
					try { obj.loadTexCoords(is); }
					catch (LoadingException &e) {
				std::cerr << "[" << lineNumber << "] " << e.what() << std::endl;
					}
					break;
				}
			}
			break;
		}
		case 'f': { // Face command
			try { obj.loadIndices(is); }
			catch (LoadingException &e) {
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
	return is;
}

