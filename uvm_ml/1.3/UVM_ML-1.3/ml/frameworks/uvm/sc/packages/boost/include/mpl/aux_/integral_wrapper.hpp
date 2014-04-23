
// Copyright Aleksey Gurtovoy 2000-2006
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: integral_wrapper.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 02:19:02 -0400 (Sat, 11 Oct 2008) $
// $Revision: 49267 $

// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION!

#include <packages/boost/include/mpl/integral_c_tag.hpp>
#include <packages/boost/include/mpl/aux_/static_cast.hpp>
#include <packages/boost/include/mpl/aux_/nttp_decl.hpp>
#include <packages/boost/include/mpl/aux_/config/static_constant.hpp>
#include <packages/boost/include/mpl/aux_/config/workaround.hpp>

#include <packages/boost/include/preprocessor/cat.hpp>

#if !defined(AUX_WRAPPER_NAME)
#   define AUX_WRAPPER_NAME UVMSC_BOOST_PP_CAT(AUX_WRAPPER_VALUE_TYPE,_)
#endif

#if !defined(AUX_WRAPPER_PARAMS)
#   define AUX_WRAPPER_PARAMS(N) UVMSC_BOOST_MPL_AUX_NTTP_DECL(AUX_WRAPPER_VALUE_TYPE, N)
#endif

#if !defined(AUX_WRAPPER_INST)
#   if UVMSC_BOOST_WORKAROUND(__MWERKS__, <= 0x2407)
#       define AUX_WRAPPER_INST(value) AUX_WRAPPER_NAME< value >
#   else 
#       define AUX_WRAPPER_INST(value) UVMSC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::AUX_WRAPPER_NAME< value >
#   endif
#endif

UVMSC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

template< AUX_WRAPPER_PARAMS(N) >
struct AUX_WRAPPER_NAME
{
    UVMSC_BOOST_STATIC_CONSTANT(AUX_WRAPPER_VALUE_TYPE, value = N);
// agurt, 08/mar/03: SGI MIPSpro C++ workaround, have to #ifdef because some 
// other compilers (e.g. MSVC) are not particulary happy about it
#if UVMSC_BOOST_WORKAROUND(__EDG_VERSION__, <= 238)
    typedef struct AUX_WRAPPER_NAME type;
#else
    typedef AUX_WRAPPER_NAME type;
#endif
    typedef AUX_WRAPPER_VALUE_TYPE value_type;
    typedef integral_c_tag tag;

// have to #ifdef here: some compilers don't like the 'N + 1' form (MSVC),
// while some other don't like 'value + 1' (Borland), and some don't like
// either
#if UVMSC_BOOST_WORKAROUND(__EDG_VERSION__, <= 243)
 private:
    UVMSC_BOOST_STATIC_CONSTANT(AUX_WRAPPER_VALUE_TYPE, next_value = UVMSC_BOOST_MPL_AUX_STATIC_CAST(AUX_WRAPPER_VALUE_TYPE, (N + 1)));
    UVMSC_BOOST_STATIC_CONSTANT(AUX_WRAPPER_VALUE_TYPE, prior_value = UVMSC_BOOST_MPL_AUX_STATIC_CAST(AUX_WRAPPER_VALUE_TYPE, (N - 1)));
 public:
    typedef AUX_WRAPPER_INST(next_value) next;
    typedef AUX_WRAPPER_INST(prior_value) prior;
#elif UVMSC_BOOST_WORKAROUND(__BORLANDC__, UVMSC_BOOST_TESTED_AT(0x561)) \
    || UVMSC_BOOST_WORKAROUND(__IBMCPP__, UVMSC_BOOST_TESTED_AT(502)) \
    || (UVMSC_BOOST_WORKAROUND(__HP_aCC, <= 53800) && (UVMSC_BOOST_WORKAROUND(__hpxstd98, != 1)))
    typedef AUX_WRAPPER_INST( UVMSC_BOOST_MPL_AUX_STATIC_CAST(AUX_WRAPPER_VALUE_TYPE, (N + 1)) ) next;
    typedef AUX_WRAPPER_INST( UVMSC_BOOST_MPL_AUX_STATIC_CAST(AUX_WRAPPER_VALUE_TYPE, (N - 1)) ) prior;
#else
    typedef AUX_WRAPPER_INST( UVMSC_BOOST_MPL_AUX_STATIC_CAST(AUX_WRAPPER_VALUE_TYPE, (value + 1)) ) next;
    typedef AUX_WRAPPER_INST( UVMSC_BOOST_MPL_AUX_STATIC_CAST(AUX_WRAPPER_VALUE_TYPE, (value - 1)) ) prior;
#endif

    // enables uniform function call syntax for families of overloaded 
    // functions that return objects of both arithmetic ('int', 'long',
    // 'double', etc.) and wrapped integral types (for an example, see 
    // "mpl/example/power.cpp")
    operator AUX_WRAPPER_VALUE_TYPE() const { return static_cast<AUX_WRAPPER_VALUE_TYPE>(this->value); } 
};

#if !defined(UVMSC_BOOST_NO_INCLASS_MEMBER_INITIALIZATION)
template< AUX_WRAPPER_PARAMS(N) >
AUX_WRAPPER_VALUE_TYPE const AUX_WRAPPER_INST(N)::value;
#endif

UVMSC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#undef AUX_WRAPPER_NAME
#undef AUX_WRAPPER_PARAMS
#undef AUX_WRAPPER_INST
#undef AUX_WRAPPER_VALUE_TYPE