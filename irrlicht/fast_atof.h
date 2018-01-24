// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

// provides a fast function for converting a string into a f32,
// about 6 times faster than atof.

#ifndef __FAST_A_TO_F_H_INCLUDED__
#define __FAST_A_TO_F_H_INCLUDED__

#include <stdlib.h>

namespace irr
{
namespace core
{

const f32 fast_atof_table[] =	{
										0.f,
										0.1f,
										0.01f,
										0.001f,
										0.0001f,
										0.00001f,
										0.000001f,
										0.0000001f,
										0.00000001f,
										0.000000001f,
										0.0000000001f,
										0.00000000001f,
										0.000000000001f,
										0.0000000000001f,
										0.00000000000001f,
										0.000000000000001f
									};

inline f32 fast_atof(const c8* c)
{
	bool inv = false;
	c8 *t;
	f32 f;

	if (*c=='-')
	{
		c++;
		inv = true;
	}

	f = (f32)strtol(c, &t, 10);

	c = t;

	if (*c == '.')
	{
		c++;

		f32 pl = (f32)strtol(c, &t, 10);
		pl *= fast_atof_table[t-c];

		f += pl;

		c = t;

		if (*c == 'e')
		{
			++c;
			f32 exp = (f32)strtol(c, &t, 10);
			f *= (f32)pow(10, exp);
		}
	}

	if (inv)
		f *= -1.0f;
	
	return f;
}

} // end namespace core
}// end namespace irr

#endif