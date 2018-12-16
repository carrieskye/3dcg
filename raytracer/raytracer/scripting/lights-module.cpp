#ifndef EXCLUDE_SCRIPTING

#include "scripting/lights-module.h"
#include "scripting/scripting-util.h"
#include "lights/lights.h"

using namespace chaiscript;
using namespace raytracer;
using namespace math;
using namespace imaging;


namespace
{
    struct LightLibrary
    {
        LightSource omnidirectional(const Point3D& position, const Color& c) const
        {
            return lights::omnidirectional(position, c);
        }
		LightSource spot(const Point3D& position, const Point3D& direction, const Angle& angle, const Color& c) const
        {
			return lights::spot(position, direction, angle, c);
        }
    };
}

ModulePtr scripting::_private_::create_lights_module()
{
    auto module = std::make_shared<Module>();

    util::register_type<LightSource>(*module, "LightSource");

    auto lights_library = std::make_shared<LightLibrary>();
    module->add_global_const(const_var(lights_library), "Lights");

#   define BIND(NAME)                      BIND_AS(NAME, NAME)
#   define BIND_AS(INTERNAL, EXTERNAL)     module->add(fun(&LightLibrary::INTERNAL), #EXTERNAL)
    BIND(omnidirectional);
	BIND(spot);
#   undef BIND_AS
#   undef BIND

    return module;
}

#endif
