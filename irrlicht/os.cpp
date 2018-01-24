// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "os.h"
#include <stdio.h>


#ifdef WIN32

// Windows specific functions

#include <windows.h>

namespace irr
{
namespace os
{


	//! prints a debuginfo string
	void Debuginfo::print(const c8* message, const c8* hint)
	{
		size_t hs = hint ? strlen(hint) : 0;
		c8* tmp = new c8[strlen(message) + hs + 14];
		sprintf(tmp, "%s%s%s\n", message, hint ? ":" : " ", hint ? hint : " ");
		OutputDebugString(tmp);
		delete [] tmp;
	}

	//! prints a debuginfo string (unicode)
	void Debuginfo::print(const wchar_t* message, const wchar_t* hint)
	{
		size_t hs = hint ? wcslen(hint) : 0;
		wchar_t* tmp = new wchar_t[wcslen(message) + hs + 14];
		swprintf(tmp, L"%s%s%s\n", message, hint ? L":" : L" ", hint ? hint : L" ");
		OutputDebugStringW(tmp);
		delete [] tmp;
	}

	//! shows a warning message box
	void Warning::show(const c8* warning, const c8* hint)
	{
		if (hint)
		{
			c8* tmp = new c8[strlen(warning) + strlen(hint) + 2];
			sprintf(tmp, "%s\n%s", warning, hint);
			MessageBoxA(0, tmp, "Warning!", MB_OK);
			delete [] tmp;
		}
		else
			MessageBox(0, warning, "Warning!", MB_OK);	
	}



	//! shows a warning message box in unicode form
	void Warning::show(const wchar_t* warning, const wchar_t* hint)
	{
		if (hint)
		{
			wchar_t* tmp = new wchar_t[wcslen(warning) + wcslen(hint) + 2];
			swprintf(tmp, L"%s\n%s", warning, hint);
			MessageBoxW(0, tmp, L"Warning!", MB_OK);
			delete [] tmp;
		}
		else
			MessageBoxW(0, warning, L"Warning!", MB_OK);	
	}



	//! prints a warning into the warning log
	void Warning::print(const c8* message, const c8* hint)
	{
		size_t hs = hint ? strlen(hint) : 0;
		c8* tmp = new c8[strlen(message) + hs + 14];
		sprintf(tmp, "Warning: %s%s%s\n", message, hint ? ":" : " ", hint ? hint : " ");
		OutputDebugString(tmp);
		delete [] tmp;
	}



	u32 Timer::getTime()
	{
		return GetTickCount();
		//return timeGetTime();
	}

} // end namespace os


#else

	// TODO: code a linux/unix/mac version

#endif



namespace os
{
	// our Randomizer is not really os specific, so we
	// code one for all, which should work on every platform the same,
	// which is desireable.

	s32 Randomizer::seed = 0x0f0f0f0f;

	//! generates a pseudo random number
	s32 Randomizer::rand()
	{
		const s32 m = 2147483399;	// a non-Mersenne prime
		const s32 a = 40692;		// another spectral success story
		const s32 q = 52774;		// m/a
		const s32 r = 3791;			// m%a; again less than q

		seed = a * (seed%q) - r* (seed/q);
		if (seed<0) seed += m;

		return seed;
	}

	//! resets the randomizer
	void Randomizer::reset()
	{
		seed = 0x0f0f0f0f;
	}

} // end namespace os
} // end namespace klib