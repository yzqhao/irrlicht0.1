#include "CQ3LevelMesh.h"
#include "ISceneManager.h"
#include "os.h"
#include "SMeshBufferLightMap.h"
#include "irrstring.h"
#include "CColorConverter.h"
#include <stdio.h>

namespace irr
{
namespace scene
{


//! constructor
CQ3LevelMesh::CQ3LevelMesh(io::IFileSystem* fs, video::IVideoDriver* driver)
: Textures(0), LightMaps(0),
 Vertices(0), Faces(0),	Planes(0), Nodes(0), Leafs(0), LeafFaces(0),
	MeshVerts(0), Brushes(0), Driver(driver), FileSystem(fs)
{
	#ifdef _DEBUG
	IUnknown::setDebugName("CQ3LevelMesh");
	#endif	

	if (Driver)
		Driver->grab();

	if (FileSystem)
		FileSystem->grab();
}


//! destructor
CQ3LevelMesh::~CQ3LevelMesh()
{
	if (Textures)
		delete [] Textures;

	if (LightMaps)
		delete [] LightMaps;
	
	if (Vertices)
		delete [] Vertices;

	if (Faces)
		delete [] Faces;

	if (Planes)
		delete [] Planes;

	if (Nodes)
		delete [] Nodes;

	if (Leafs)
		delete [] Leafs;

	if (LeafFaces)
		delete [] LeafFaces;
	
	if (MeshVerts)
		delete [] MeshVerts;

	if (Brushes)
		delete [] Brushes;

	if (Driver)
		Driver->drop();

	if (FileSystem)
		FileSystem->drop();
}



//! loads a level from a .bsp-File. Also tries to load all needed textures. Returns true if successful.
bool CQ3LevelMesh::loadFile(io::IReadFile* file)
{
	if (!file)
		return false;

	LevelName = file->getFileName();

	tBSPHeader header;
	file->read(&header, sizeof(tBSPHeader));

	if (header.strID != 'PSBI' || header.version != 0x2e)
	{
		os::Warning::print("Could not load .bsp file, unknown header.", file->getFileName());
		return false;
	}

	// now read lumps

	file->read(&Lumps[0], sizeof(tBSPLump)*kMaxLumps);

	// load everything

	loadTextures(&Lumps[kTextures], file);		// Load the textures
	loadLightmaps(&Lumps[kLightmaps], file);      // Load the lightmaps
	loadVerts(&Lumps[kVertices], file);		// Load the vertices
	loadFaces(&Lumps[kFaces], file);		// Load the faces
	loadPlanes(&Lumps[kPlanes], file);		// Load the Planes of the BSP
	loadNodes(&Lumps[kNodes], file);		// load the Nodes of the BSP
	loadLeafs(&Lumps[kLeafs], file);		// load the Leafs of the BSP
	loadLeafFaces(&Lumps[kLeafFaces], file);		// load the Faces of the Leafs of the BSP
	loadVisData(&Lumps[kVisData], file);		// load the visibility data of the clusters
	loadEntities(&Lumps[kEntities], file);		// load the entities
	loadModels(&Lumps[kModels], file);		// load the models
	loadMeshVerts(&Lumps[kMeshVerts], file);		// load the mesh vertices
	loadBrushes(&Lumps[kBrushes], file);		// load the brushes of the BSP
	loadBrushSides(&Lumps[kBrushSides], file);		// load the brushsides of the BSP
	loadLeafBrushes(&Lumps[kLeafBrushes], file);		// load the brushes of the leaf*/

	constructMesh();

	loadTextures();

	return true;
}



//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
s32 CQ3LevelMesh::getFrameCount()
{
	return 1;
}



//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
IMesh* CQ3LevelMesh::getMesh(s32 frameInMs, s32 detailLevel)
{
	return &Mesh;
}


void CQ3LevelMesh::loadTextures(tBSPLump* l, io::IReadFile* file)
{
	NumTextures = l->length / sizeof(tBSPTexture);
	Textures = new tBSPTexture[NumTextures];

	file->seek(l->offset);
	file->read(Textures, l->length);
}


void CQ3LevelMesh::loadLightmaps(tBSPLump* l, io::IReadFile* file)
{
	NumLightMaps = l->length / sizeof(tBSPLightmap);
	LightMaps = new tBSPLightmap[NumLightMaps];

	file->seek(l->offset);
	file->read(LightMaps, l->length);
}


void CQ3LevelMesh::loadVerts(tBSPLump* l, io::IReadFile* file)
{
	NumVertices = l->length / sizeof(tBSPVertex);
	Vertices = new tBSPVertex[NumVertices];

	file->seek(l->offset);
	file->read(Vertices, l->length);
}


void CQ3LevelMesh::loadFaces(tBSPLump* l, io::IReadFile* file)
{
	NumFaces = l->length / sizeof(tBSPFace);
	Faces = new tBSPFace[NumFaces];

	file->seek(l->offset);
	file->read(Faces, l->length);
}


void CQ3LevelMesh::loadPlanes(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadNodes(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadLeafs(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadLeafFaces(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadVisData(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadEntities(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadModels(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadMeshVerts(tBSPLump* l, io::IReadFile* file)
{
	NumMeshVerts = l->length / sizeof(s32);
	MeshVerts = new s32[NumMeshVerts];

	file->seek(l->offset);
	file->read(MeshVerts, l->length);
}


void CQ3LevelMesh::loadBrushes(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadBrushSides(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


void CQ3LevelMesh::loadLeafBrushes(tBSPLump* l, io::IReadFile* file)
{
	// ignore
}


//! constructs a mesh from the quake 3 level file.
void CQ3LevelMesh::constructMesh()
{
	// reserve buffer. 

	for (s32 i=0; i<(NumTextures+1) * (NumLightMaps+1); ++i)
	{
		scene::SMeshBufferLightMap* buffer = new scene::SMeshBufferLightMap();

		buffer->Material.MaterialType = video::EMT_LIGHTMAP;
		buffer->Material.Wireframe = false;
		buffer->Material.Lighting = false;
		buffer->Material.BilinearFilter = true;

		Mesh.addMeshBuffer(buffer);

		buffer->drop();
	}

	// go through all faces and add them to the buffer.

	for (int i=0; i<NumFaces; ++i)
	{
		if (Faces[i].lightmapID < -1)
			Faces[i].lightmapID = -1;

		if (Faces[i].lightmapID > NumLightMaps-1)
			Faces[i].lightmapID = -1;

		// there are lightmapsids and textureid with -1
		s32 meshBufferIndex = ((Faces[i].lightmapID+1) * (NumTextures+1)) + (Faces[i].textureID+1);
		SMeshBufferLightMap* meshBuffer = ((SMeshBufferLightMap*)Mesh.getMeshBuffer(meshBufferIndex));

		switch(Faces[i].type)
		{
			//case 3: // mesh vertices
			case 1: // normal polygons
				{
					for (s32 tf=2; tf<Faces[i].numOfVerts; ++tf)
					{
						s32 idx = meshBuffer->getVertexCount();
						s32 vidxes[3];

						vidxes[0] = Faces[i].vertexIndex;
						vidxes[1] = Faces[i].vertexIndex+tf-1;
						vidxes[2] = Faces[i].vertexIndex+tf;

						// add all tree vertices

						for (s32 vu=0; vu<3; ++vu)
						{
							video::S3DVertex2TCoords currentVertex;
							tBSPVertex *v = &Vertices[vidxes[vu]];

							currentVertex.Color = video::Color(v->color[3], v->color[0], v->color[1], v->color[2]);
							//currentVertex.Color = video::Color(v->color[3], v->color[2], v->color[1], v->color[0]);
							currentVertex.Pos.X = v->vPosition[0];
							currentVertex.Pos.Y = v->vPosition[2];
							currentVertex.Pos.Z =  v->vPosition[1];
							currentVertex.Normal.X = v->vNormal[0];
							currentVertex.Normal.Y = v->vNormal[1];
							currentVertex.Normal.Z = v->vNormal[2];
							currentVertex.TCoords.X = v->vTextureCoord[0];
							currentVertex.TCoords.Y = v->vTextureCoord[1];
							currentVertex.TCoords2.X = v->vLightmapCoord[0];
							currentVertex.TCoords2.Y = v->vLightmapCoord[1];

							meshBuffer->Vertices.push_back(currentVertex);
						}

						// add indexes

						meshBuffer->Indices.push_back(idx);
						meshBuffer->Indices.push_back(idx+1);
						meshBuffer->Indices.push_back(idx+2);
					}
				}
				break;
			case 2: // curved surfaces
				// TODO: we'll ignore curved surfaces currently.
				break;
			case 4: // billboards
				break;
			default:
				break;
		} // end switch
	}
	
}


//! loads the textures
void CQ3LevelMesh::loadTextures()
{
	if (!Driver)
		return;

	core::stringc s;
	core::stringc extensions[2];
	extensions[0] = ".jpg";
	extensions[1] = ".tga";

	// load textures

	core::array<video::ITexture*> tex;
	tex.set_used(NumTextures+1);

	tex[0] = 0;

	for (s32 t=1; t<(NumTextures+1); ++t)
	{
		tex[t] = 0;

		for (s32 e=0; e<2; ++e)
		{
			s = Textures[t-1].strName;
			s.append(extensions[e]);
			if (FileSystem->existFile(s.c_str()))
			{
				tex[t] = Driver->getTexture(s.c_str(), true);
				break;
			}
		}

		//if (!tex[t])
		//	os::Warning::print("Could not find a texture for entry in bsp file", Textures[t-1].strName);
	}

	// load lightmaps.

	core::array<video::ITexture*> lig;
	lig.set_used(NumLightMaps+1);

	lig[0] = 0;

	c8 lightmapname[255];
	core::dimension2d<s32> lmapsize(128,128);

	for (s32 t = 1; t<(NumLightMaps + 1); ++t)
	{
		sprintf(lightmapname, "%s.lightmap.%d", LevelName.c_str(), t);
		lig[t] = Driver->addTexture(lmapsize, lightmapname);
		if (lig[t])
		{
			s16* p = (s16*)lig[t]->lock();
			if (p)
			{
				if (lig[t]->getColorFormat() == video::EHCF_R5G5B5)
				{
					tBSPLightmap* lm;
					lm = &LightMaps[t-1];

					for (s32 x=0; x<128; ++x)
						for (s32 y=0; y<128; ++y)
						{
							// Checker lightmaps
							//s32 r = ((x+y)%2)*255;
							//p[x*128 +y] = video::RGB16(r,r,r);

							// Directly lightmaps from bps-file
							p[x*128 +y] = video::RGB16(
								lm->imageBits[x][y][0],
								lm->imageBits[x][y][1],
								lm->imageBits[x][y][2]);

							// Modulate lightmaps by 2
							/*s32 r = lm->imageBits[x][y][0]*2;
							s32 g = lm->imageBits[x][y][1]*2;
							s32 b = lm->imageBits[x][y][2]*2;

							if (r>255) r = 255;
							if (g>255) g = 255;
							if (b>255) b = 255;

							p[x*128 +y] = video::RGB16(r,g,b);*/
						}
				}
				else
					os::Warning::print("Could not create lightmap, unsupported texture format.");
			}

			lig[t]->unlock();
		}
		else
			os::Warning::print("Could not create lightmap, driver created no texture.");
	}

	// attach textures to materials.

	for (s32 l=0; l<NumLightMaps+1; ++l)
		for (s32 t=0; t<NumTextures+1; ++t)
		{
			SMeshBufferLightMap* b = (SMeshBufferLightMap*)Mesh.getMeshBuffer(l*(NumTextures+1) + t);
			b->Material.Texture2 = lig[l];
			b->Material.Texture1 = tex[t];

			if (!b->Material.Texture2)
				b->Material.MaterialType = video::EMT_SOLID;
		}

	// delete all buffers without geometry in it.

	s32 i = 0;
	while(i < (s32)Mesh.MeshBuffers.size())
	{
		if (Mesh.MeshBuffers[i]->getVertexCount() == 0 ||
			Mesh.MeshBuffers[i]->getIndexCount() == 0)
		{
			// Meshbuffer löschen
			Mesh.MeshBuffers[i]->drop();
			Mesh.MeshBuffers.erase(i);		
		}
		else
			++i;
	}
}

} // end namespace scene
} // end namespace irr
