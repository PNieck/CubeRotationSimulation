#include "simulation/views/visualization.hpp"

#include <glm/gtc/matrix_transform.hpp>


Visualization::Visualization(const int xResolution, const int yResolution):
    camera({
        .target = glm::vec3(0.f),
        .viewportWidth = xResolution,
        .viewportHeight = yResolution,
        .fov = std::numbers::pi_v<float> / 4.f,
        .nearPlane = 0.1f,
        .farPlane = 100.0f,
    })
{
    const std::vector<float> planeVertices = {
         1.f, 0.f,  1.f,
         1.f, 0.f, -1.f,
        -1.f, 0.f, -1.f,
        -1.f, 0.f,  1.f,
    };

    const std::vector<uint32_t> planeIndices = {
        0, 1, 2,
        3, 0, 2
    };

    plane.UpdateMesh(planeVertices, planeIndices);

    const std::vector<float> cubeVertices = {
         0.5f,  0.5f,  0.5f,    // Vertex 0
         0.5f, -0.5f,  0.5f,    // Vertex 1
         0.5f, -0.5f, -0.5f,    // Vertex 2
         0.5f,  0.5f, -0.5f,    // Vertex 3
        -0.5f,  0.5f,  0.5f,    // Vertex 4
        -0.5f, -0.5f,  0.5f,    // Vertex 5
        -0.5f, -0.5f, -0.5f,    // Vertex 6
        -0.5f,  0.5f, -0.5f     // Vertex 7
    };

    const std::vector<uint32_t> cubeIndices = {
        0, 2, 1,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        4, 1, 5,
        4, 0, 1,
        7, 6, 2,
        7, 2, 3,
        7, 0, 4,
        7, 3, 0,
        5, 1, 6,
        6, 1, 2
    };

    const auto cubePos = glm::vec3(0.f, std::sqrt(3.f)/2.f, 0.f);
    cube.UpdateMesh(cubeVertices, cubeIndices);
    cube.SetPosition(cubePos);

    const auto cubeRotation = glm::rotate(glm::mat4(1.f), glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    const auto cubeRotation2 = glm::rotate(glm::mat4(1.f), glm::radians(90.f - 35.264f), normalize(glm::vec3(0.f, 0.f, 1.f)));

    cube.SetRotation(cubeRotation2 * cubeRotation);

    const std::vector<float> diagonalVertices = {
         0.5f,  0.5f,  0.5f,    // Vertex 0
        -0.5f, -0.5f, -0.5f,
    };

    diagonal.UpdateMesh(diagonalVertices, {0,1});
    diagonal.SetRotation(cubeRotation2 * cubeRotation);
    diagonal.SetPosition(cubePos);
}

void Visualization::Render() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    const auto view = camera.ViewMatrix();
    const auto projection = camera.ProjectionMatrix();

    const auto cameraMtx = projection * view;

    grid.Render(view, projection);

    shader.Use();
    shader.SetColor(glm::vec4(0.5f));
    shader.SetMVP(cameraMtx * plane.ModelMatrix());
    plane.UseMesh();
    glDrawElements(GL_TRIANGLES, plane.MeshElements(), GL_UNSIGNED_INT, 0);

    shader.SetColor(glm::vec4(0.133, 0.592, 0.82, 0.5));
    shader.SetMVP(cameraMtx * cube.ModelMatrix());
    cube.UseMesh();
    glDrawElements(GL_TRIANGLES, cube.MeshElements(), GL_UNSIGNED_INT, 0);

    shader.SetColor(glm::vec4(0.91, 0.902, 0.106, 1.f));
    shader.SetMVP(cameraMtx * diagonal.ModelMatrix());
    diagonal.UseMesh();
    glDrawElements(GL_LINES, diagonal.MeshElements(), GL_UNSIGNED_INT, 0);
}


void Visualization::ResizeWindow(const int width, const int height)
{
    glViewport(0, 0, width, height);
    camera.SetViewportSize(width, height);
}


void Visualization::RotateCamera(float x, float y)
{
    const auto oldPos = camera.GetPosition();

    auto rotation = glm::mat4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );

    rotation = glm::rotate(rotation, x, glm::vec3(0.f, 1.f, 0.f));

    const auto axis = glm::cross(oldPos, glm::vec3(0.f, 1.f, 0.f));
    rotation = glm::rotate(rotation, y, axis);

    const auto newPos = rotation * glm::vec4(oldPos, 1.f);
    camera.SetPosition(glm::vec3(newPos.x, newPos.y, newPos.z));
}
