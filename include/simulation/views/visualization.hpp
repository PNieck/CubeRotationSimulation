#pragma once

#include <numbers>

#include "visualization/framebuffer.hpp"
#include "visualization/camera.hpp"
#include "visualization/grid.hpp"

class Visualization {
public:
    Visualization(const int xResolution, const int yResolution):
        camera({
            .target = glm::vec3(0.f),
            .viewportWidth = xResolution,
            .viewportHeight = yResolution,
            .fov = std::numbers::pi_v<float> / 4.f,
            .nearPlane = 0.1f,
            .farPlane = 100.0f,
        })
    {}

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
};
