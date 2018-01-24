// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_OS_H_INCLUDED__
#define __IRR_OS_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace os
{

	class Warning
	{
	public:

		//! shows a warning message box
		static void show(const c8* message, const c8* hint = 0);

		//! shows a warning message box in unicode form
		static void show(const wchar_t* message, const wchar_t* hint = 0);

		//! prints a warning into the warning log
		static void print(const c8* message, const c8* hint = 0);
	};



	class Debuginfo
	{
	public:

		//! prints a debuginfo string
		static void print(const c8* message, const c8* hint = 0);

		//! prints a debuginfo string (unicode)
		static void print(const wchar_t* message, const wchar_t* hint = 0);
	};


	class Randomizer
	{
	public:

		//! resets the randomizer
		static void reset();

		//! generates a pseudo random number
		static s32 rand();

	private:

		static s32 seed;
	};




	class Timer
	{
	public:

		//! returns the current time in milliseconds
		static u32 getTime();
	};


} // end namespace os
} // end namespace irr


#endif