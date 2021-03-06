/*
 *
 * Copyright (c) 2004
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         static_mutex.hpp
  *   VERSION      see <packages/boost/include/version.hpp>
  *   DESCRIPTION: Declares static_mutex lock type, there are three different
  *                implementations: POSIX pthreads, WIN32 threads, and portable,
  *                these are described in more detail below.
  */

#ifndef UVMSC_BOOST_REGEX_STATIC_MUTEX_HPP
#define UVMSC_BOOST_REGEX_STATIC_MUTEX_HPP

#include <packages/boost/include/config.hpp>
#include <packages/boost/include/regex/config.hpp> // dll packages/export options.

#ifdef UVMSC_BOOST_HAS_PTHREADS
#include <pthread.h>
#endif

#if defined(UVMSC_BOOST_HAS_PTHREADS) && defined(PTHREAD_MUTEX_INITIALIZER)
//
// pthreads version:
// simple wrap around a pthread_mutex_t initialized with
// PTHREAD_MUTEX_INITIALIZER.
//
namespace uvmsc_boost{

class UVMSC_BOOST_REGEX_DECL scoped_static_mutex_lock;

class static_mutex
{
public:
   typedef scoped_static_mutex_lock scoped_lock;
   pthread_mutex_t m_mutex;
};

#define UVMSC_BOOST_STATIC_MUTEX_INIT { PTHREAD_MUTEX_INITIALIZER, }

class UVMSC_BOOST_REGEX_DECL scoped_static_mutex_lock
{
public:
   scoped_static_mutex_lock(static_mutex& mut, bool lk = true);
   ~scoped_static_mutex_lock();
   inline bool locked()const
   {
      return m_have_lock;
   }
   inline operator void const*()const
   {
      return locked() ? this : 0;
   }
   void lock();
   void unlock();
private:
   static_mutex& m_mutex;
   bool m_have_lock;
};


} // namespace uvmsc_boost
#elif defined(UVMSC_BOOST_HAS_WINTHREADS)
//
// Win32 version:
// Use a 32-bit int as a lock, along with a test-and-set
// implementation using InterlockedCompareExchange.
//

#include <packages/boost/include/cstdint.hpp>

namespace uvmsc_boost{

class UVMSC_BOOST_REGEX_DECL scoped_static_mutex_lock;

class static_mutex
{
public:
   typedef scoped_static_mutex_lock scoped_lock;
   uvmsc_boost::int32_t m_mutex;
};

#define UVMSC_BOOST_STATIC_MUTEX_INIT { 0, }

class UVMSC_BOOST_REGEX_DECL scoped_static_mutex_lock
{
public:
   scoped_static_mutex_lock(static_mutex& mut, bool lk = true);
   ~scoped_static_mutex_lock();
   operator void const*()const
   {
      return locked() ? this : 0;
   }
   bool locked()const
   {
      return m_have_lock;
   }
   void lock();
   void unlock();
private:
   static_mutex& m_mutex;
   bool m_have_lock;
   scoped_static_mutex_lock(const scoped_static_mutex_lock&);
   scoped_static_mutex_lock& operator=(const scoped_static_mutex_lock&);
};

} // namespace

#else
//
// Portable version of a static mutex based on Boost.Thread library:
// This has to use a single mutex shared by all instances of static_mutex
// because uvmsc_boost::call_once doesn't alow us to pass instance information
// down to the initialisation proceedure.  In fact the initialisation routine
// may need to be called more than once - but only once per instance.
//
// Since this preprocessor path is almost never taken, we hide these header
// dependencies so that build tools don't find them.
//
#define B1 <packages/boost/include/thread/once.hpp>
#define B2 <packages/boost/include/thread/recursive_mutex.hpp>
#include B1
#include B2
#undef B1
#undef B2

namespace uvmsc_boost{

class UVMSC_BOOST_REGEX_DECL scoped_static_mutex_lock;
extern "C" UVMSC_BOOST_REGEX_DECL void boost_regex_free_static_mutex();

class UVMSC_BOOST_REGEX_DECL static_mutex
{
public:
   typedef scoped_static_mutex_lock scoped_lock;
   static void init();
   static uvmsc_boost::recursive_mutex* m_pmutex;
   static uvmsc_boost::once_flag m_once;
};

#define UVMSC_BOOST_STATIC_MUTEX_INIT {  }

class UVMSC_BOOST_REGEX_DECL scoped_static_mutex_lock
{
public:
   scoped_static_mutex_lock(static_mutex& mut, bool lk = true);
   ~scoped_static_mutex_lock();
   operator void const*()const;
   bool locked()const;
   void lock();
   void unlock();
private:
   uvmsc_boost::recursive_mutex::scoped_lock* m_plock;
   bool m_have_lock;
};

inline scoped_static_mutex_lock::operator void const*()const
{
   return locked() ? this : 0;
}

inline bool scoped_static_mutex_lock::locked()const
{
   return m_have_lock;
}

} // namespace

#endif

#endif
