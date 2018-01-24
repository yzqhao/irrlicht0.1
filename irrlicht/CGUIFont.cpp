#include "CGUIFont.h"
#include "os.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIFont::CGUIFont(video::IVideoDriver* driver)
: Driver(driver), Positions(382), Texture(0), WrongCharacter(0)
{
	#ifdef _DEBUG
	setDebugName("CGUIFont");
	#endif

	if (Driver)
		Driver->grab();
}



//! destructor
CGUIFont::~CGUIFont()
{
	if (Driver)
		Driver->drop();

	if (Texture)
		Texture->drop();
}



//! loads a font file
bool CGUIFont::load(io::IReadFile* file)
{
	if (!Driver)
		return false;

	return loadTexture(Driver->getTexture(file));
}


//! loads a font file
bool CGUIFont::load(const c8* filename)
{
	if (!Driver)
		return false;

	return loadTexture(Driver->getTexture(filename));
}



//! load & prepare font from ITexture
bool CGUIFont::loadTexture(video::ITexture* texture)
{
	if (!texture)
		return false;

	Texture = texture;
	Texture->grab();

	s32 pitch = texture->getPitch();
	core::dimension2d<s32> size = texture->getDimension();

	s32 lowerRightPostions = 0;

	if (texture->getColorFormat() == video::EHCF_R5G5B5)
	{
        s16* p = (s16*)texture->lock();
		if (!p)
		{
			os::Warning::print("Could not lock texture while preparing texture for a font.");
			return false;
		}

		s16 colorTopLeft = *p;
		s16 colorLowerRight = *(p+1);
		s16 colorBackGround = *(p+2);
		s16 colorBackGroundWithAlphaTrue = (0x1<<15) | (~(0x1<<15) & colorBackGround);
		s16 colorBackGroundWithAlphaFalse = (0x0<<15) | (~(0x1<<15) & colorBackGround);
		s16 colorFont = (0x1<<15) | (~(0x1<<15) & video::RGB16(255,255,255));

		*(p+1) = colorBackGround;
		*(p+2) = colorBackGround;

		// start parsing

		core::position2d<s32> pos(0,0);

		c8* row = (c8*)((void*)p);

		for (pos.Y=0; pos.Y<size.Height; ++pos.Y)
		{
			p = (s16*)((void*)row);

			for (pos.X=0; pos.X<size.Width; ++pos.X)
			{
				if (*p == colorTopLeft)
				{
					*p = colorBackGroundWithAlphaFalse;
					Positions.push_back(core::rectEx<s32>(pos, pos));
				}
				else
				if (*p == colorLowerRight)
				{
					if (Positions.size()<=(u32)lowerRightPostions)
					{
						texture->unlock();
						return false;
					}

					*p = colorBackGroundWithAlphaFalse;
					Positions[lowerRightPostions].LowerRightCorner = pos;
					++lowerRightPostions;
				}
				else 
				if (*p == colorBackGround)
					*p = colorBackGroundWithAlphaFalse;
				else
					*p = colorFont;


				++p;
			}

			row += pitch;
		}

   
		// Positions parsed.

		texture->unlock();

		// output warnings
		if (!lowerRightPostions || !Positions.size())
			os::Warning::print("The amount of upper corner pixels or lower corner pixels is == 0, font file may be corrupted.");
		else
		if (lowerRightPostions != (s32)Positions.size())
			os::Warning::print("The amount of upper corner pixels and the lower corner pixels is not equal, font file may be corrupted.");

	}

	if (Positions.size() > 127)
		WrongCharacter = 127;

	return (!Positions.empty() && lowerRightPostions);
}


//! returns the dimension of a text
core::dimension2d<s32> CGUIFont::getDimension(const wchar_t* text)
{
	core::dimension2d<s32> dim(0, Positions[0].getHeight());

	u32 n;

	for(const wchar_t* p = text; *p; ++p)
	{
		n = (*p) - 32;
		if ( n > Positions.size())
			n = WrongCharacter;

		dim.Width += Positions[n].getWidth();
	}

	return dim;
}



//! draws an text and clips it to the specified rectangle if wanted
void CGUIFont::draw(const wchar_t* text, const core::rectEx<s32>& position, video::Color color, bool hcenter, bool vcenter, const core::rectEx<s32>* clip)
{
	if (!Driver)
		return;

	core::dimension2d<s32> textDimension;
	core::position2d<s32> offset = position.UpperLeftCorner;

    if (hcenter || vcenter)
	{
		textDimension = getDimension(text);

		if (hcenter)
			offset.X = ((position.getWidth() - textDimension.Width)>>1) + offset.X;

		if (vcenter)
			offset.Y = ((position.getHeight() - textDimension.Height)>>1) + offset.Y;
	}

	u32 n;
	
	while(*text)
	{
		n = (*text) - 32;
		if ( n > Positions.size())
			n = WrongCharacter;

		Driver->draw2DImage(Texture, offset, Positions[n], clip, color, true);
        
		offset.X += Positions[n].getWidth();

		++text;
	}
}


} // end namespace gui
} // end namespace irr
