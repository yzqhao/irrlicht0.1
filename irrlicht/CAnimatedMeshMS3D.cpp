#include "CAnimatedMeshMS3D.h"
#include "os.h"
#include <string.h>

namespace irr
{
namespace scene
{


// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

// File header
struct MS3DHeader
{
	c8 ID[10];
	s32 Version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	u8 Flags;
	f32 Vertex[3];
	s8 BoneID;
	u8 RefCount;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	u16 Flags;
	u16 VertexIndices[3];
	f32 VertexNormals[3][3];
	f32 S[3], T[3];
	u8 SmoothingGroup;
	u8 GroupIndex;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
    s8 Name[32];
    f32 Ambient[4];
    f32 Diffuse[4];
    f32 Specular[4];
    f32 Emissive[4];
    f32 Shininess;	// 0.0f - 128.0f
    f32 Transparency;	// 0.0f - 1.0f
    u8 Mode;	// 0, 1, 2 is unused now
    s8 Texture[128];
    s8 Alphamap[128];
} PACK_STRUCT;

//	Joint information
struct MS3DJoint
{
	u8 Flags;
	s8 Name[32];
	s8 ParentName[32];
	f32 Rotation[3];
	f32 Translation[3];
	u16 NumRotationKeyframes;
	u16 NumTranslationKeyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	f32 Time;
	f32 Parameter[3];
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT


//! constructor
CAnimatedMeshMS3D::CAnimatedMeshMS3D()
{
}



//! destructor
CAnimatedMeshMS3D::~CAnimatedMeshMS3D()
{
}



//! loads an md2 file
bool CAnimatedMeshMS3D::loadFile(io::IReadFile* file)
{
	if (!file)
		return false;

	// find file size

	size_t fileSize = file->getSize();

	// read whole file

	u8* buffer = new u8[fileSize];
	size_t read = (s32)file->read(buffer, fileSize);
	if (read != fileSize)
	{
		delete [] buffer;
		os::Warning::print("Could not read full file. Loading failed", file->getFileName());
		return false;
	}

	// read header

	const u8 *pPtr = (u8*)((void*)buffer);
	MS3DHeader *pHeader = (MS3DHeader*)pPtr;
	pPtr += sizeof(MS3DHeader);

	if ( strncmp( pHeader->ID, "MS3D000000", 10 ) != 0 )
	{
		delete [] buffer;
		os::Warning::print("Not a valid Milkshape3D Model File. Loading failed", file->getFileName());
		return false;	// Not A Valid Milkshape3D Model File
	}

	if ( pHeader->Version < 3 || pHeader->Version > 4 )
	{
		delete [] buffer;
		os::Warning::print("Only Milkshape3D version 1.3 and 1.4 is supported. Loading failed", file->getFileName());
		return false;	// Unhandled File Version. Only Milkshape3D Version 1.3 And 1.4 Is Supported.
	}

	// read vertices

	u16 numVertices = *(u16*)pPtr;
	pPtr += sizeof(u16);
	Vertices.set_used(numVertices);
	BoneIDs.set_used(numVertices);

	for (s32 i=0; i<numVertices; ++i)
	{
		MS3DVertex *pVertex = (MS3DVertex*)pPtr;

		BoneIDs[i] = pVertex->BoneID;
		Vertices[i].Pos.X = pVertex->Vertex[0];
		Vertices[i].Pos.Y = pVertex->Vertex[1];
		Vertices[i].Pos.Z = pVertex->Vertex[2];

		pPtr += sizeof( MS3DVertex );
	}

	// read triangles

	u16 numTriangles = *(u16*)pPtr;
	Indices.set_used(numTriangles*3);
	pPtr += sizeof(u16);

	for (s32 i = 0; i<numTriangles; ++i)
	{
		MS3DTriangle *pTriangle = (MS3DTriangle*)pPtr;

		Indices[i*3 +0] = pTriangle->VertexIndices[0];
		Indices[i*3 +1] = pTriangle->VertexIndices[1];
		Indices[i*3 +2] = pTriangle->VertexIndices[2];

		for (s32 iv=0; iv<3; ++iv)
		{
			video::S3DVertex& v = Vertices[Indices[i*3+iv]];

			v.Normal.X = pTriangle->VertexNormals[iv][0];
			v.Normal.Y = pTriangle->VertexNormals[iv][1];
			v.Normal.Z = pTriangle->VertexNormals[iv][2];

			v.TCoords.X = pTriangle->S[iv];
			v.TCoords.Y = 1.0f - pTriangle->T[iv];
		}

		pPtr += sizeof(MS3DTriangle);
	}

	// TODO: read Materials and Groups.
	// TODO: calculate bounding box

	delete [] buffer;
	return true;
}



//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
s32 CAnimatedMeshMS3D::getFrameCount()
{
	return 1;
}



//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
IMesh* CAnimatedMeshMS3D::getMesh(s32 frame, s32 detailLevel)
{
	return this;
}



//! returns amount of mesh buffers.
s32 CAnimatedMeshMS3D::getMeshBufferCount()
{
	return 1;
}



//! returns pointer to a mesh buffer
IMeshBuffer* CAnimatedMeshMS3D::getMeshBuffer(s32 nr)
{
	return this;
}



//! returns the material of this meshbuffer
const video::SMaterial& CAnimatedMeshMS3D::getMaterial() const
{
	return Material;
}



//! returns the material of this meshbuffer
video::SMaterial& CAnimatedMeshMS3D::getMaterial()
{
	return Material;
}



//! returns pointer to vertices
const void* CAnimatedMeshMS3D::getVertices() const
{
	return Vertices.const_pointer();
}


//! returns pointer to vertices
void* CAnimatedMeshMS3D::getVertices()
{
	return Vertices.pointer();
}


//! returns which type of vertex data is stored.
video::E_VERTEX_TYPE CAnimatedMeshMS3D::getVertexType() const
{
	return video::EVT_STANDARD;
}



//! returns amount of vertices
s32 CAnimatedMeshMS3D::getVertexCount() const
{
	return Vertices.size();
}



//! returns pointer to Indices
const u16* CAnimatedMeshMS3D::getIndices() const
{
	return Indices.const_pointer();
}



//! returns amount of indices
s32 CAnimatedMeshMS3D::getIndexCount() const
{
	return Indices.size();
}


//! returns an axis aligned bounding box
const core::aabbox3d<f32>& CAnimatedMeshMS3D::getBoundingBox() const
{
	return BoundingBox;
}


//! sets a flag of all contained materials to a new value
void CAnimatedMeshMS3D::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
	Material.Flags[flag] = newvalue;
}



} // end namespace scene
} // end namespace irr
