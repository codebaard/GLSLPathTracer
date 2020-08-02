#include <Rendermesh.h>

void Rendermesh::ParseModelData(Model* model) {

	Triangle* itr = (Triangle*)malloc(sizeof(Triangle));
	Faces = (Triangle*)malloc(sizeof(Triangle) * model->_faceCount); //uuuh...I <3 C++

	itr = Faces; //point itr to the first element of the Array.

	__int64 offset = 0;
	__int64 i;

	for (Mesh mesh : model->_meshes) {

		for (i = 0; i < mesh.Vertices.size(); i+=3) { //mesh can be big!
			itr->v1 = mesh.Vertices[i].Position;
			itr->v2 = mesh.Vertices[i+1].Position;
			itr->v3 = mesh.Vertices[i+2].Position;

			//Assimp recalculates the mesh indices on a mesh by mesh basis.
			//For a single large SSBO we need the indices in a global context.
			itr->indices = glm::vec3(mesh.Indices[i]+offset, mesh.Indices[i+1]+offset, mesh.Indices[i+2]+offset);

			// Edge1 + Edge2 will be calculated on the GPU on the fly -> Faster!

			itr->n1 = mesh.Vertices[i].Normal;
			itr->n2 = mesh.Vertices[i+2].Normal;
			itr->n3 = mesh.Vertices[i+2].Normal;

			itr->Albedo = mesh.pbrMat->Albedo;
			itr->Metalness = mesh.pbrMat->Metallic;
			itr->Roughness = mesh.pbrMat->Roughness;
			itr->IOR = mesh.pbrMat->IOR;
			itr->Tr = mesh.pbrMat->Tr;
			itr->F0 = mesh.pbrMat->F0;

			itr++; //next element
			Facecount++;
			
		}

		offset += i;
	}


}