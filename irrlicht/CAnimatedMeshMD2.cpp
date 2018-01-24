#include "CAnimatedMeshMD2.h"
#include "os.h"
#include "color.h"
#include "IReadFile.h"

namespace irr
{
namespace scene
{

#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

	// structs needed to load the md2-format

	const s32 MD2_MAGIC_NUMBER = 844121161;
	const s32 MD2_VERSION		= 8;
	const s32 MD2_MAX_VERTS		= 2048;

	struct SMD2Header
	{
		s32 magic;
		s32 version;
		s32 skinWidth;
		s32 skinHeight;
		s32 frameSize;
		s32 numSkins;
		s32 numVertices;
		s32 numTexcoords;
		s32 numTriangles;	
		s32 numGlCommands;
		s32 numFrames;
		s32 offsetSkins;
		s32 offsetTexcoords;
		s32 offsetTriangles;
		s32 offsetFrames;
		s32 offsetGlCommands;
		s32 offsetEnd;
	} PACK_STRUCT;

	struct SMD2Vertex
	{
		u8 vertex[3];
		u8 lightNormalIndex;
	} PACK_STRUCT;

	struct SMD2Frame
	{
		f32	scale[3];
		f32	translate[3];
		s8	name[16];
		SMD2Vertex vertices[1];
	} PACK_STRUCT;

	struct SMD2Triangle
	{
		s16 vertexIndices[3];
		s16 textureIndices[3];
	} PACK_STRUCT;

	struct SMD2TextureCoordinate
	{
		s16 s;
		s16 t;
	} PACK_STRUCT;

	struct SMD2GLCommand
	{
		f32 s, t;
		s32 vertexIndex;
	} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT


const s32 Q2_VERTEX_NORMAL_TABLE_SIZE = 162;

const f32 Q2_VERTEX_NORMAL_TABLE[Q2_VERTEX_NORMAL_TABLE_SIZE][3] = {
	{-0.525731f, 0.000000f, 0.850651f}, 
	{-0.442863f, 0.238856f, 0.864188f}, 
	{-0.295242f, 0.000000f, 0.955423f}, 
	{-0.309017f, 0.500000f, 0.809017f}, 
	{-0.162460f, 0.262866f, 0.951056f}, 
	{0.000000f, 0.000000f, 1.000000f}, 
	{0.000000f, 0.850651f, 0.525731f}, 
	{-0.147621f, 0.716567f, 0.681718f}, 
	{0.147621f, 0.716567f, 0.681718f}, 
	{0.000000f, 0.525731f, 0.850651f}, 
	{0.309017f, 0.500000f, 0.809017f}, 
	{0.525731f, 0.000000f, 0.850651f}, 
	{0.295242f, 0.000000f, 0.955423f}, 
	{0.442863f, 0.238856f, 0.864188f}, 
	{0.162460f, 0.262866f, 0.951056f}, 
	{-0.681718f, 0.147621f, 0.716567f}, 
	{-0.809017f, 0.309017f, 0.500000f}, 
	{-0.587785f, 0.425325f, 0.688191f}, 
	{-0.850651f, 0.525731f, 0.000000f}, 
	{-0.864188f, 0.442863f, 0.238856f}, 
	{-0.716567f, 0.681718f, 0.147621f}, 
	{-0.688191f, 0.587785f, 0.425325f}, 
	{-0.500000f, 0.809017f, 0.309017f}, 
	{-0.238856f, 0.864188f, 0.442863f}, 
	{-0.425325f, 0.688191f, 0.587785f}, 
	{-0.716567f, 0.681718f, -0.147621f}, 
	{-0.500000f, 0.809017f, -0.309017f}, 
	{-0.525731f, 0.850651f, 0.000000f}, 
	{0.000000f, 0.850651f, -0.525731f}, 
	{-0.238856f, 0.864188f, -0.442863f}, 
	{0.000000f, 0.955423f, -0.295242f}, 
	{-0.262866f, 0.951056f, -0.162460f}, 
	{0.000000f, 1.000000f, 0.000000f}, 
	{0.000000f, 0.955423f, 0.295242f}, 
	{-0.262866f, 0.951056f, 0.162460f}, 
	{0.238856f, 0.864188f, 0.442863f}, 
	{0.262866f, 0.951056f, 0.162460f}, 
	{0.500000f, 0.809017f, 0.309017f}, 
	{0.238856f, 0.864188f, -0.442863f}, 
	{0.262866f, 0.951056f, -0.162460f}, 
	{0.500000f, 0.809017f, -0.309017f}, 
	{0.850651f, 0.525731f, 0.000000f}, 
	{0.716567f, 0.681718f, 0.147621f}, 
	{0.716567f, 0.681718f, -0.147621f}, 
	{0.525731f, 0.850651f, 0.000000f}, 
	{0.425325f, 0.688191f, 0.587785f}, 
	{0.864188f, 0.442863f, 0.238856f}, 
	{0.688191f, 0.587785f, 0.425325f}, 
	{0.809017f, 0.309017f, 0.500000f}, 
	{0.681718f, 0.147621f, 0.716567f}, 
	{0.587785f, 0.425325f, 0.688191f}, 
	{0.955423f, 0.295242f, 0.000000f}, 
	{1.000000f, 0.000000f, 0.000000f}, 
	{0.951056f, 0.162460f, 0.262866f}, 
	{0.850651f, -0.525731f, 0.000000f}, 
	{0.955423f, -0.295242f, 0.000000f}, 
	{0.864188f, -0.442863f, 0.238856f}, 
	{0.951056f, -0.162460f, 0.262866f}, 
	{0.809017f, -0.309017f, 0.500000f}, 
	{0.681718f, -0.147621f, 0.716567f}, 
	{0.850651f, 0.000000f, 0.525731f}, 
	{0.864188f, 0.442863f, -0.238856f}, 
	{0.809017f, 0.309017f, -0.500000f}, 
	{0.951056f, 0.162460f, -0.262866f}, 
	{0.525731f, 0.000000f, -0.850651f}, 
	{0.681718f, 0.147621f, -0.716567f}, 
	{0.681718f, -0.147621f, -0.716567f}, 
	{0.850651f, 0.000000f, -0.525731f}, 
	{0.809017f, -0.309017f, -0.500000f}, 
	{0.864188f, -0.442863f, -0.238856f}, 
	{0.951056f, -0.162460f, -0.262866f}, 
	{0.147621f, 0.716567f, -0.681718f}, 
	{0.309017f, 0.500000f, -0.809017f}, 
	{0.425325f, 0.688191f, -0.587785f}, 
	{0.442863f, 0.238856f, -0.864188f}, 
	{0.587785f, 0.425325f, -0.688191f}, 
	{0.688191f, 0.587785f, -0.425325f}, 
	{-0.147621f, 0.716567f, -0.681718f}, 
	{-0.309017f, 0.500000f, -0.809017f}, 
	{0.000000f, 0.525731f, -0.850651f}, 
	{-0.525731f, 0.000000f, -0.850651f}, 
	{-0.442863f, 0.238856f, -0.864188f}, 
	{-0.295242f, 0.000000f, -0.955423f}, 
	{-0.162460f, 0.262866f, -0.951056f}, 
	{0.000000f, 0.000000f, -1.000000f}, 
	{0.295242f, 0.000000f, -0.955423f}, 
	{0.162460f, 0.262866f, -0.951056f}, 
	{-0.442863f, -0.238856f, -0.864188f}, 
	{-0.309017f, -0.500000f, -0.809017f}, 
	{-0.162460f, -0.262866f, -0.951056f}, 
	{0.000000f, -0.850651f, -0.525731f}, 
	{-0.147621f, -0.716567f, -0.681718f}, 
	{0.147621f, -0.716567f, -0.681718f}, 
	{0.000000f, -0.525731f, -0.850651f}, 
	{0.309017f, -0.500000f, -0.809017f}, 
	{0.442863f, -0.238856f, -0.864188f}, 
	{0.162460f, -0.262866f, -0.951056f}, 
	{0.238856f, -0.864188f, -0.442863f}, 
	{0.500000f, -0.809017f, -0.309017f}, 
	{0.425325f, -0.688191f, -0.587785f}, 
	{0.716567f, -0.681718f, -0.147621f}, 
	{0.688191f, -0.587785f, -0.425325f}, 
	{0.587785f, -0.425325f, -0.688191f}, 
	{0.000000f, -0.955423f, -0.295242f}, 
	{0.000000f, -1.000000f, 0.000000f}, 
	{0.262866f, -0.951056f, -0.162460f}, 
	{0.000000f, -0.850651f, 0.525731f}, 
	{0.000000f, -0.955423f, 0.295242f}, 
	{0.238856f, -0.864188f, 0.442863f}, 
	{0.262866f, -0.951056f, 0.162460f}, 
	{0.500000f, -0.809017f, 0.309017f}, 
	{0.716567f, -0.681718f, 0.147621f}, 
	{0.525731f, -0.850651f, 0.000000f}, 
	{-0.238856f, -0.864188f, -0.442863f}, 
	{-0.500000f, -0.809017f, -0.309017f}, 
	{-0.262866f, -0.951056f, -0.162460f}, 
	{-0.850651f, -0.525731f, 0.000000f}, 
	{-0.716567f, -0.681718f, -0.147621f}, 
	{-0.716567f, -0.681718f, 0.147621f}, 
	{-0.525731f, -0.850651f, 0.000000f}, 
	{-0.500000f, -0.809017f, 0.309017f}, 
	{-0.238856f, -0.864188f, 0.442863f}, 
	{-0.262866f, -0.951056f, 0.162460f}, 
	{-0.864188f, -0.442863f, 0.238856f}, 
	{-0.809017f, -0.309017f, 0.500000f}, 
	{-0.688191f, -0.587785f, 0.425325f}, 
	{-0.681718f, -0.147621f, 0.716567f}, 
	{-0.442863f, -0.238856f, 0.864188f}, 
	{-0.587785f, -0.425325f, 0.688191f}, 
	{-0.309017f, -0.500000f, 0.809017f}, 
	{-0.147621f, -0.716567f, 0.681718f}, 
	{-0.425325f, -0.688191f, 0.587785f}, 
	{-0.162460f, -0.262866f, 0.951056f}, 
	{0.442863f, -0.238856f, 0.864188f}, 
	{0.162460f, -0.262866f, 0.951056f}, 
	{0.309017f, -0.500000f, 0.809017f}, 
	{0.147621f, -0.716567f, 0.681718f}, 
	{0.000000f, -0.525731f, 0.850651f}, 
	{0.425325f, -0.688191f, 0.587785f}, 
	{0.587785f, -0.425325f, 0.688191f}, 
	{0.688191f, -0.587785f, 0.425325f}, 
	{-0.955423f, 0.295242f, 0.000000f}, 
	{-0.951056f, 0.162460f, 0.262866f}, 
	{-1.000000f, 0.000000f, 0.000000f}, 
	{-0.850651f, 0.000000f, 0.525731f}, 
	{-0.955423f, -0.295242f, 0.000000f}, 
	{-0.951056f, -0.162460f, 0.262866f}, 
	{-0.864188f, 0.442863f, -0.238856f}, 
	{-0.951056f, 0.162460f, -0.262866f}, 
	{-0.809017f, 0.309017f, -0.500000f}, 
	{-0.864188f, -0.442863f, -0.238856f}, 
	{-0.951056f, -0.162460f, -0.262866f}, 
	{-0.809017f, -0.309017f, -0.500000f}, 
	{-0.681718f, 0.147621f, -0.716567f}, 
	{-0.681718f, -0.147621f, -0.716567f}, 
	{-0.850651f, 0.000000f, -0.525731f}, 
	{-0.688191f, 0.587785f, -0.425325f}, 
	{-0.587785f, 0.425325f, -0.688191f}, 
	{-0.425325f, 0.688191f, -0.587785f}, 
	{-0.425325f, -0.688191f, -0.587785f}, 
	{-0.587785f, -0.425325f, -0.688191f}, 
	{-0.688191f, -0.587785f, -0.425325f}, 
	};


//! constructor
CAnimatedMeshMD2::CAnimatedMeshMD2()
: FrameCount(0), FrameList(0), LastUpdatedFrame(-1)
{
	#ifdef _DEBUG
	IAnimatedMesh::setDebugName("CAnimatedMeshMD2 IAnimatedMesh");
	IMesh::setDebugName("CAnimatedMeshMD2 IMesh");
	IMeshBuffer::setDebugName("CAnimatedMeshMD2 IMeshBuffer");
	#endif	
}



//! destructor
CAnimatedMeshMD2::~CAnimatedMeshMD2()
{
	if (FrameList)
		delete [] FrameList;
}



//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
s32 CAnimatedMeshMD2::getFrameCount()
{
	return FrameCount<<3;
}



//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
IMesh* CAnimatedMeshMD2::getMesh(s32 frame, s32 detailLevel)
{
	LastFrame = frame;

	if ((u32)LastFrame > (FrameCount<<3))
		LastFrame = (LastFrame % (FrameCount<<3));

	updateInterpolationBuffer();
	return this;
}


//! returns amount of mesh buffers.
s32 getMeshBufferCount()
{
	return 1;
}


//! returns pointer to a mesh buffer
IMeshBuffer* CAnimatedMeshMD2::getMeshBuffer(s32 nr)
{
	return this;
}



//! returns the material of this meshbuffer
const video::SMaterial& CAnimatedMeshMD2::getMaterial() const
{
	return Material;
}


//! returns the material of this meshbuffer
video::SMaterial& CAnimatedMeshMD2::getMaterial()
{
	return Material;
}



//! returns pointer to vertices
const void* CAnimatedMeshMD2::getVertices() const
{
	return InterpolateBuffer.const_pointer();
}


//! returns pointer to vertices
void* CAnimatedMeshMD2::getVertices()
{
	return InterpolateBuffer.pointer();
}



//! returns which type of vertex data is stored.
video::E_VERTEX_TYPE CAnimatedMeshMD2::getVertexType() const
{
	return video::EVT_STANDARD;
}



//! returns amount of vertices
s32 CAnimatedMeshMD2::getVertexCount() const
{
	return FrameList[0].size();
}



//! returns pointer to Indices
const u16* CAnimatedMeshMD2::getIndices() const
{
	return Indices.const_pointer();
}



//! returns amount of indices
s32 CAnimatedMeshMD2::getIndexCount() const
{
	return Indices.size();
}


//! returns amount of mesh buffers.
s32 CAnimatedMeshMD2::getMeshBufferCount()
{
	return 1;
}


// updates the interpolation buffer
void CAnimatedMeshMD2::updateInterpolationBuffer()
{
	if (LastUpdatedFrame == LastFrame)
		return;

	u32 firstFrame = LastFrame>>3;
	u32 secondFrame = (LastFrame>>3)+1;

	if (secondFrame >= FrameCount)
	{
		firstFrame = FrameCount-2;
		secondFrame = FrameCount-1;
	}

	f32 div = (f32)(LastFrame - (firstFrame<<3)) / (f32)((secondFrame<<3) - (firstFrame<<3));
	video::S3DVertex* target = &InterpolateBuffer[0];
	video::S3DVertex* first = FrameList[firstFrame].pointer();
	video::S3DVertex* second = FrameList[secondFrame].pointer();

	s32 count = FrameList[firstFrame].size();

	// interpolate both frames
	for (s32 i=0; i<count; ++i)
	{
		target->Pos = (second->Pos - first->Pos) * div + first->Pos;
		target->Normal = (second->Normal - first->Normal) * div + first->Normal;

		++target;
		++first;
		++second;
	}

	LastUpdatedFrame = LastFrame;
}



//! returns max element
inline s32 CAnimatedMeshMD2::max(s32 a, s32 b)
{
	return a>b ? a : b;
}


//! loads an md3 file
bool CAnimatedMeshMD2::loadFile(io::IReadFile* file)
{
	if (!file)
		return false;

	SMD2Header header;
	s8* frames = 0;

	file->read(&header, sizeof(SMD2Header));

	if (header.magic != MD2_MAGIC_NUMBER || header.version != MD2_VERSION)
	{
		os::Debuginfo::print("MD2 Loader: Wrong file header", file->getFileName());
		return false;
	}

	// create Memory for indices and frames

	Indices.reallocate(header.numTriangles);
	FrameList = new core::array<video::S3DVertex>[header.numFrames];
	FrameCount = header.numFrames;

	for (s32 i=0; i<header.numFrames; ++i)
		FrameList[i].reallocate(header.numVertices);

	// read TextureCoords

	file->seek(header.offsetTexcoords, false);
	SMD2TextureCoordinate* textureCoords = new SMD2TextureCoordinate[header.numTexcoords];

	if (!file->read(textureCoords, sizeof(SMD2TextureCoordinate)*header.numTexcoords))
	{
		os::Debuginfo::print("MD2 Loader: Error reading TextureCoords.", file->getFileName());
		return false;
	}

	// read Triangles

	file->seek(header.offsetTriangles, false);

	SMD2Triangle *triangles = new SMD2Triangle[header.numTriangles];
	if (!file->read(triangles, header.numTriangles *sizeof(SMD2Triangle)))
	{
		os::Debuginfo::print("MD2 Loader: Error reading triangles.", file->getFileName());
		return false;
	}

	// read Vertices

	s8 buffer[MD2_MAX_VERTS*4+128];
	SMD2Frame* frame = (SMD2Frame*)buffer;

	core::array< core::vector3df >* vertices = new core::array< core::vector3df >[header.numFrames];
	core::array< core::vector3df >* normals = new core::array< core::vector3df >[header.numFrames];

	file->seek(header.offsetFrames, false);

	for (int i = 0; i<header.numFrames; ++i)
	{
		vertices[i].reallocate(header.numVertices);
		file->read(frame, header.frameSize);

		for (s32 j=0; j<header.numVertices; ++j)
		{
			core::vector3df v;
			v.X = (f32)frame->vertices[j].vertex[0] * frame->scale[0] + frame->translate[0];
			v.Z = (f32)frame->vertices[j].vertex[1] * frame->scale[1] + frame->translate[1];
			v.Y = (f32)frame->vertices[j].vertex[2] * frame->scale[2] + frame->translate[2];

			vertices[i].push_back(v);

			s32 normalidx = frame->vertices[j].lightNormalIndex;
			if (normalidx > 0 && normalidx < Q2_VERTEX_NORMAL_TABLE_SIZE)
			{
				v.X = Q2_VERTEX_NORMAL_TABLE[normalidx][0];
				v.Y = Q2_VERTEX_NORMAL_TABLE[normalidx][1];
				v.Z = Q2_VERTEX_NORMAL_TABLE[normalidx][2];
			}	

			normals[i].push_back(v);
		}
	}

	// put triangles into frame list
	
	f32 dmaxs = 1.0f/header.skinWidth;
	f32 dmaxt = 1.0f/header.skinHeight;

	video::S3DVertex vtx;
	vtx.Color = video::Color(255,255,255,255);
	
	for (s32 f = 0; f<header.numFrames; ++f)
	{
		core::array< core::vector3df >& vert = vertices[f];

		for (s32 t=0; t<header.numTriangles; ++t)
		{
			for (s32 n=0; n<3; ++n)
			{
				vtx.Pos = vert[triangles[t].vertexIndices[n]];
				vtx.TCoords.X = textureCoords[triangles[t].textureIndices[n]].s * dmaxs;
				vtx.TCoords.Y = textureCoords[triangles[t].textureIndices[n]].t * dmaxt;
				vtx.Normal = normals[f].pointer()[triangles[t].vertexIndices[n]];
				FrameList[f].push_back(vtx);
			}
		}
	}

	// create indices

	Indices.reallocate(header.numVertices);
	s32 count = header.numTriangles*3;
	for (s32 n=0; n<count; n+=3)
	{
		Indices.push_back(n);
		Indices.push_back(n+1);
		Indices.push_back(n+2);
	}

	//calculateNormals();

	// reallocate interpolate buffer
	if (header.numFrames)
	{
		InterpolateBuffer.set_used(FrameList[0].size());

		s32 count = FrameList[0].size();
		for (s32 i = 0; i<count; ++i)
		{
			InterpolateBuffer[i].TCoords = FrameList[0].pointer()[i].TCoords;
			InterpolateBuffer[i].Color = vtx.Color;
		}
	}

	// clean up

	delete [] normals;
	delete [] vertices;
	delete [] triangles;
	delete [] textureCoords;
	TriangleCount = Indices.size() / 3;

	// return

	calculateBoundingBox();

	return true;
}



//! calculates the bounding box
void CAnimatedMeshMD2::calculateBoundingBox()
{
	BoundingBox.reset(0,0,0);

	if (FrameCount)
	{
		u32 defaultFrame = 1;

		if (defaultFrame>=FrameCount)
			defaultFrame = 0;

			for (u32 j=0; j<FrameList[defaultFrame].size(); ++j)
				BoundingBox.addInternalPoint(FrameList[defaultFrame].pointer()[j].Pos);
	}
}


//! sets a flag of all contained materials to a new value
void CAnimatedMeshMD2::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
	Material.Flags[flag] = newvalue;
}



//! returns an axis aligned bounding box
const core::aabbox3d<f32>& CAnimatedMeshMD2::getBoundingBox() const
{
	return BoundingBox;
}



//! calculates normals
void CAnimatedMeshMD2::calculateNormals()
{
	for (u32 i=0; i<FrameCount; ++i)
	{
		video::S3DVertex* vtx = FrameList[i].pointer();

		for (u32 j=0; j<Indices.size(); j+=3)
		{
			core::plane3dex<f32> plane(
				vtx[Indices[j]].Pos, vtx[Indices[j+1]].Pos,	vtx[Indices[j+2]].Pos);

			vtx[Indices[j]].Normal = plane.Normal;
			vtx[Indices[j+1]].Normal = plane.Normal;
			vtx[Indices[j+2]].Normal = plane.Normal;
		}
	}
}



} // end namespace scene
} // end namespace klib