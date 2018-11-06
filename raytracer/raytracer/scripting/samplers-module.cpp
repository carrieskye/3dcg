#ifndef EXCLUDE_SCRIPTING

#include "scripting/samplers-module.h"
#include "samplers/samplers.h"
#include "scripting/scripting-util.h"

using namespace chaiscript;
using namespace raytracer;
using namespace math;


namespace
{
	struct SamplerLibrary
	{
		Sampler single() const
		{
			return samplers::single();
		}
		Sampler random(const int& sample_count) const
		{
			return samplers::random(sample_count);
		}
		Sampler stratified(const int& rows, const int& cols) const
		{
			return samplers::stratified(rows, cols);
		}
		Sampler jittered(const int& rows, const int& cols) const
		{
			return samplers::jittered(rows, cols);
		}
		Sampler halfjittered(const int& rows, const int& cols) const
		{
			return samplers::half_jittered(rows, cols);
		}
		Sampler nrooks(const int& rooks) const
		{
			return samplers::n_rooks(rooks);
		}
	};
}

ModulePtr scripting::_private_::create_samplers_module()
{
	auto module = std::make_shared<Module>();

	util::register_type<Sampler>(*module, "Sampler");

	auto sampler_library = std::make_shared<SamplerLibrary>();
	module->add_global_const(const_var(sampler_library), "Samplers");

#   define BIND(NAME)  module->add(fun(&SamplerLibrary::NAME), #NAME)
	BIND(single);
	BIND(random);
	BIND(stratified);
	BIND(jittered);
	BIND(halfjittered);
	BIND(nrooks);
#   undef BIND

	return module;
}

#endif
