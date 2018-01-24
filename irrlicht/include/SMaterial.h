// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __S_MATERIAL_H_INCLUDED__
#define __S_MATERIAL_H_INCLUDED__

#include "Color.h"
#include "ITexture.h"

namespace irr
{
namespace video
{
	//! Abstracted and easy to use standard material modes.
	enum E_MATERIAL_TYPE
	{
		//! Standard solid material. Only first texture is used.
		EMT_SOLID = 0,					

		//! A transparent material. Only the first texture is used.
		//! The new color is calculated by simply adding the source color and
		//! the dest color. This means if for example a billboard using a texture with 
		//! black background and a red circle on it is drawed with this material, the
		//! result is that only the red circle will be drawn a little bit transparent,
		//! and everything which was black is 100% transparent and not visible.
		//! This material type is useful for e.g. particle effects.
		EMT_TRANSPARENT_ADD_COLOR,

		//! Makes the material transparent based on the texture alpha channel.
		//! Only first texture is used.
		EMT_TRANSPARENT_ALPHA_CHANNEL,	

		//! Material type with standard lightmap technique: 
		//! There should be 2 textures: The first texture layer is a diffuse map,
		//! the second is a light map. Vertex light is ignored.
		EMT_LIGHTMAP,					

		//! This value is not used. it only forces this enumeration to compile in 32 bit. 
		EMT_FORCE_32BIT = 0x7fffffff	
	};

	//! Material flags
	enum E_MATERIAL_FLAG
	{
		//! Draw as wireframe or filled triangles? Default: false
		EMF_WIREFRAME = 0,

		//! Flat or Gouraud shading? Default: true
		EMF_GOURAUD_SHADING,

		//! Will this material be lighted? Default: true
		EMF_LIGHTING,

		//! Is the ZBuffer enabled? Default: true
		EMF_ZBUFFER,

		//! May be written to the zbuffer or is it readonly. Default: true
		//! This flag is ignored, if the material type is a transparent type.
		EMF_ZWRITE_ENABLE,

		//! Is backfaceculling enabled? Default: true
		EMF_BACK_FACE_CULLING,

		//! Is bilinear filtering enabled? Default: true
		EMF_BILINEAR_FILTER,

		//! This is not a flag, but a value indicating how much flags there are.
		EMF_MATERIAL_FLAG_COUNT 
	};

	const s32 MATERIAL_MAX_TEXTURES = 2;


	//! struct for holding a material
	struct SMaterial
	{
		//! default constructor, creates a solid material with standard colors
		SMaterial()
		: AmbientColor(255,255,255,255), DiffuseColor(255,255,255,255),
		EmissiveColor(0,0,0,0), SpecularColor(0,0,0,0), Texture1(0), Texture2(0),
			MaterialType(EMT_SOLID), Wireframe(false), Lighting(true),
			ZBuffer(true), ZWriteEnable(true), BackfaceCulling(true),
			GouraudShading(true), Shininess(0.0f), BilinearFilter(true)
		{}

		//! Type of the material. Specifies how everything is blended together
		E_MATERIAL_TYPE MaterialType;	

		//! How much ambient light (a global light) is reflected by this material.
		//! The default is full white, meaning objects are completely globally illuminated.
		//! Reduce this if you want to see diffuse or specular light effects, or change
		//! the blend of colours to make the object have a base colour other than white.
		Color AmbientColor;		

		//! How much diffuse light coming from a light source is reflected by this material.
		//! The default is full white.
		Color DiffuseColor;		

		//! Light emitted by this material. Default is to emitt no light.
		Color EmissiveColor;	

		//! How much specular light (highlights from a light) is reflected. 
		//! The default is to reflect no specular light. 
		Color SpecularColor;	

		//! Value affecting the size of specular highlights.
		f32 Shininess;			

		//! Texture layer union.
		union
		{
			struct
			{
				//! Primary texture layer.
				ITexture* Texture1;	

				//! Secondary texture layer
				ITexture* Texture2;	
			};

			ITexture* Textures[MATERIAL_MAX_TEXTURES];
		};

		//! material flag union. This enables the user to access the
		//! material flag using e.g: material.Wireframe = true or
		//! material.flag[EMF_WIREFRAME] = true;
		union
		{
			struct
			{
				//! Draw as wireframe or filled triangles? Default: false
				bool Wireframe;

				//! Flat or Gouraud shading? Default: true
				bool GouraudShading;				

				//! Will this material be lighted? Default: true
				bool Lighting;						

				//! Is the ZBuffer enabled? Default: true
				bool ZBuffer;						

				//! May be written to the zbuffer or is it readonly. Default: true
				//! This flag is ignored, if the MaterialType is a transparent type.
				bool ZWriteEnable;					

				//! Is backfaceculling enabled? Default: true
				bool BackfaceCulling;				

				//! Is bilinear filtering enabled? Default: true
				bool BilinearFilter;				
			};

			bool Flags[EMF_MATERIAL_FLAG_COUNT];
		};


		//! Determinates if the material is somehow transparent.
		//! \return Returns true if material is transparent, false if not.
		inline bool isTransparent()
		{
			return MaterialType == EMT_TRANSPARENT_ADD_COLOR ||
					MaterialType == EMT_TRANSPARENT_ALPHA_CHANNEL;
		}
	};

} // end namespace video
} // end namespace irr

#endif