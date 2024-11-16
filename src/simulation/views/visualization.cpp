#include "simulation/views/visualization.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"


Visualization::Visualization(const int xResolution, const int yResolution):
    camera({
        .target = glm::vec3(0.f),
        .viewportWidth = xResolution,
        .viewportHeight = yResolution,
        .fov = std::numbers::pi_v<float> / 4.f,
        .nearPlane = 0.1f,
        .farPlane = 100.0f,
    }),
    initCubeRotation(
        toMat4(rotation(glm::normalize(glm::vec3(1.f, 1.f, 1.f)), glm::vec3(0.f, 1.f, 0.f)))
    )
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
        1.f, 1.f, 1.f,    // Vertex 0
        1.f, 0.f, 1.f,    // Vertex 1
        1.f, 0.f, 0.f,    // Vertex 2
        1.f, 1.f, 0.f,    // Vertex 3
        0.f, 1.f, 1.f,    // Vertex 4
        0.f, 0.f, 1.f,    // Vertex 5
        0.f, 0.f, 0.f,    // Vertex 6
        0.f, 1.f, 0.f     // Vertex 7
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

    cube.UpdateMesh(cubeVertices, cubeIndices);

    const std::vector<float> diagonalVertices = {
        1.f, 1.f, 1.f,    // Vertex 0
        0.f, 0.f, 0.f,    // Vertex 1
    };

    diagonal.UpdateMesh(diagonalVertices, {0,1});

    const std::vector<float> vectorVertices = {
         0.f,   0.f, 0.f,    // Vertex 0
         1.f,   0.f, 0.f,    // Vertex 1
        0.8f,  0.2f, 0.f,    // Vector 2
        0.8f, -0.2f, 0.f     // Vector 3
    };

    const std::vector<uint32_t> vectorIndices {
        0, 1, 2, 1, 3
    };

    gravityVector.UpdateMesh(vectorVertices, vectorIndices);
    gravityVector.SetRotation(
        toMat4(rotation(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)))
    );
    gravityVector.SetScale(glm::vec3(0.981f, 0.981f, 0.981f));

    traceVertices.reserve(maxTraceLen * 3);
}


void Visualization::Update(const glm::quat& q)
{
    const auto rotMat = glm::toMat4(q) * initCubeRotation;

    cube.SetRotation(rotMat);
    diagonal.SetRotation(rotMat);

    const auto diagMat = diagonal.ModelMatrix();

    const auto diagonalEnd = diagMat * glm::vec4(1.f, 1.f, 1.f, 1.f);
    if (traceVertices.size() == maxTraceLen*3) {
        const auto firstElem = traceVertices.begin();
        const auto thirdElem = traceVertices.begin() + 3;

        traceVertices.erase(firstElem, thirdElem);
    }

    traceVertices.push_back(diagonalEnd.x);
    traceVertices.push_back(diagonalEnd.y);
    traceVertices.push_back(diagonalEnd.z);

    traceMesh.Update(traceVertices);

    gravityVector.SetPosition(diagMat * glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
}


void Visualization::Render() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    const auto view = camera.ViewMatrix();
    const auto projection = camera.ProjectionMatrix();

    const auto cameraMtx = projection * view;

    grid.Render(view, projection);

    shader.Use();

    if (renderPlane) {
        shader.SetColor(glm::vec4(0.5f));
        shader.SetMVP(cameraMtx * plane.ModelMatrix());
        plane.UseMesh();
        glDrawElements(GL_TRIANGLES, plane.MeshElements(), GL_UNSIGNED_INT, nullptr);
    }

    if (renderCube) {
        shader.SetColor(glm::vec4(0.133, 0.592, 0.82, 0.5));
        shader.SetMVP(cameraMtx * cube.ModelMatrix());
        cube.UseMesh();
        glDrawElements(GL_TRIANGLES, cube.MeshElements(), GL_UNSIGNED_INT, nullptr);
    }

    if (renderDiagonal) {
        shader.SetColor(glm::vec4(0.91, 0.902, 0.106, 1.f));
        shader.SetMVP(cameraMtx * diagonal.ModelMatrix());
        diagonal.UseMesh();
        glDrawElements(GL_LINES, diagonal.MeshElements(), GL_UNSIGNED_INT, nullptr);
    }

    if (renderTrace) {
        shader.SetColor(glm::vec4(1.f));
        shader.SetMVP(cameraMtx);
        traceMesh.Use();
        glDrawArrays(GL_LINE_STRIP, 0, traceMesh.GetElementsCnt());
    }

    if (renderGravityVector) {
        shader.SetColor(glm::vec4(1.f));
        shader.SetMVP(cameraMtx * gravityVector.ModelMatrix());
        gravityVector.UseMesh();
        glDrawElements(GL_LINE_STRIP, gravityVector.MeshElements(), GL_UNSIGNED_INT, nullptr);
    }
}


void Visualization::RenderOptions()
{
    ImGui::Begin(WindowName());

    ImGui::Checkbox("Render cube", &renderCube);
    ImGui::Checkbox("Render diagonal", &renderDiagonal);
    ImGui::Checkbox("Render plane", &renderPlane);
    ImGui::Checkbox("Render trace", &renderTrace);
    ImGui::Checkbox("Render gravity vector", &renderGravityVector);

    const int oldMaxTraceLen = maxTraceLen;
    if (ImGui::InputInt("Trace length",&maxTraceLen)) {
        if (oldMaxTraceLen > maxTraceLen)
            traceVertices.erase(traceVertices.begin() + maxTraceLen*3, traceVertices.end());
        else
            traceVertices.reserve(maxTraceLen*3);
    }

    if (ImGui::Button("Reset trace"))
        ResetTrace();

    ImGui::End();
}


void Visualization::ResizeWindow(const int width, const int height)
{
    glViewport(0, 0, width, height);
    camera.SetViewportSize(width, height);
}


void Visualization::RotateCamera(const float x, const float y)
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

void Visualization::SetCubeEdgeLen(const float edge)
{
    const auto newScale = glm::vec3(edge, edge, edge);
    cube.SetScale(newScale);
    diagonal.SetScale(newScale);
}
