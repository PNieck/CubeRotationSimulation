#pragma once

#include "visualization/camera.hpp"
#include "visualization/grid.hpp"
#include "visualization/renderObject.hpp"
#include "visualization/shaders/stdShader.hpp"
#include "visualization/mesh.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


class Visualization {
public:
    Visualization(int xResolution, int yResolution);

    void Update(const glm::quat& q);

    void Render() const;
    void RenderOptions();

    void ResizeWindow(int width, int height);

    void RotateCamera(float x, float y);

    [[nodiscard]]
    glm::vec3 GetCameraPosition() const
        { return camera.GetPosition(); }

    void SetCameraPosition(const glm::vec3& position)
        { camera.SetPosition(position); }

    [[nodiscard]]
    static const char* WindowName()
        { return "Visualization options"; }

private:
    Camera camera;
    Grid grid;
    RenderObject cube;
    RenderObject plane;
    RenderObject diagonal;
    RenderObject trace;

    glm::mat4 initCubeRotation;

    int maxTraceLen = 5000;
    std::vector<float> traceVertices;
    Mesh traceMesh;

    StdShader shader;

    bool renderCube = true;
    bool renderDiagonal = true;
    bool renderPlane = true;
    bool renderTrace = true;
};
