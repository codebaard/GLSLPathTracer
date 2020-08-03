#include <Rendermesh.h>

void Rendermesh::ParseModelData(Model* model) {

	Triangle* itr = (Triangle*)malloc(sizeof(Triangle));
	Faces = (Triangle*)malloc(sizeof(Triangle) * model->_faceCount);

	itr = Faces; //point itr to the first element of the Array.

	__int64 offset = 0;
	__int64 i; //mesh can be big!
	unsigned int maxN = model->_meshes.size();
	//unsigned int maxN = 1;

	for (int n = 0; n < maxN; n++) {

		for (i = 0; i < model->_meshes[n].Vertices.size(); i+=3) { 
		//for (i = 0; i < 1; i+=3) { 

			if (n == 0 && i == 0)
				itr->v1 = model->_meshes[n].Vertices[i].Position;			
			else 
				++itr->v1 = model->_meshes[n].Vertices[i].Position;

			itr->v2 = model->_meshes[n].Vertices[i+1].Position;
			itr->v3 = model->_meshes[n].Vertices[i+2].Position;

			//Assimp recalculates the mesh indices on a mesh by mesh basis.
			//For a single large SSBO we need the indices in a global context.
			itr->indices = glm::vec3(model->_meshes[n].Indices[i]+offset, model->_meshes[n].Indices[i+1]+offset, model->_meshes[n].Indices[i+2]+offset);

			// Edge1 + Edge2 will be calculated on the GPU on the fly -> Faster!

			itr->n1 = model->_meshes[n].Vertices[i].Normal;
			itr->n2 = model->_meshes[n].Vertices[i+2].Normal;
			itr->n3 = model->_meshes[n].Vertices[i+2].Normal;

			itr->Albedo = model->_meshes[n].pbrMat->Albedo;
			itr->Metalness = model->_meshes[n].pbrMat->Metallic;
			itr->Roughness = model->_meshes[n].pbrMat->Roughness;
			itr->IOR = model->_meshes[n].pbrMat->IOR;
			itr->Tr = model->_meshes[n].pbrMat->Tr;
			itr->F0 = model->_meshes[n].pbrMat->F0;

			//create predictable behaviour for the padding.
			itr->pad0 = 0;
			itr->pad1 = 0;
			itr->pad2 = 0;
			itr->pad3 = 0;
			itr->pad4 = 0;
			itr->pad5 = 0;
			itr->pad6 = 0;
			itr->pad7 = 0;
			itr->pad8 = 0;
			itr->pad9 = 0;
			itr->pad10 = 0;

			Facecount++;			
		}
		offset += i;
	}
}