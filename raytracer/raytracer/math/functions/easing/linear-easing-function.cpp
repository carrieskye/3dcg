#include "math/functions/easing/linear-easing-function.h"
#include "math/interval.h"
#include <assert.h>

using namespace math;
using namespace functions;


EasingFunction easing::linear()
{
    std::function<double(double)> lambda = [](double t) {
        assert(interval(0.0, 1.0).contains(t));

        return t;
    };

    return from_lambda(lambda);
}
