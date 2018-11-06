#include "samplers/jittered-sampler.h"
#include "math/rasterizer.h"

using namespace math;
using namespace raytracer;

namespace
{
	class JitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int rows;
		int cols;

		explicit JitteredSampler(const int& rows, const int& cols)
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
					const auto x = f_rand(0, 1);
					const auto y = f_rand(0, 1);
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

Sampler samplers::jittered(const int& rows, const int& cols)
{
	return Sampler(std::make_shared<JitteredSampler>(rows, cols));
}