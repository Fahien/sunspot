#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

#include "WavefrontObject.h"


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


/// Loads a vertex from a string stream
void WavefrontObject::loadPosition(std::stringstream is)
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
	if(positionCount_ >= vertices_.size()) { // Add a new vertex
		vertices_.push_back(Vertex{});
	}
	vertices_[positionCount_].position = v; // Update new vertex
	++positionCount_;
}


/// Loads a texture coordinate from a string stream
void WavefrontObject::loadTexCoords(std::stringstream is)
{
	std::string command{};
	math::Vec2 t{};
	is >> command >> t.x;
	if (is.fail()) { throw LoadingException{ "Error loading texture coordinate" }; }
	is >> t.y;
	if (is.fail()) { t.y = 0.0f; } // Default to 0
	if(texCoordsCount_ >= vertices_.size()) { // Add a new vertex
		vertices_.push_back(Vertex{});
	}
	vertices_[texCoordsCount_].texCoords = t; // Update new vertex
	++texCoordsCount_;
}


/// Loads a vertex normal from a string stream
void WavefrontObject::loadNormal(std::stringstream is)
{
	std::string command{};
	math::Vec3 n{};
	is >> command >> n.x >> n.y >> n.z;
	if (is.fail()) { throw LoadingException{ "Error loading vertex normal" }; }
	if(normalCount_ >= vertices_.size()) { // Add a new vertex
		vertices_.push_back(Vertex{});
	}
	vertices_[normalCount_].normal= n; // Update new vertex
	++normalCount_;
}


template <char C>
std::istream& expect(std::istream& in)
{
	if ((in >> std::ws).peek() == C) { in.ignore(); }
	else { in.setstate(std::ios_base::failbit); }
	return in;
}


/// Loads a face from a string stream
void WavefrontObject::loadIndices(std::stringstream is)
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
		if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
	} else if (next == '/') {
		is.ignore();
		next = is.peek();
		if (next == '/') { // Only vertices and normals
			is.ignore();
			is >> f.normals[0] >> f.indices[1] >> expect<'/'> >> expect<'/'> >> f.normals[1]
				>> f.indices[2] >> expect<'/'> >> expect<'/'> >> f.normals[2];
			if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
			is >> f.indices[3] >> expect<'/'> >> expect<'/'> >> f.normals[3];
			if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
		}
		else { // Vertices, coordinates and normals
			is >> f.textures[0] >> expect<'/'> >> f.normals[0] >>
				f.indices[1] >> expect<'/'> >> f.textures[1] >> expect<'/'> >> f.normals[1] >>
				f.indices[2] >> expect<'/'> >> f.textures[2] >> expect<'/'> >> f.normals[2];
			if (is.fail()) { throw LoadingException{ "Error loading indices" }; }
			is >> f.indices[3] >> expect<'/'> >> f.textures[3] >> expect<'/'> >> f.normals[3];
			if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
		}
	}
	// Pick only first 3 indices, ignoring all the other values. -1 cause obj starts from 1
	for (int i{ 0 }; i < 3; ++i) {
		if (f.indices[i] > 0) { indices_.push_back(f.indices[i] - 1); }
		else if (f.indices[i] < 0) { indices_.push_back(positionCount_ + f.indices[i]); }
		else { throw LoadingException{ "Negative index" }; }
	}
}


/// Reads a Wavefront Object
std::ifstream &operator>>(std::ifstream &is, WavefrontObject &obj)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (std::getline(is, line)) {
		if (line.length() <= 0) { continue; }
		switch (line[0]) {
		case '#': {
			std::cout << "## Comment\n";
			break;
		}
		case 'v': {
			if (line.length() < 2) {
				std::cerr << "Error at line " << lineNumber << ": " << line <<  std::endl;
				break;
			}
			switch (line[1]) {
				case ' ': { // Vertex command
					try { obj.loadPosition(std::stringstream{ line }); }
					catch (LoadingException &e) {
						std::cerr << "Error at line " << lineNumber << ": " << e.what() <<  std::endl;
					}
					break;
				}
				case 'p': {
					std::cout << "Point in the parameter space of a curve or a surface not supported\n";
					break;
				}
				case 'n': { // Vertex Normal command
					try { obj.loadNormal(std::stringstream{ line }); }
					catch (LoadingException &e) {
						std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
					}
					break;
				}
				case 't': { // Texture Coordinate command
					try { obj.loadTexCoords(std::stringstream{ line }); }
					catch (LoadingException &e) {
						std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
					}
					break;
				}
			}
			break;
		}
		case 'f': { // Face command
			try { obj.loadIndices(std::stringstream{ line }); }
			catch (LoadingException &e) {
				std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
			}
			break;
		}
		default: {
			std::cerr << "Line " << lineNumber << " ignored: " << line <<  std::endl;
			break;
		}}
		++lineNumber;
	}
	return is;
}

