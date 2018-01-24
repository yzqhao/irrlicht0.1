// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __S_LIGHT_H_INCLUDED__
#define __S_LIGHT_H_INCLUDED__

#include "Color.h"

namespace irr
{
namespace video  
{

//! structure for holding data describing a dynamic point light.
/** ambient light and point lights are the only light supported 
by the irrlicht engine.
*/
struct SLight
{
	SLight() : AmbientColor(0.0f,0.0f,0.0f), DiffuseColor(1.0f, 1.0f, 1.0f), 
		SpecularColor(0.0f,0.0f,0.0f), Position(0.0f, 0.0f, 0.0f), Range(100.0f),
		ConstantAttenuation(1.0f), LinearAttenuation(0.0f), QuadraticAttenuation(0.0f) {};

	//! Ambient color emitted by the light
	Colorf AmbientColor; 

	//! Diffuse color emitted by the light.
	Colorf DiffuseColor; 

	//! Specular color emitted by the light. 
	Colorf SpecularColor; 

	//! Position of the light.
	core::vector3df Position; 

	//! Range of the light.
	f32 Range; 

	//! Constant attenuatuion value specifying how the light intensity changes
	//! over distance. Must be between 0.0 and 1.0: 1.0 means never attenuate, 
	//! 0.0 is complete attenuation.
	f32 ConstantAttenuation; 

	//! Linear attenuatuion value specifying how the light intensity changes over distance.
	//! Must be a positive value. 1.0 means attenuate evenly over the distance.
	f32 LinearAttenuation; 

	//! Quadratic attenuatuion value specifying how the light intensity changes over distance.
	//! Must be a positive value. Adds a curvature to the attenuation formula. 
	f32 QuadraticAttenuation; 
};

} // end namespace video
} // end namespace irr

#endif