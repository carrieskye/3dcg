#include "samplers/sampler.h"

using namespace math;


std::vector<Point2D> raytracer::samplers::_private_::SamplerImplementation::sample(const Rectangle2D& rectangle) const
{
    std::vector<Point2D> result;

    this->sample(rectangle, [&result](const Point2D& p) {
        result.push_back(p);
    });

    return result;
}

void raytracer::samplers::_private_::SamplerImplementation::sample(const Rectangle2D& rectangle, std::function<void(const Point2D&)> callback) const
{
    auto points = sample(rectangle);

    for (auto& point : points)
    {
        callback(point);
    }
}