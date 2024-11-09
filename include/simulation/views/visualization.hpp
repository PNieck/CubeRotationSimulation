#pragma once

#include "visualization/framebuffer.hpp"

class Visualization {
public:
    Visualization(const int xResolution, const int yResolution):
        framebuffer(yResolution, xResolution) {}

    void Render() const;

    [[nodiscard]]
    static const char* WindowName()
        { return "Visualization"; }

private:
    Framebuffer framebuffer;
};
