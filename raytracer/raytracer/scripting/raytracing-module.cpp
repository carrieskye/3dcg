#include "raytracers/ray-tracer-v1.h"
#ifndef EXCLUDE_SCRIPTING

#include "scripting/raytracing-module.h"
#include "raytracers/ray-tracers.h"
#include "scripting/scripting-util.h"


using namespace chaiscript;
using namespace raytracer;
using namespace scripting;


namespace
{
    struct RaytracerLibrary
    {
        RayTracer v0() const { return raytracers::v0(); }
		RayTracer v1() const { return raytracers::v1(); }
		RayTracer v2() const { return raytracers::v2(); }

        RayTracer v(int version) const
        {
            switch (version)
            {
		
#           define DISPATCH(N) case N: return v ## N()
				DISPATCH(0);
#           undef DISPATCH

            default:
                LOG(ERROR) << "Unknown ray tracer version " << version;
                abort();
            }
        }
    };

    std::shared_ptr<Scene> create_scene(Camera camera, Primitive root, const std::vector<Boxed_Value>& boxed_lights)
    {
        auto unboxed_lights = util::cast_vector_elements<LightSource>(boxed_lights);
        return std::make_shared<Scene>(camera, root, unboxed_lights);
    }
}

ModulePtr scripting::_private_::create_raytracing_module()
{
    // Create new chaiscript module
    auto module = std::make_shared<Module>();

    // Tell chaiscript about the RayTracer type
    raytracer::scripting::util::register_type<RayTracer>(*module, "RayTracer");

    // Create library
    auto raytracer_library = std::make_shared<RaytracerLibrary>();

    // Expose library under the given name (member functions need to be exposed separately)
    module->add_global_const(const_var(raytracer_library), "Raytracers");

    // Expose each member of the library
#   define BIND(NAME)   module->add(fun(&RaytracerLibrary::NAME), #NAME)
	BIND(v0);
	BIND(v1);
	BIND(v2);
	BIND(v);
#   undef BIND

    // Expose create_scene under the same name
    module->add(fun(&create_scene), "create_scene");

    return module;
}

#endif
