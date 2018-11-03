#pragma once

#include "primitives/primitives.h"
#include "lights/lights.h"
#include "cameras/camera.h"


namespace raytracer
{
    struct Scene
    {
        Scene(Camera camera, Primitive root, const std::vector<LightSource>& light_sources)
            : camera(camera), root(root), light_sources(light_sources) { }

        Scene(Camera camera, Primitive root)
            : camera(camera), root(root), light_sources() { }

        Camera camera;
        Primitive root;
        std::vector<LightSource> light_sources;
    };
}