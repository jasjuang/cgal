// ======================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.4-I-65 $
// release_date  : $CGAL_Date: 2002/03/19 $
//
// file          : include/CGAL/config.h
// package       : Configuration (2.29)
// maintainer    : Geert-Jan Giezeman <geert@cs.uu.nl>
// source        :
// revision      : 1.11
// revision_date : 30 Mar 1998
// author(s)     : Wieger Wesselink <wieger@cs.ruu.nl>
//                 Michael Hoffmann
//
// coordinator   : Utrecht University
//
// ======================================================================

#ifndef CGAL_CONFIG_H
#define CGAL_CONFIG_H

#define CGAL_VERSION 2.4-I-65
#define CGAL_VERSION_NR 1002004065

#define CGAL_CFG_NO_ADVANCED_KERNEL 1

//----------------------------------------------------------------------//
//             STLport fix for MSVC
//----------------------------------------------------------------------//


#ifdef _MSC_VER
#   define CGAL_SCOPE
#   define CGAL_LIMITED_ITERATOR_TRAITS_SUPPORT 1
#	if _MSC_VER < 1300
#           include <stl_config.h>
#           include <stl_iterator_base.h>
#	endif
#else  // not _MSC_VER
#   define CGAL_SCOPE CGAL::
#   define CGAL_DEFINE_ITERATOR_TRAITS_POINTER_SPEC(a)
#endif // _MSC_VER


//----------------------------------------------------------------------//
//             include platform specific workaround flags (CGAL_CFG_...)
//----------------------------------------------------------------------//

#include <CGAL/compiler_config.h>

//----------------------------------------------------------------------//
//             do some post processing for the flags
//----------------------------------------------------------------------//


#ifdef CGAL_CFG_TYPENAME_BUG
#   define CGAL_TYPENAME_MSVC_NULL
#else
#   define CGAL_TYPENAME_MSVC_NULL typename
#endif

#ifdef CGAL_CFG_NO_NAMESPACE
#  define CGAL_USING_NAMESPACE_STD
#  define CGAL_STD
#  define CGAL std
#else
#  define CGAL_USING_NAMESPACE_STD using namespace std;
#  define CGAL_STD std
#  ifndef CGAL_USE_NAMESPACE
#    define CGAL_USE_NAMESPACE 1
#  endif
#endif

#if CGAL_USE_NAMESPACE
#  define CGAL_BEGIN_NAMESPACE namespace CGAL {
#  define CGAL_END_NAMESPACE }
#else
#  define CGAL_BEGIN_NAMESPACE
#  define CGAL_END_NAMESPACE
#endif

#ifdef CGAL_CFG_VC7_PRIVATE_TYPE_BUG
#  define CGAL_VC7_BUG_PROTECTED protected:
#else
#  define CGAL_VC7_BUG_PROTECTED
#endif

#ifdef CGAL_CFG_MATCHING_BUG_2
#   define CGAL_MSVC_DUMMY_ARGUMENT , int dummy=1
#else
#   define CGAL_MSVC_DUMMY_ARGUMENT
#endif

#ifdef CGAL_CFG_NO_TEMPLATE_FRIEND_DISTINCTION
#  define CGAL_NULL_TMPL_ARGS
#else
#  define CGAL_NULL_TMPL_ARGS <>
#endif

#ifdef CGAL_CFG_NO_EXPLICIT_CLASS_TEMPLATE_SPECIALISATION
#  define CGAL_TEMPLATE_NULL
#else
#  define CGAL_TEMPLATE_NULL template <>
#endif

#ifdef CGAL_CFG_NO_STDC_NAMESPACE
#define CGAL_CLIB_STD
#else
#define CGAL_CLIB_STD std
#endif

//----------------------------------------------------------------------//
//             include separate workaround files
//----------------------------------------------------------------------//

#ifdef _MSC_VER
#  include <CGAL/MSVC_standard_header_fixes.h>
#endif
#if defined(__BORLANDC__) && __BORLANDC__ > 0x520
#include <CGAL/Borland_fixes.h>
#endif
#if defined(__sun) && defined(__SUNPRO_CC)
#include <CGAL/Sun_fixes.h>
#endif

//----------------------------------------------------------------------//
//             select old or new style headers
//----------------------------------------------------------------------//

#ifndef CGAL_USE_NEWSTYLE_HEADERS
#  ifndef CGAL_CFG_NO_STANDARD_HEADERS
#    ifndef CGAL_NO_NEWSTYLE_HEADERS
#      define CGAL_USE_NEWSTYLE_HEADERS
#    endif // ! CGAL_NO_NEWSTYLE_HEADERS
#  endif // ! CGAL_CFG_NO_STANDARD_HEADERS
#endif // ! CGAL_USE_NEWSTYLE_HEADERS

//--------------------------------------------------------------------//
//           if defined CGAL_CFG_NOMINMAX_BUG define NOMINMAX flag
//-------------------------------------------------------------------//

#ifdef CGAL_CFG_NOMINMAX_BUG
#define NOMINMAX 1
#endif

#endif // CGAL_CONFIG_H

