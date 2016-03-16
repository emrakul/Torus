#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "main.h"
#include "Utility.h"


class Mesh
{
public:
	const std::string location;
	const aiScene* sc;
	GLuint VBOid;
	GLuint Indexid;
	
	std::vector<uint> indices;
	void Draw();

	Mesh(const std::string location);
	
	~Mesh();
};

