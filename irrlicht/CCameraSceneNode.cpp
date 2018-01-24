#include "CCameraSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "os.h"

namespace irr
{
namespace scene
{


//! constructor
CCameraSceneNode::CCameraSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, 
	const core::vector3df& position, const core::vector3df& lookat)
	: ICameraSceneNode(parent, mgr, id, position, core::vector3df(0.0f, 0.0f, 0.0f),
			core::vector3df(1.0f, 1.0f, 1.0f)) 
{
	BBox.reset(0,0,0);

	// set default view

	UpVector.set(0.0f, 1.0f, 0.0f);
	Target.set(lookat);

	// set default projection

	fovy = core::PI / 2.5f;	// Field of view, in radians. 
	aspect = 4.0f / 3.0f;	// Aspect ratio. 
	zn = 1.0f;				// value of the near view-plane. 
	zf = 3000.0f;			// Z-value of the far view-plane. 

	video::IVideoDriver* d = mgr->getVideoDriver();
	if (d) aspect = ((f32)d->getScreenSize().Width / d->getScreenSize().Height);

	recalculateProjectionMatrix();
	recalculateViewArea();
}



//! destructor
CCameraSceneNode::~CCameraSceneNode()
{
}



//! Sets the projection matrix of the camera. The core::matrix4 class has some methods
//! to build a projection matrix. e.g: core::matrix4::buildProjectionMatrixPerspectiveFovLH
//! \param projection: The new projection matrix of the camera. 
void CCameraSceneNode::setProjectionMatrix(const core::matrix4& projection)
{
	Projection = projection;
}



//! Gets the current projection matrix of the camera
//! \return Returns the current projection matrix of the camera.
const core::matrix4& CCameraSceneNode::getProjectionMatrix()
{
	return Projection;
}



//! Gets the current view matrix of the camera
//! \return Returns the current view matrix of the camera.
const core::matrix4& CCameraSceneNode::getViewMatrix()
{
	return View;
}



//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for 
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addMeshViewerCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever. 
bool CCameraSceneNode::OnEvent(SEvent event)
{
	return false;
}



//! sets the look at target of the camera
//! \param pos: Look at target of the camera.
void CCameraSceneNode::setTarget(const core::vector3df& pos)
{
	Target = pos;
}



//! Gets the current look at target of the camera
//! \return Returns the current look at target of the camera
core::vector3df CCameraSceneNode::getTarget() const
{
	return Target;
}



//! sets the up vector of the camera
//! \param pos: New upvector of the camera.
void CCameraSceneNode::setUpVector(const core::vector3df& pos)
{
	UpVector = pos;
}



//! Gets the up vector of the camera.
//! \return Returns the up vector of the camera.
core::vector3df CCameraSceneNode::getUpVector() const
{
	return UpVector;
}


f32 CCameraSceneNode::getNearValue()
{
	return zn;
}

f32 CCameraSceneNode::getFarValue()
{
	return zf;
}

f32 CCameraSceneNode::getAspectRatio()
{
	return aspect;
}

f32 CCameraSceneNode::getFOV()
{
	return fovy;
}

void CCameraSceneNode::setNearValue(f32 f)
{
	zn = f;
	recalculateProjectionMatrix();
}

void CCameraSceneNode::setFarValue(f32 f)
{
	zf = f;
	recalculateProjectionMatrix();
}

void CCameraSceneNode::setAspectRatio(f32 f)
{
	aspect = f;
	recalculateProjectionMatrix();
}

void CCameraSceneNode::setFOV(f32 f)
{
	fovy = f;
	recalculateProjectionMatrix();
}

void CCameraSceneNode::recalculateProjectionMatrix()
{
	Projection.buildProjectionMatrixPerspectiveFovLH(fovy, aspect, zn, zf);
	recalculateViewArea();
}


//! prerender
void CCameraSceneNode::OnPreRender()
{
	if (SceneManager->getActiveCamera() == this)
	{
		SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
	}

	ISceneNode::OnPreRender();
	recalculateViewArea();
}


//! post render event
void CCameraSceneNode::OnPostRender()
{
	ISceneNode::OnPostRender(os::Timer::getTime());
}



//! render
void CCameraSceneNode::render()
{	
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	if (!driver)
		return;

	driver->setTransform(video::TS_PROJECTION, Projection);

	// if upvector and vector to the target are the same, we have a
	// problem. so solve this problem:

	core::vector3df pos = getAbsolutePosition();
	core::vector3df tgtv = Target - pos;
	tgtv.normalize();

	core::vector3df up = UpVector;
	up.normalize();

	f32 dp = tgtv.dotProduct(up);
	if ((dp > -1.0001f && dp < -0.9999f) ||
		(dp < 1.0001f && dp > 0.9999f))
		up.X += 1.0f;

	View.buildCameraLookAtMatrixLH(pos, Target, up);

	driver->setTransform(video::TS_VIEW, View);
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CCameraSceneNode::getBoundingBox() const
{
	return BBox;
}



//! returns the view frustrum. needed sometimes by bsp or lod render nodes.
const SViewFrustrum* CCameraSceneNode::getViewFrustrum()
{
	return &ViewArea;
}


void CCameraSceneNode::recalculateViewArea()
{
	core::vector3df pos = getAbsolutePosition();
	core::vector3df nvect = Target - pos;
	nvect.normalize();

	ViewArea.planes[SViewFrustrum::CVA_FAR_PLANE].setPlane(pos + nvect* zf, nvect);
	ViewArea.planes[SViewFrustrum::CVA_NEAR_PLANE].setPlane(pos + nvect* zn, nvect * -1.0f);

	f64 angle = fovy * core::GRAD_PI / 2.0f;
	f64 angle2 = angle / aspect;

	// left Side

	ViewArea.leftFarUp = Target;
	ViewArea.leftFarUp.rotateXZBy(angle, pos);

	ViewArea.leftFarDown = ViewArea.leftFarUp;

	ViewArea.leftFarUp.rotateYZBy(angle2, pos);
	ViewArea.planes[SViewFrustrum::CVA_FAR_PLANE].getIntersectionWithLine(pos, pos - ViewArea.leftFarUp, ViewArea.leftFarUp);

	ViewArea.leftFarDown.rotateYZBy(-angle2, pos);
	ViewArea.planes[SViewFrustrum::CVA_FAR_PLANE].getIntersectionWithLine(pos, pos - ViewArea.leftFarDown, ViewArea.leftFarDown);

	// right Side

	ViewArea.rightFarUp = Target;
	ViewArea.rightFarUp.rotateXZBy(-angle, pos);

	ViewArea.rightFarDown = ViewArea.rightFarUp;

	ViewArea.rightFarUp.rotateYZBy(angle2, pos);
	ViewArea.planes[SViewFrustrum::CVA_FAR_PLANE].getIntersectionWithLine(pos, pos - ViewArea.rightFarUp, ViewArea.rightFarUp);

	ViewArea.rightFarDown.rotateYZBy(-angle2, pos);
	ViewArea.planes[SViewFrustrum::CVA_FAR_PLANE].getIntersectionWithLine(pos, pos - ViewArea.rightFarDown, ViewArea.rightFarDown);

	// create 4 remaining planes from points

	// der normalvector einer ebene geht nach INNEN,
	// wenn man die punkte im uhrzeigersinn übergibt.

	ViewArea.planes[SViewFrustrum::CVA_BOTTOM_PLANE].setPlane(pos, ViewArea.leftFarDown, ViewArea.rightFarDown);
	ViewArea.planes[SViewFrustrum::CVA_TOP_PLANE].setPlane(ViewArea.rightFarUp, ViewArea.leftFarUp, pos);
	ViewArea.planes[SViewFrustrum::CVA_LEFT_PLANE].setPlane(pos, ViewArea.leftFarUp, ViewArea.leftFarDown);
	ViewArea.planes[SViewFrustrum::CVA_RIGHT_PLANE].setPlane(ViewArea.rightFarUp, pos, ViewArea.rightFarDown);

	// eine boundingbox drumherum

	ViewArea.box.reset(pos);
	ViewArea.box.addInternalPoint(ViewArea.leftFarUp);
	ViewArea.box.addInternalPoint(ViewArea.leftFarDown);
	ViewArea.box.addInternalPoint(ViewArea.rightFarUp);
	ViewArea.box.addInternalPoint(ViewArea.rightFarDown);

	//viewAreaIsInvalid = false;

	for (s32 i=SViewFrustrum::CVA_LEFT_PLANE; i<SViewFrustrum::CVA_PLANE_COUNT; ++i)
	{
		if (nvect.dotProduct(ViewArea.planes[i].Normal) > 0.0f)
		{
			ViewArea.planes[i].Normal.invert();
			ViewArea.planes[i].recalculateD();
		}
	}
}



} // end namespace
} // end namespace
