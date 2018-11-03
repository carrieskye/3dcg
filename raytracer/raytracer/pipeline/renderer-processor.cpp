#include "pipeline/renderer-processor.h"
#include "easylogging++.h"

using namespace raytracer;
using namespace pipeline;
using namespace imaging;


namespace
{
    class RendererProcessor : public Processor<std::shared_ptr<Scene>, std::shared_ptr<Bitmap>>
    {
    public:
        RendererProcessor(Renderer renderer)
            : m_renderer(renderer) { }

        void consume(std::shared_ptr<Scene> scene) override
        {
            assert(scene);

            auto result = m_renderer->render(*scene);
            produce(result);
        }

    private:
        Renderer m_renderer;
    };
}

std::shared_ptr<Processor<std::shared_ptr<Scene>, std::shared_ptr<Bitmap>>> pipeline::renderer(Renderer renderer)
{
    return std::make_shared<RendererProcessor>(renderer);
}