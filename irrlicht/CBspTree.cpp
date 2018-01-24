#include "S3DVertex.h"
#include <stdio.h>
#include "os.h"

#ifdef _DEBUG
#include <assert.h>
#endif

#include "CBspTree.h"
#include "line3d.h"

namespace irr
{
namespace scene
{


CBspTree::CBspTree(const scene::IMeshBuffer* array, u32 bufferCount)
{
	createStats(array, bufferCount);
	createRoot(array, bufferCount);
	createBspTree(root);
	createRenderBuffer();

	// output creation stats.

#ifdef _DEBUG
	c8 tmp[255];
	os::Debuginfo::print("/* Bsp Tree Creation stats block--------------");

	sprintf(tmp, " * LeafCounts: %d", stats.leafCounts);
	os::Debuginfo::print(tmp);
	sprintf(tmp, " * PolySplits: %d", stats.polySplits);
	os::Debuginfo::print(tmp);
	sprintf(tmp, " * NodeCount: %d", stats.nodeCount);
	os::Debuginfo::print(tmp);
	sprintf(tmp, " * Total Index count: %d", stats.totalIndexCount);
	os::Debuginfo::print(tmp);
	sprintf(tmp, " * Total Vertex count: %d", stats.totalVertexCount);
	os::Debuginfo::print(tmp);
	sprintf(tmp, " * Original total Index count: %d", stats.originalTotalIndexCount);
	os::Debuginfo::print(tmp);
	sprintf(tmp, " * Original total vertex count: %d", stats.originalTotalVertexCount);
	os::Debuginfo::print(tmp);
	os::Debuginfo::print(" *--------------------------------------------*/");
#endif
}



CBspTree::~CBspTree()
{
	deleteTree(&root);
	deleteStats();

	delete [] renderBuffer;
}




void CBspTree::createBspTree(BspNode& node)
{
	s32 vertexCount = createBoundingBox(&node.polys[0], node.polys.size(), node.bbox);

	bool found = true;

	if (vertexCount < 50) // stoppt den vorgang bei 50 vertizen.
		found = false;
	
	if (found)
		found = findSplitter(&node.polys[0], node.polys.size(), node.plane);

	if (!found)	// es wurde keine ebene gefunden, daher ist es ein leaf
	{
		node.isLeaf = true;
		node.back = 0;
		node.front = 0;

		++stats.leafCounts;

		countPolyIndizes(node);
		countPolyVertizes(node);

		stats.boxes.push_back(node.bbox);

		return;
	}

	// es ist eine node, wir machen kinder.
	node.front = new BspNode();
	node.back = new BspNode();
	stats.nodeCount += 2;

	// polys in front oder back kopieren
	sortPolys(node);

	// polygone löschen
	node.polys.clear();

	// mit kindern das gleiche machen
	createBspTree(*node.front);
	createBspTree(*node.back);
}




void CBspTree::sortPolys(const BspNode& node)
{
	for (u32 i=0; i<node.polys.size(); ++i)
	{
		core::EIntersectionRelation3D bpc = classifyPoly(node.plane, node.polys[i]);

		switch(bpc)
		{
		case core::ISREL3D_FRONT:
			node.front->polys.push_back(node.polys[i]);
			break;
		case core::ISREL3D_BACK:
			node.back->polys.push_back(node.polys[i]);
			break;
		case core::ISREL3D_SPANNING: // muss geteilt werden
			{
				BspPoly newFront, newBack;
				
				splitPoly(node.polys[i], node.plane, newFront, newBack);

				node.front->polys.push_back(newFront);
				node.back->polys.push_back(newBack);
			}
			break;
		case core::ISREL3D_PLANAR: // schauen in welche richtung es schaut
			{
				if (node.polys[i].plane.Normal.dotProduct(node.plane.Normal) >= 0.0f)
					node.front->polys.push_back(node.polys[i]);
				else
					node.back->polys.push_back(node.polys[i]);
			}
			break;
		}
	}

}


// liefert die anzahl der gesamten vertices im node zurück
s32 CBspTree::createBoundingBox(const BspPoly* poly, u32 polyCount, core::aabbox3d<f32>& outBox)
{
	s32 totalCount = 0;
	outBox.reset(poly->vertices[0].Pos);

	for (u32 i = 0; i<polyCount; ++i)
	{
		totalCount += poly[i].vertices.size();

		for (u32 j = 0; j<poly[i].vertices.size(); ++j)
			outBox.addInternalPoint(poly[i].vertices[j].Pos);
	}

	return totalCount;
}




bool CBspTree::findSplitter(BspPoly* poly, u32 polyCount, core::plane3d<f32>& outSplitter)
{
	BspPoly* bestSplitter = 0;
	s32 bestScore = 0xfffffff;

	for (u32 i=0; i<polyCount; ++i)
	{
		u32 frontCount = 0;
		u32 backCount = 0;
		u32 planarCount = 0;
		u32 splitCount = 0;

		if (!poly[i].wasSplitter)
		{
			for (u32 j=0; j<polyCount; ++j)
			{
				if (i!=j)
				{
					core::EIntersectionRelation3D bpc = classifyPoly(poly[i].plane, poly[j]);

					switch(bpc)
					{
					case core::ISREL3D_FRONT: ++frontCount; break;
					case core::ISREL3D_BACK: ++backCount; break;
					case core::ISREL3D_PLANAR: ++planarCount; break;
					case core::ISREL3D_SPANNING: ++splitCount; break;
					}
				}
			}

			s32 score = abs(int(frontCount - backCount)) + (splitCount*3) + planarCount;
			//u32 score = abs(frontCount - backCount) + (splitCount*3) + planarCount;

			if (score < bestScore)
			{
				if ( ((frontCount > 0) && (backCount > 0)) || (splitCount > 0)) // Ist die Polyliste nicht konvex?
				{
					bestScore = score;
					bestSplitter = &poly[i];
				} 
			}
		}
	}

	if (!bestSplitter)
		return false;

	bestSplitter->wasSplitter = true;
	outSplitter = bestSplitter->plane;

	return true;
}




void CBspTree::render(const SViewFrustrum* camArea, const core::vector3df& camPos, bool aabbRendering)
{
	for (u32 i=0; i<stats.materialCount; ++i)
	{
		renderBuffer[i].vertices.clear();
		renderBuffer[i].indices.clear();
	}

	stats.renderCullings = 0;
	stats.renderedIndices = 0;

	if (!aabbRendering)
		render(root, camArea, camPos);
	else
		renderBoxCulling(root, camArea, camPos);
}




void CBspTree::render(BspNode& node, const SViewFrustrum* camArea, const core::vector3df& camPos)
{
	++stats.renderCullings;

	if (cullBoxAgainsView(node.bbox, camArea) == BCR_COMPLETE_OUTSIDE)
      return;

	if (node.isLeaf)
	{
		renderLeaf(node);
		return;
	}

	if (node.plane.classifyPointRelation(camPos) == core::ISREL3D_BACK)
	{
		render(*node.front, camArea, camPos);
		render(*node.back, camArea, camPos);
	}
	else
	{
		render(*node.back, camArea, camPos);
		render(*node.front, camArea, camPos);
	}
}




void CBspTree::renderBoxCulling(BspNode& node, const SViewFrustrum* camArea, const core::vector3df& camPos)
{
	++stats.renderCullings;

	if (!node.bbox.intersectsWithBox(camArea->box))
     return;

	if (node.isLeaf)
	{
		renderLeaf(node);
		return;
	}

	renderBoxCulling(*node.front, camArea, camPos);
	renderBoxCulling(*node.back, camArea, camPos);
}




void CBspTree::renderLeaf(BspNode& node)
{
	for (u32 i=0; i<node.polys.size(); ++i)
	{
		s32 material = node.polys[i].material;
		RenderBuffer* buffer = &renderBuffer[material];

		s32 start = buffer->vertices.size();

		// add vertices

		s32 vtxCount = node.polys[i].vertices.size();

		for (s32 v=0; v<vtxCount; ++v)
			buffer->vertices.push_back(node.polys[i].vertices[v]);

		// add indices

		s32 idxCount = node.polys[i].indices.size();
		
		for (s32 j=0; j<idxCount; ++j)
			buffer->indices.push_back(node.polys[i].indices[j] + start);

		// update stats

		stats.renderedIndices += idxCount;
	}
}




CBspTree::BoxCullingResult CBspTree::cullBoxAgainsView(const core::aabbox3d<f32>& box, const scene::SViewFrustrum* camArea)
{
	bool clipped = false;

	for (s32 i=0; i<6; ++i)
		switch(box.classifyPlaneRelation(camArea->planes[i]))
		{
		case core::ISREL3D_FRONT:
			return BCR_COMPLETE_OUTSIDE;
		case core::ISREL3D_CLIPPED:
			clipped = true;
			break;
		}

	if (clipped)
		return BCP_CLIPPED;

	return BCR_COMPLETE_INSIDE;
}




core::EIntersectionRelation3D CBspTree::classifyPoly(const core::plane3d<f32>& plane, const BspPoly& poly)
{
	u32 countFront = 0;
	u32 countBack = 0;
	u32 countPlanar = 0;

	for (u32 i=0; i < poly.vertices.size(); ++i)
	{
		core::EIntersectionRelation3D ir3d = plane.classifyPointRelation(poly.vertices[i].Pos);

		switch(ir3d)
		{
		case core::ISREL3D_FRONT: ++countFront; break;
		case core::ISREL3D_BACK: ++countBack; break;
		case core::ISREL3D_PLANAR: ++countPlanar; ++countFront; ++countBack; break;
		}
	}

	if (countPlanar == poly.vertices.size()) // Alle Verts des Polys sind planar
		return core::ISREL3D_PLANAR; 

	if (countFront == poly.vertices.size()) // Alle Verts des Polys liegen vor der Ebene
		return core::ISREL3D_FRONT; 

	if (countBack == poly.vertices.size()) // Alle Verts des Polys liegen hinter der Ebene
		return core::ISREL3D_BACK; 

	return core::ISREL3D_SPANNING;
}




core::plane3d<f32> CBspTree::planeFromPoly(const BspPoly& poly)
{
	return core::plane3d<f32>(	poly.vertices[poly.indices[0]].Pos,
									poly.vertices[poly.indices[1]].Pos,
									poly.vertices[poly.indices[2]].Pos);
}




void CBspTree::deleteTree(BspNode *pNode)
{
	if (pNode->front)
	{
		deleteTree(pNode->front);
		delete pNode->front;
	}

	if (pNode->back)
	{
		deleteTree(pNode->back);
		delete pNode->back;
	}
}
 



void CBspTree::createRoot(const scene::IMeshBuffer* array, u32 arraySize)
{
	// Behandelt jeweils drei indizes als ein polygon.
	// TODO: Die Routine sollte zusammenliegende Dreiecke in einer Ebene
	// als ein einzelnes polygon ansehen.

	root.isLeaf = false;

	//for (u32 i=0; i<arraySize; ++i)
	//	root.countPolys += array[i].getIndexCount() / 3;
	
	//root.polys.set_used(root.countPolys);

	root.front = 0;
	root.back = 0;

	for (u32 i=0; i<arraySize; ++i)
	{
		const video::S3DVertex* vertices = (video::S3DVertex*)array[i].getVertices();

		stats.materialMap[i] = array[i].getMaterial();

		const u16* indices = array[i].getIndices();

		for (s32 index = 0; index < array[i].getIndexCount(); index+=3)
		{
			BspPoly poly;
			poly.vertices.push_back( vertices[indices[index]] );
			poly.vertices.push_back( vertices[indices[index+1]] );
			poly.vertices.push_back( vertices[indices[index+2]] );

			poly.indices.push_back( 0 );
			poly.indices.push_back( 1 );
			poly.indices.push_back( 2 );
			
			poly.wasSplitter = false;
			poly.plane = planeFromPoly(poly);

			poly.material = i;

			root.polys.push_back(poly);
		}
	}
}




bool CBspTree::polyExistsInNode(BspPoly* poly, BspNode* node, u32 polysToCheck)
{
	for (u32 i=0; i<polysToCheck; ++i)
	{
		if (node->polys[i] == *poly)
			return true;
	}
	return false;	
}




void CBspTree::createStats(const scene::IMeshBuffer* array, u32 materialCount)
{
	stats.leafCounts = 0;
	stats.nodeCount = 0;
	stats.polySplits = 0;
	stats.totalIndexCount = 0;
	stats.totalVertexCount = 0;
	stats.originalTotalVertexCount = 0;
	stats.originalTotalIndexCount = 0;

	u32 i=0;

	for (i=0; i<materialCount; ++i)
	{
		stats.originalTotalVertexCount += array[i].getVertexCount();
		stats.originalTotalIndexCount += array[i].getIndexCount();
	}

	stats.materialCount = materialCount;

	stats.indexCount = new u32[materialCount];
	for (i=0; i<materialCount; ++i)
		stats.indexCount[i] = 0;
	
	stats.vertexCount = new u32[materialCount];
	for (i=0; i<materialCount; ++i)
		stats.vertexCount[i] = 0;

	stats.materialMap = new video::SMaterial[materialCount];
}




void CBspTree::deleteStats()
{
	delete [] stats.indexCount;
	delete [] stats.vertexCount;
	delete [] stats.materialMap;
}

const SBspTreeStats* CBspTree::getStats()
{
	return &stats;
}

void CBspTree::createRenderBuffer()
{
	renderBuffer = new RenderBuffer[stats.materialCount];

	for (u32 i=0; i<stats.materialCount; ++i)
	{
		//renderBuffer[i].vertices.set_used( = new u16[stats.indexCount[i]];
		//memset(renderBuffer[i].iBuffer , 0, sizeof(u16)*stats.indexCount[i]); // TODO wegtun
		//renderBuffer[i].vBuffer = new SStdVertex[stats.vertexCount[i]];
		//memset(renderBuffer[i].vBuffer , 0, sizeof(SStdVertex)*stats.vertexCount[i]); // TODO wegtun
		//renderBuffer[i].usedIndices = 0; // TODO: Wegtun
		//renderBuffer[i].usedVertices = 0;// TODO: wegtun
	}
}


void CBspTree::countPolyVertizes(const BspNode& node)
{
	for (u32 i=0; i<node.polys.size(); ++i)
	{
		stats.totalVertexCount += node.polys[i].vertices.size();
		stats.vertexCount[node.polys[i].material] += node.polys[i].vertices.size();
	}
}

void CBspTree::countPolyIndizes(const BspNode& node)
{
	for (u32 i=0; i<node.polys.size(); ++i)
	{
		stats.totalIndexCount += node.polys[i].vertices.size();
		stats.indexCount[node.polys[i].material] += node.polys[i].vertices.size();
	}
}

#ifdef _DEBUG
void CBspTree::debugOutputLeaf(const BspNode& node)
{
/*	c8 tmp[1024];
	sprintf(tmp, "(box: (%d, %d, %d), (%d, %d, %d)) countPolys:%d \n", (s32)node.bbox.minEdge.x, (s32)node.bbox.minEdge.y, (s32)node.bbox.minEdge.z, (s32)node.bbox.maxEdge.x, (s32)node.bbox.maxEdge.y, (s32)node.bbox.maxEdge.z, node.countPolys);
	logString(tmp);

	for (u32 i=0; i<node.countPolys; ++i)
	{
		logString("       polygon: ");

		for (u32 v=0; v<node.polys[i].countIndices; ++v)
		{
			SStdVertex* vtx = &node.polys[i].vertices[node.polys[i].indices[v]];
			sprintf(tmp, "(%d %d %d)", (s32)vtx->x, (s32)vtx->y, (s32)vtx->z);
			logString(tmp);
		}
		sprintf(tmp, "  (plane nVect: %d %d %d)\n", (s32)node.polys[i].plane.nVect.x, (s32)node.polys[i].plane.nVect.y, (s32)node.polys[i].plane.nVect.z);
		logString(tmp);
	}
*/
}

void CBspTree::debugValidateLeaf(const BspNode& node)
{
	/*
	if (node.isLeaf)
	{
		logString("begin NodeValidate\n");
		if (node.back)	logString("[CBspTree] Error validating leaf: back is nonzero.\n");
		if (node.front)	logString("[CBspTree] Error validating leaf: front is nonzero.\n");
		if (node.countPolys == 0) logString("[CBspTree] Error validating leaf: countPolys is zero.\n");

		for (u32 i=0; i<node.countPolys; ++i)
		{
			if (!node.polys[i].countVertices) logString("[CBspTree] Error validating leaf: no vertices in poly.\n");
			if (!node.polys[i].countIndices) logString("[CBspTree] Error validating leaf: no indices in poly.\n");

			if (node.polys[i].countIndices >= BSP_MAX_POLY_INDICES)
				logString("[CBspTree] Error validating leaf: too much indices in poly.\n");

			if (node.polys[i].countVertices >= BSP_MAX_POLY_VERTICES)
				logString("[CBspTree] Error validating leaf: too much vertices in poly.\n");
			
			for (u32 j=0; j<node.polys[i].countIndices; ++j)
				if (node.polys[i].indices[j] >= node.polys[i].countVertices)
					logString("[CBspTree] Error validating leaf: index pointing to invalid vertex.\n");

			if (node.polys[i].countIndices % 3 != 0)
				logString("[CBspTree] Error validating leaf: indexCount is not power of 3.\n");

			c8 tmp[255];
			sprintf(tmp, "Created poly%d got %d indices and %d vertices.\n", i, node.polys[i].countIndices, node.polys[i].countVertices);
			logString(tmp);
		}

		logString("end NodeValidate\n");
	}
	else
		logString("[CBspTree] Error validating leaf: not a leaf.\n");

	*/
}
#endif



void CBspTree::splitPoly(const BspPoly& poly, const core::plane3d<f32>& plane,
						 BspPoly& frontSplit, BspPoly& backSplit)
{
	#ifdef _DEBUG
	assert(frontSplit.indices.size() == 0 && frontSplit.vertices.size() == 0);
	#endif

	++stats.polySplits;

	const s32 LIST_BUFFER_SIZE = 40;

	video::S3DVertex frontList[LIST_BUFFER_SIZE];
	video::S3DVertex backList[LIST_BUFFER_SIZE];
	u32 countFront = 0;
	u32 countBack = 0;

	//memset(&frontList, 0, sizeof(SStdVertex)*LIST_BUFFER_SIZE); // todo: weg damit
	//memset(&backList, 0, sizeof(SStdVertex)*LIST_BUFFER_SIZE); // todo: weg damit
	//memset(&frontSplit, 0, sizeof(BspPoly)); // todo: weg damit
	//memset(&backSplit, 0, sizeof(BspPoly)); // todo: weg damit

	// Die Beiden Schnittpunkte müssen gesucht werden.
	// Dazu gehe ich im uhrzeigersinn die Punkte einzeln ab
	// und ordne in front und BackList ein. Wenn dann auch noch
	// ein Schnittpunkt zwischen diesem eingeornetem Punkt und 
	// dem nächstem Punkt existiert, dann wird der auch noch
	// eingeordnet.

	u32 currentIdx = poly.vertices.size() - 1;

	//assert(currentIdx < BSP_MAX_POLY_VERTICES);

	switch (plane.classifyPointRelation(poly.vertices[currentIdx].Pos))
	{
	case core::ISREL3D_FRONT:
		frontList[countFront] = poly.vertices[currentIdx];
		countFront++;
		break;
	case core::ISREL3D_BACK:
		backList[countBack] = poly.vertices[currentIdx];
		countBack++;
		break;
	case core::ISREL3D_PLANAR:
		backList[countBack]   = poly.vertices[currentIdx];
		frontList[countFront] = poly.vertices[currentIdx];
		countBack++;
		countFront++;
		break;
	}

	for (u32 currentIdx2=0; currentIdx2<poly.vertices.size(); ++currentIdx2)
	{
		const core::vector3df &pointA = poly.vertices[currentIdx].Pos;
		const core::vector3df &pointB = poly.vertices[currentIdx2].Pos;
		core::vector3df intersection;

		core::EIntersectionRelation3D i3d = plane.classifyPointRelation(pointB);

		if (i3d != core::ISREL3D_PLANAR && 
			plane.getIntersectionWithLimitedLine(pointA, pointB, intersection))
		{
			// TODO: Kann beschleunigt werden, indem statt getIntersectionWithLimitedLine eine Funktion
			// verwendet wird, die nicht überprüft, ob sich der schnittpunkt auch zwischen den punkten
			// befindet, das macht schon die percent-Abfrage.

			core::line3d<f32> l(pointB, pointA);
			core::line3d<f32> l2(pointB, intersection);
			f32 percent = (f32)(l2.getLenght() / l.getLenght());

			if (!(percent < 0.0f || percent > 1.0f))
			{
				// Hier gibt's eine intersection, den schnittpunkt als vertex in beide listen einfügen
				u32 red = (u32)(
					poly.vertices[currentIdx2].Color.getRed() +
					(poly.vertices[currentIdx].Color.getRed() - 
					poly.vertices[currentIdx2].Color.getRed()) * percent );

				u32 green = (u32)(
					poly.vertices[currentIdx2].Color.getGreen() +
					(poly.vertices[currentIdx].Color.getGreen() - 
					poly.vertices[currentIdx2].Color.getGreen()) * percent );

				u32 blue = (u32)(
					poly.vertices[currentIdx2].Color.getBlue() +
					(poly.vertices[currentIdx].Color.getBlue() - 
					poly.vertices[currentIdx2].Color.getBlue()) * percent );

				core::vector2d<f32> tcoords =
					(poly.vertices[currentIdx].TCoords - poly.vertices[currentIdx2].TCoords) * percent;

				//assert(poly.vertices[currentIdx].nx == poly.vertices[currentIdx2].nx);
				//assert(poly.vertices[currentIdx].ny == poly.vertices[currentIdx2].ny);
				//assert(poly.vertices[currentIdx].nz == poly.vertices[currentIdx2].nz);

				video::S3DVertex newVertex(intersection,
											poly.vertices[currentIdx].Normal,
											video::Color(0, red, green, blue),
											tcoords);

				frontList[countFront] = newVertex;
				countFront++;
				backList[countBack] = newVertex;
				countBack++;
			}
		}

		#ifdef _DEBUG
		assert(countBack <= LIST_BUFFER_SIZE && countFront <= LIST_BUFFER_SIZE); // listbuffer ist zu klein
		#endif

		if (currentIdx2 != poly.vertices.size() - 1)
		{
			switch (i3d)
			{
			case core::ISREL3D_FRONT:
				frontList[countFront] = poly.vertices[currentIdx2];
				countFront++;
				break;
			case core::ISREL3D_BACK:
				backList[countBack] = poly.vertices[currentIdx2];
				countBack++;
				break;
			case core::ISREL3D_PLANAR:
				backList[countBack]   = poly.vertices[currentIdx2];
				frontList[countFront] = poly.vertices[currentIdx2];
				countBack++;
				countFront++;
				break;
			}
		}

		// index höherzählen
		
		++currentIdx;
		if (currentIdx == poly.vertices.size())
			currentIdx = 0;
	}

	// Die richtigen Vertizen wurden jetzt in die buffer geschrieben, jetzt müssen
	// daraus polygone gemacht werden.

	//frontSplit.countVertices = countFront;
	//backSplit.countVertices = countBack;
	frontSplit.wasSplitter = poly.wasSplitter;
	backSplit.wasSplitter = poly.wasSplitter;
	frontSplit.material = poly.material;
	backSplit.material = poly.material;
	
	//assert(countFront <= BSP_MAX_POLY_VERTICES && countBack <= BSP_MAX_POLY_VERTICES);

	for (u32 i=0; i<countFront; ++i)
		frontSplit.vertices.push_back(frontList[i]);
	
	for (int i=0; i<countBack; ++i)
		backSplit.vertices.push_back(backList[i]);

	s32 frontIndexCount = (frontSplit.vertices.size() - 2) * 3;
	s32 backIndexCount = (backSplit.vertices.size() - 2) * 3;

	#ifdef _DEBUG
	assert((frontIndexCount >= 3 || frontIndexCount == 0) &&
			(backIndexCount >= 3 || backIndexCount == 0));
	assert (frontSplit.vertices.size() && backSplit.vertices.size());
	#endif

	s32 v0=0, v1=1, v2=2;

	for (int i=0; i<(u32)frontIndexCount/3; ++i)
	{
		if (i) {	v1=v2; v2++; }

		frontSplit.indices.push_back(v0);
		frontSplit.indices.push_back(v1);
		frontSplit.indices.push_back(v2);
	}

	frontSplit.plane = planeFromPoly(frontSplit);

	#ifdef _DEBUG 	// falsche indizes suchen
	for (int i=0; i<(u32)frontIndexCount; ++i)
		if (frontSplit.indices[i] >= frontSplit.vertices.size())
		{
			os::Debuginfo::print("[CBspTree] Error! Invalid index created during split.\n");
			_asm int 3
		}
	#endif

	v0=0; v1=1; v2=2;

	for (int i=0; i<(u32)backIndexCount/3; ++i)
	{
		if (i)
		{
			v1=v2;
			v2++;
		}

		backSplit.indices.push_back(v0);
		backSplit.indices.push_back(v1);
		backSplit.indices.push_back(v2);
	}

	backSplit.plane = planeFromPoly(backSplit);

	#ifdef _DEBUG // falsche indizes suchen
	for (int i=0; i<(u32)backIndexCount; ++i)
		if (backSplit.indices[i] >= backSplit.vertices.size())
		{
			os::Debuginfo::print("[CBspTree] Error! Invalid index created during split.\n");
			_asm int 3
		}
	#endif
}


const CBspTree::RenderBuffer* CBspTree::getRenderBuffer(u32 materialNr) const
{
	return &renderBuffer[materialNr];
}


} // end namespace
} // end namespace 
