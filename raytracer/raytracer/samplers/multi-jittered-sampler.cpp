#include "samplers/multi-jittered-sampler.h"
#include "math/rasterizer.h"
#include <numeric>

using namespace math;
using namespace raytracer;

namespace
{
	class MultiJitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int n;

		explicit MultiJitteredSampler(const int& n)
		{
			this->n = n;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			auto rasterizer = Rasterizer(rectangle, n, n);

			std::vector<int> sub_squares(n*n);
			std::iota(std::begin(sub_squares), std::end(sub_squares), 0);

			for (auto i = 0; i < n*n; i++)
			{
				const auto sub_square = rand() % sub_squares.size();
				sub_squares.erase(sub_squares.begin() + sub_square);

				auto sub_rectangle = rasterizer[Position2D(i / n, i % n)];
				auto sub_rasterizer = Rasterizer(sub_rectangle, n, n);
				auto sub_sub_rectangle = sub_rasterizer[Position2D(sub_square / n, sub_square % n)];

				const auto x = f_rand(0, 1);
				const auto y = f_rand(0, 1);
				result.push_back(sub_sub_rectangle.from_relative(Point2D(x, y)));
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

Sampler samplers::multi_jittered(const int& n)
{
	return Sampler(std::make_shared<MultiJitteredSampler>(n));
}