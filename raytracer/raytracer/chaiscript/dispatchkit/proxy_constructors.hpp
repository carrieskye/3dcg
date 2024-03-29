// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2016, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com


#ifndef CHAISCRIPT_PROXY_CONSTRUCTORS_HPP_
#define CHAISCRIPT_PROXY_CONSTRUCTORS_HPP_

#include "proxy_functions.hpp"

namespace chaiscript
{
  namespace dispatch
  {
    namespace detail
    {

      template<typename Class, typename ... Params  >
        Proxy_Function build_constructor_(Class (*)(Params...))
        {
          auto call = detail::Constructor<Class, Params...>();

          return Proxy_Function(
            chaiscript::make_shared<Proxy_Function_Base, Proxy_Function_Callable_Impl<std::shared_ptr<Class> (Params...), decltype(call)>>(call));
        }
    }
  }


  /// \brief Generates a constructor function for use with ChaiScript
  /// 
  /// \tparam T The signature of the constructor to generate. In the form of: ClassType (ParamType1, ParamType2, ...)
  /// 
  /// Example:
  /// \code
  ///    chaiscript::ChaiScript chai;
  ///    // Create a new function that creates a MyClass object using the (int, float) constructor
  ///    // and call that function "MyClass" so that it appears as a normal constructor to the user.
  ///    chai.add(constructor<MyClass (int, float)>(), "MyClass");
  /// \endcode
  template<typename T>
    Proxy_Function constructor()
    {
      T *f = nullptr;
      return (dispatch::detail::build_constructor_(f));
    }

}

#endif

