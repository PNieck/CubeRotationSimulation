#pragma once

#include "visualization/camera.hpp"
#include "visualization/grid.hpp"
#include "visualization/renderObject.hpp"
#include "visualization/shaders/stdShader.hpp"


class Visualization {
public:
    Visualization(int xResolution, int yResolution);

    void Render() const;

    void ResizeWindow(int width, int height);

    void RotateCamera(float x, float y);

    [[nodiscard]]
    glm::vec3 GetCameraPosition() const
        { return camera.GetPosition(); }

    void SetCameraPosition(const glm::vec3& position)
        { camera.SetPosition(position); }

    [[nodiscard]]
    static const char* WindowName()
        { return "Visualization"; }

private:

    Camera camera;
    Grid grid;
    RenderObject cube;
    RenderObject plane;
    RenderObject diagonal;

    StdShader shader;
};
