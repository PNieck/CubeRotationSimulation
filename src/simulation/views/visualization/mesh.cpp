#include <simulation/views/visualization/mesh.hpp>


Mesh::Mesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}


Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


void Mesh::Update(const std::vector<float> &vertices)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));

    elementsCnt = vertices.size() / 3;
}



