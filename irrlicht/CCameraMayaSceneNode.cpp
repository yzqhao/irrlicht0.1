#include "CCameraMayaSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"

namespace irr
{
namespace scene
{

//! constructor
CCameraMayaSceneNode::CCameraMayaSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
	 f32 rs, f32 zs, f32 ts)
: CCameraSceneNode(parent, mgr, id)
{
	zooming = false;
	rotating = false;
	moving = false;
	translating = false;
	zoomSpeed = zs;
	rotateSpeed = rs;
	translateSpeed = ts;
	currentZoom = 70.0f;
	//Pos.set(0.0f, 70.0f, 0.0f);
	Target.set(0.0f, 0.0f, 0.0f);
	rotX = 0;
	rotY = 0;
	oldTargetX = Target.X;
	oldTargetY = Target.Y;
	oldTargetZ = Target.Z;

	allKeysUp();
	recalculateViewArea();
}


//! destructor
CCameraMayaSceneNode::~CCameraMayaSceneNode()
{
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for 
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addMeshViewerCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever. 
bool CCameraMayaSceneNode::OnEvent(SEvent event)
{
	if (event.EventType != EET_MOUSE_INPUT_EVENT)
		return false;

	switch(event.MouseInput.Event)
	{
	case EMIE_LMOUSE_PRESSED_DOWN:
		MouseKeys[0] = true;
		break;
	case EMIE_RMOUSE_PRESSED_DOWN:
		MouseKeys[2] = true;
		break;
	case EMIE_MMOUSE_PRESSED_DOWN:
		MouseKeys[1] = true;
		break;
	case EMIE_LMOUSE_LEFT_UP:
		MouseKeys[0] = false;
		break;
	case EMIE_RMOUSE_LEFT_UP:
		MouseKeys[2] = false;
		break;
	case EMIE_MMOUSE_LEFT_UP:
		MouseKeys[1] = false;
		break;
	case EMIE_MOUSE_MOVED:
		{
			video::IVideoDriver* driver = SceneManager->getVideoDriver();
			if (driver)
			{
				core::dimension2d<s32> ssize = SceneManager->getVideoDriver()->getScreenSize();
				MousePos.X = event.MouseInput.X / (f32)ssize.Width;
				MousePos.Y = event.MouseInput.Y / (f32)ssize.Height;
			}
		}
		break;
	}
	return true;
}

//! onPostRender is called just after rendering the whole scene.
//! nodes may calculate or store animations here, and may do other useful things,
//! dependent on what they are.
void CCameraMayaSceneNode::OnPostRender(u32 timeMs)
{
	animate();

	AnimatedRelativeTransformation = RelativeTransformation;
	AnimatedRelativeTransformation.setTranslation(Pos);
	updateAbsolutePosition();

	// This scene node cannot be animated by scene node animators.
}


bool CCameraMayaSceneNode::isMouseKeyDown(s32 key)
{
	return MouseKeys[key];
}



void CCameraMayaSceneNode::animate()
{
	//Alt + LM = Rotieren um KameraDrehpunkt
	//Alt + LM + MM = Dolly vor/zurück in Kamerablickrichtung (Geschwindigkeit % von Abstand Kamera zu Drehpunkt - Maximalweg zum Kameradrehpunkt)
	//Alt + MM = Verschieben in Kameraebene (Bildmitte hängt vom Tempo ungefähr am Mauszeiger)

	const SViewFrustrum* va = getViewFrustrum();

	f32 nRotX = rotX;
	f32 nRotY = rotY;
	f32 nZoom = currentZoom;

	if (isMouseKeyDown(0) && isMouseKeyDown(2))
	{
		if (!zooming)
		{
			zoomStartX = MousePos.X;
			zoomStartY = MousePos.Y;
			zooming = true;
			nZoom = currentZoom;
		}
		else
		{
			f32 old = nZoom;
			nZoom += (zoomStartX - MousePos.X) * zoomSpeed;

			if (nZoom < 0)
				nZoom = old;
		}
	}
	else
	{
		if (zooming)
		{
			f32 old = currentZoom;
			currentZoom = currentZoom + (zoomStartX - MousePos.X ) * zoomSpeed;
			nZoom = currentZoom;

			if (nZoom < 0)
				nZoom = currentZoom = old;
		}

		zooming = false;
	}

	// Translation ---------------------------------

	core::vector3df translate(oldTargetX, oldTargetY, oldTargetZ);

	core::vector3df tvectX = Pos - Target;
	tvectX = tvectX.crossProduct(UpVector);
	tvectX.normalize();

	core::vector3df tvectY = (va->leftFarDown - va->rightFarDown);
	tvectY = tvectY.crossProduct(UpVector.Y > 0 ? Pos - Target : Target - Pos);
	tvectY.normalize();
	

	if (isMouseKeyDown(2) && !zooming)
	{
		if (!translating)
		{
			translateStartX = MousePos.X;
			translateStartY = MousePos.Y;
			translating = true;
		}
		else
		{
			translate +=	tvectX * (translateStartX - MousePos.X)*translateSpeed + 
								tvectY * (translateStartY - MousePos.Y)*translateSpeed;
		}
	}
	else
	{
		if (translating)
		{
			translate +=	tvectX * (translateStartX - MousePos.X)*translateSpeed + 
								tvectY * (translateStartY - MousePos.Y)*translateSpeed;
			oldTargetX = translate.X;
			oldTargetY = translate.Y;
			oldTargetZ = translate.Z;
		}

		translating = false;
	}

	// Rotation ------------------------------------

	if (isMouseKeyDown(0) && !zooming)
	{
		if (!rotating)
		{
			rotateStartX = MousePos.X;
			rotateStartY = MousePos.Y;
			rotating = true;
			nRotX = rotX;
			nRotY = rotY;
		}
		else
		{
			nRotX += (rotateStartX - MousePos.X) * rotateSpeed;
			nRotY += (rotateStartY - MousePos.Y) * rotateSpeed;
		}
	}
	else
	{
		if (rotating)
		{
			rotX = rotX + (rotateStartX - MousePos.X) * rotateSpeed;
			rotY = rotY + (rotateStartY - MousePos.Y) * rotateSpeed;
			nRotX = rotX;
			nRotY = rotY;
		}

		rotating = false;
	}

	// Set Pos ------------------------------------

	Target = translate;

	Pos.X = nZoom + Target.X;
	Pos.Y = Target.Y;
	Pos.Z = Target.Z;

	Pos.rotateXYBy(nRotY, Target);
	Pos.rotateXZBy(-nRotX, Target);

	// Rotation Error ----------------------------

	if (nRotY < 0.0f)
		nRotY *= -1.0f;
	
	nRotY = (f32)fmod(nRotY, (f32)360.0);
	
	
	if (nRotY >= 90.0f && nRotY <= 270.0f)
		UpVector.set(0, -1, 0);
	else
		UpVector.set(0, 1, 0);
}


void CCameraMayaSceneNode::allKeysUp()
{
	for (s32 i=0; i<3; ++i)
		MouseKeys[i] = false;
}


} // end namespace
} // end namespace