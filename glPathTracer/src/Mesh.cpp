#include <Mesh.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, mtlMaterial* mat)
{
    this->Vertices = vertices;
    this->Indices = indices;
    this->Mat = mat; //for non-pbr workflow.
    this->pbrMat = mat->TransformToPBR();

    _setupMesh();
    //_setupTextures(); //not for pbr-workflow
}

void Mesh::_setupMesh() 
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    //vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangents));

    //vertex tangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangents));

    glBindVertexArray(0);
}

void Mesh::_setupTextures() {
    if (Mat->Ambient.IsLoaded) {
        LoadedTextures.push_back(&Mat->Ambient);
    }
    if (Mat->Diffuse.IsLoaded) {
        LoadedTextures.push_back(&Mat->Diffuse);
    }
    if (Mat->Specular.IsLoaded) {
        LoadedTextures.push_back(&Mat->Specular);
    }
    if (Mat->Emissive.IsLoaded) {
        LoadedTextures.push_back(&Mat->Emissive);
    }
    if (Mat->Normal.IsLoaded) {
        LoadedTextures.push_back(&Mat->Normal);
    }
}

void Mesh::Draw(RenderShader&shader) {
    
    shader.use();

    //for non-pbr workflow - deactivated 31.07.2020
    //for (auto tex : LoadedTextures) {
    //    tex->ActivateTexture();
    //}

    shader.setMaterial(pbrMat);
    
    // draw mesh
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

