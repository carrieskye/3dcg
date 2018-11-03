#include "demos/basic-sample2.h"
#include "demos/demo.h"
#include "util/lazy.h"
#include "easylogging++.h"

using namespace raytracer;
using namespace animation;
using namespace math;
using namespace imaging;
using namespace demos;


namespace
{
    class MeshDemo : public Demo
    {
    public:
        // C++ allows you to copy a superclass's constructor
        using Demo::Demo;

    protected:
        /// <summary>
        /// Creates the root. This method will be called for several values of <paramref name="now" />,
        /// thus creating an animation.
        /// </summary>
        Primitive create_root(TimeStamp now) override
        {
            // Local imports. Allows us to write sphere() instead of raytracer::primitives::sphere()
            using namespace primitives;
            using namespace materials;

            // Define material properties
            MaterialProperties material_properties(
                colors::black(),      // ambient lighting, not supported yet
                colors::black(),      // diffuse lighting, not supported yet
                colors::black(),      // specular highlights, not supported yet
                0                     // specular exponent, not supported yet
            );

            // Create a uniform material: all parts of the primitive will be made out of the same material
            auto material = uniform(material_properties);

            // Create an animation of a double going from -5.0 to 5.0 in 1 second
            // It is important to write 5.0 and not 5, otherwise it will create an animation of ints instead of doubles
			auto x_position = animation::animate(5.0, -5.0, 1_s);
			auto z_size = animation::animate(1.0, 5.0, 1_s);

            // Create a sphere. It has radius 1 and is centered at (0, 0, 0)
			Primitive primitive1 = sphere();
			Primitive primitive2 = sphere();

            // Move the sphere. x_position(now) = asks the animation what x at this point in time
			primitive1 = translate(Vector3D(x_position(now), x_position(now), 3), primitive1);
			primitive2 = translate(Vector3D(-x_position(now), 0, 0), primitive2);
			std::vector<Primitive> primitives = { primitive1, primitive2 };

			auto root_union = make_union(primitives);

            // Assign a material to the sphere
            return decorate(material, root_union);

            //return primitive;
        }

        /// <summary>
        /// Creates light sources.
        /// </summary>
        std::vector<LightSource> create_light_sources(TimeStamp now) override
        {
            // Local import
            using namespace lights;

            std::vector<LightSource> light_sources;

            // No lights

            return light_sources;
        }

        /// <summary>
        /// Creates camera.
        /// </summary>
        Camera create_camera(TimeStamp) override
        {
            return cameras::perspective(
                Point3D(0, 0, 5),         // position of eye
                Point3D(0, 0, 0),          // point the camera looks at
                Vector3D(1, 0, 0),         // up-vector: indicates camera is "standing up"
                1,                         // distance between eye and viewing plane
                1                          // aspect ratio
            );
        }

    private:
        Primitive m_mesh;
    };
}

void demos::basic_sample2(std::shared_ptr<pipeline::Consumer<std::shared_ptr<Bitmap>>> output)
{
    MeshDemo(200, 1_s, 5, 1).render(output);
}
