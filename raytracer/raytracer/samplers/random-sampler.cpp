#include "samplers/random-sampler.h"
#include "animation/animation.h"

using namespace math;
using namespace raytracer;

namespace
{
	class RandomSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int m_sample_count;

		explicit RandomSampler(const int& sample_count)
		{
			m_sample_count = sample_count;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;

			for (auto i = 0; i < m_sample_count; ++i)
			{
				const auto x = f_rand(0, 1);
				const auto y = f_rand(0, 1);
				result.push_back(rectangle.from_relative(Point2D(x, y)));
			}
			return result;
		}

		static double f_rand(const double f_min, const double f_max)
		{
			const auto f = static_cast<double>(rand()) / RAND_MAX;
			return f_min + f * (f_max - f_min);
		}
	};
}

Sampler samplers::random(const int& sample_count)
{
	return Sampler(std::make_shared<RandomSampler>(sample_count));
}