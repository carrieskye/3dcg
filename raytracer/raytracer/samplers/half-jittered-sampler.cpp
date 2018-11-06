#include "samplers/half-jittered-sampler.h"
#include "math/rasterizer.h"

using namespace math;
using namespace raytracer;

namespace
{
	class HalfJitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int rows;
		int cols;

		explicit HalfJitteredSampler(const int& rows, const int& cols)
		{
			this->rows = rows;
			this->cols = cols;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			auto rasterizer = Rasterizer(rectangle, rows, cols);

			for (auto i = 0; i < rows; i++)
			{
				for (auto j = 0; j < cols; j++)
				{
					auto sub_rectangle = rasterizer[Position2D(i, j)];
					const auto x = f_rand(0.25, 0.75);
					const auto y = f_rand(0.25, 0.75);
					result.push_back(sub_rectangle.from_relative(Point2D(x, y)));
				}
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

Sampler samplers::half_jittered(const int& rows, const int& cols)
{
	return Sampler(std::make_shared<HalfJitteredSampler>(rows, cols));
}