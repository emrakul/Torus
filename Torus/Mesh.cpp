#include "Mesh.h"



Mesh::Mesh(const std::string location)
{
	sc = aiImportFile(&location[0], aiProcessPreset_TargetRealtime_Quality | aiProcess_GenNormals);
	
	indices.resize(sc->mMeshes[0]->mNumFaces * 3);
	for (int i = 0; i < sc->mMeshes[0]->mNumFaces * 3; i+=3) {
		indices[i  ] = sc->mMeshes[0]->mFaces[i].mIndices[0];
		indices[i+1] = sc->mMeshes[0]->mFaces[i].mIndices[1];
		indices[i+2] = sc->mMeshes[0]->mFaces[i].mIndices[2];
	}

	glGenBuffers(1, &VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sc->mMeshes[0]->mNumVertices, sc->mMeshes[0]->mVertices, GL_STATIC_DRAW);
	
	
	
	//glEnableVertexAttribArray(0);    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glDisableVertexAttribArray(0); 
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &Indexid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indexid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * sc->mMeshes[0]->mNumFaces, &indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Draw() {
	
	int CurrentSize1 = sc->mMeshes[0]->mNumFaces;

	
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, VBOid);


	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(uint), 0);	


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indexid);
	glDrawElements(GL_TRIANGLES, CurrentSize1, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(0);
}

Mesh::~Mesh()
{
}
