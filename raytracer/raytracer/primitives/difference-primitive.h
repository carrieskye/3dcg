#pragma once

#include "primitives/primitive.h"
#include <memory>

namespace raytracer
{
	namespace primitives
	{
		Primitive difference(Primitive child1, Primitive child2);
	}
}
