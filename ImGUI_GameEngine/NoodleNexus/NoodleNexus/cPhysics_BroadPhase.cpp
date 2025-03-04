#include "cPhysics.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 

cPhysics::cBroad_Cube::cBroad_Cube(
	glm::vec3 minXYZ, glm::vec3 maxXYZ,
	float sizeOrWidth,
	unsigned long long indexXYZ)
{
	this->m_minXYZ = minXYZ;
	this->m_maxXYZ = maxXYZ;
	this->m_size = sizeOrWidth;
	this->m_indexXYZ = indexXYZ;
}

unsigned long long cPhysics::cBroad_Cube::getGridIndexID(void)
{
	return this->m_indexXYZ;
}

float cPhysics::cBroad_Cube::getSize(void)
{
	return this->m_size;
}

glm::vec3 cPhysics::cBroad_Cube::getMinXYZ(void)
{
	return this->m_minXYZ;
}

glm::vec3 cPhysics::cBroad_Cube::getMaxXYZ(void)
{
	return this->m_maxXYZ;
}

glm::vec3 cPhysics::calcBP_MinXYZ_FromID(unsigned long long BP_CubeID, float sizeOrWidth)
{
	const unsigned long long MULTIPLIER = 1'000'000;

	long long xIndex = BP_CubeID / (MULTIPLIER * MULTIPLIER);

	long long yIndex = BP_CubeID - (xIndex * MULTIPLIER * MULTIPLIER);	// Strip away the x digits
	yIndex = yIndex / MULTIPLIER;

	long long zIndex = BP_CubeID - (xIndex * MULTIPLIER * MULTIPLIER) - (yIndex * MULTIPLIER);
		
	glm::vec3 minXYZ = glm::vec3(0.0f);

	// The 1st digit indicates +ve or -ve, 100,000 --> -ve 
	if (xIndex > 99'999)
	{
		// It's negative, so strip away the 1st digit
		minXYZ.x = -(((xIndex + 1) - 100'000) * sizeOrWidth);
	}
	else
	{
		minXYZ.x = xIndex * sizeOrWidth;
	}

	if (yIndex > 99'999)
	{
		// It's negative
		minXYZ.y = -(((yIndex + 1) - 100'000) * sizeOrWidth);
	}
	else
	{
		minXYZ.y = yIndex * sizeOrWidth;
	}

	if (zIndex > 99'999)
	{
		// It's negative
		minXYZ.z = -(((zIndex + 1) - 100'000) * sizeOrWidth);
	}
	else
	{
		minXYZ.z = zIndex * sizeOrWidth;
	}

	return minXYZ;
}


unsigned long long cPhysics::calcBP_GridIndex(
	float x, float y, float z, float sizeOrWidth)
{
	// Calculate the "rounded down" index based on the AABB size:
	int xIndexMin = (int)(x / sizeOrWidth);
	int yIndexMin = (int)(y / sizeOrWidth);
	int zIndexMin = (int)(z / sizeOrWidth);

	// "shift" this value by 6 digits
	// 
	// unsigned long long: 
	// 
	// xxx,xxx   yyy,yyy   zzz,zzz
	// 
	// Where 1st digit is sign
	//
	const unsigned long long NEGATIVE_DIGIT = 100'000;
	if (x < 0)
	{
		xIndexMin = abs(xIndexMin) + NEGATIVE_DIGIT;
	}
	if (y < 0)
	{
		yIndexMin = abs(yIndexMin) + NEGATIVE_DIGIT;
	}
	if (z < 0)
	{
		zIndexMin = abs(zIndexMin) + NEGATIVE_DIGIT;
	}

	const unsigned long long MULTIPLIER = 1'000'000;
	unsigned long long gridIndexID =
		xIndexMin * MULTIPLIER * MULTIPLIER
		+ yIndexMin * MULTIPLIER
		+ zIndexMin;

	return gridIndexID;
}


bool cPhysics::generateBroadPhaseGrid(std::string meshModelName, float AABBCubeSize_or_Width,
	glm::vec3 meshWorldPositionXYZ,
	glm::vec3 meshWorldOrientationEuler,
	float uniformScale)
{
	// 1. Get the mesh (triangle) information
	// 2. For each triangle, and each vertex, 
	//    see what AABB they would be in
	// 3. If there's no AABB there, make one
	// 4. Add those triangles to that AABB

	if (!this->m_pVAOManager)
	{
		// Haven't set the VAO manager pointer, yet
		return false;
	}

	// These is the triangle information from the origianl model
	// i.e. it's in Model Space (like in the file)
	std::vector<cVAOManager::sTriangle> vecVAOTriangles;
	if (!this->m_pVAOManager->getTriangleMeshInfo(meshModelName, vecVAOTriangles))
	{
		// Can't find mesh
		return false;
	}


	for (std::vector<cVAOManager::sTriangle>::iterator itVAOTri = vecVAOTriangles.begin();
		itVAOTri != vecVAOTriangles.end(); itVAOTri++)
	{
		cVAOManager::sTriangle curVAOTri = *itVAOTri;

		// This triangle is in MODEL space, not world space
		sTriangle curTri;
		curTri.vertices[0] = curVAOTri.vertices[0];
		curTri.vertices[1] = curVAOTri.vertices[1];
		curTri.vertices[2] = curVAOTri.vertices[2];
		curTri.normal = curVAOTri.normal;
		curTri.calculateSideLengths();

		// transform this into world space...
		// (just like we do to draw it)


		// Translation (movement, position, placement...)
		glm::mat4 matTranslate
			= glm::translate(glm::mat4(1.0f), meshWorldPositionXYZ);

		// Rotation...
		// Caculate 3 Euler acix matrices...
		glm::mat4 matRotateX =
			glm::rotate(glm::mat4(1.0f),
				glm::radians(meshWorldOrientationEuler.x), // Angle in radians
				glm::vec3(1.0f, 0.0, 0.0f));

		glm::mat4 matRotateY =
			glm::rotate(glm::mat4(1.0f),
				glm::radians(meshWorldOrientationEuler.y), // Angle in radians
				glm::vec3(0.0f, 1.0, 0.0f));

		glm::mat4 matRotateZ =
			glm::rotate(glm::mat4(1.0f),
				glm::radians(meshWorldOrientationEuler.z), // Angle in radians
				glm::vec3(0.0f, 0.0, 1.0f));


		// Scale
		glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
			glm::vec3(uniformScale, uniformScale, uniformScale));

		// Calculate the final model/world matrix
		glm::mat4 matModel = glm::mat4(1.0f);
		matModel *= matTranslate;     // matModel = matModel * matTranslate;
		matModel *= matRotateX;
		matModel *= matRotateY;
		matModel *= matRotateZ;
		matModel *= matScale;


		// We aren't going to compare the actucal triangles from the model.
		// Instead we will compare a bunch of possible tessellated ones.
		std::vector<cPhysics::sTriangle> vec_TessellatedTriangles;

		// Adding the OG triangle
		vec_TessellatedTriangles.push_back(curTri);
		// At this point, this vector has only the original triangle.
		// ...which MIGHT be small enough, but we want to make sure 
		//	ALL of the triangle in this vector are smalle enough
		// 
		// For each triangle, see how big it is.
		// If any side is "too big", then split it into 3
		//	(add three triangles to the vector)
		// Remove the original "too big" triangle
		//
		// We want to compare with SMALLER than the side, not exactly
		//	the same size as the AABB. 
		// i.e. we don't want a triangle that matches exactlt with an 
		//	edge of the AABB cube
		// Picking 3.0 in that the triangles should be smaller than 
		//	1/3 of the length of the AABB
		// You might want to play with this value
		float MinAABBSizeLengthToCompare = AABBCubeSize_or_Width / 3.0f;

		bool bStillHasLargeTriangles = true;
		unsigned int numPasses = 0;
		unsigned int maxNumOfTessellatedTris = 0;
		do 
		{
			numPasses++;

			// Assume all the triangles are "small enough"
			bStillHasLargeTriangles = false;

			// A new vector of potentially smaller triangles 
			std::vector<cPhysics::sTriangle> vec_TEMP_TessellatedTriangles;

			for (cPhysics::sTriangle curTessTriangle : vec_TessellatedTriangles)
			{
				curTessTriangle.calculateSideLengths();
				if (curTessTriangle.sideLengths[0] > MinAABBSizeLengthToCompare || 
					curTessTriangle.sideLengths[1] > MinAABBSizeLengthToCompare ||
					curTessTriangle.sideLengths[2] > MinAABBSizeLengthToCompare)
				{
					// At least one side is "too long" compared to the AABB.
					bStillHasLargeTriangles = true;

					// Gererate FOUR triangles that are small enough
					cPhysics::sTriangle smallerTriangles[4];
					// 
//					glm::vec3 centreOfOriginalTriangle =
//						(curTessTriangle.vertices[0] +
//						 curTessTriangle.vertices[1] +
//						 curTessTriangle.vertices[2]) / 3.0f;

					//	A = (V0 + V1) / 2.0f
					//	B = (V1 + V2) / 2.0f
					//	C = (V2 + V0) / 2.0f
					//
					glm::vec3 vA = (curTessTriangle.vertices[0] + curTessTriangle.vertices[1]) / 2.0f;
					glm::vec3 vB = (curTessTriangle.vertices[1] + curTessTriangle.vertices[2]) / 2.0f;
					glm::vec3 vC = (curTessTriangle.vertices[2] + curTessTriangle.vertices[0]) / 2.0f;

					//	T0: V0, A, C
					//	T1 : A, V1, B
					//	T2 : B, V2, C
					//	T3 : A, B, C

					smallerTriangles[0].vertices[0] = curTessTriangle.vertices[0];
					smallerTriangles[0].vertices[1] = vA;
					smallerTriangles[0].vertices[2] = vC;
					smallerTriangles[0].calculateSideLengths();

					smallerTriangles[1].vertices[0] = vA;
					smallerTriangles[1].vertices[1] = curTessTriangle.vertices[1];
					smallerTriangles[1].vertices[2] = vB;
					smallerTriangles[1].calculateSideLengths();

					smallerTriangles[2].vertices[0] = vB;
					smallerTriangles[2].vertices[1] = curTessTriangle.vertices[2];
					smallerTriangles[2].vertices[2] = vC;
					smallerTriangles[2].calculateSideLengths();

					smallerTriangles[3].vertices[0] = vA;
					smallerTriangles[3].vertices[1] = vB;
					smallerTriangles[3].vertices[2] = vC;
					smallerTriangles[3].calculateSideLengths();

					// Add these three triangles to the new tesselated list
					vec_TEMP_TessellatedTriangles.push_back(smallerTriangles[0]);
					vec_TEMP_TessellatedTriangles.push_back(smallerTriangles[1]);
					vec_TEMP_TessellatedTriangles.push_back(smallerTriangles[2]);
					vec_TEMP_TessellatedTriangles.push_back(smallerTriangles[3]);
				}
				else
				{
					// It's smalle enough, so just add it.
					vec_TEMP_TessellatedTriangles.push_back(curTessTriangle);
				}
			}//for (cPhysics::sTriangle curTessTriangle

			// Copy the tesselated triangles back
			// (Effectively removes any original "too large" triangles)
			vec_TessellatedTriangles.clear();
			vec_TessellatedTriangles = vec_TEMP_TessellatedTriangles;

			if ((unsigned int)vec_TessellatedTriangles.size() > maxNumOfTessellatedTris)
			{
				maxNumOfTessellatedTris = (unsigned int)vec_TessellatedTriangles.size();
			}

		} while (bStillHasLargeTriangles);

		// All triangles are "small enough"


		for (const cPhysics::sTriangle &curTessTriangle : vec_TessellatedTriangles)
		{

			for (unsigned int vertIndex = 0; vertIndex < 3; vertIndex++)
			{

				// Use this model/world matrix to transform the vertices
				// Just like we do it in the vertex shader:
				// 
				// 	fvertexWorldLocation = matModel * vec4(finalVert, 1.0);

				// Comparing to the tessellated...
				glm::vec4 vertexWorldPosition = matModel * glm::vec4(curTessTriangle.vertices[vertIndex], 1.0f);
				// glm::vec4 vertexWorldPosition = matModel * glm::vec4(curTri.vertices[vertIndex], 1.0f);
				// glm::vec4 vertexWorldPosition = glm::vec4(curTri.vertices[vertIndex], 1.0f);

							// HACK:
				int huzzah = 1;
				if (vertexWorldPosition.x > 0.0f &&
					vertexWorldPosition.y > 0.0f &&
					vertexWorldPosition.z > 0.0f)
				{
					huzzah = 2;
				}

				// For each vertex, calculate the AABB index it would be in
				unsigned long long vert_AABB_Index_ID =
					this->calcBP_GridIndex(vertexWorldPosition.x,
						vertexWorldPosition.y,
						vertexWorldPosition.z,
						AABBCubeSize_or_Width);
				// Store the triangle this vertex is in inside the approprirate AABB
				// Is there already an AABB there
				std::map< unsigned long long /*index*/, cBroad_Cube* >::iterator
					itAAAB_V0 = this->map_BP_CubeGrid.find(vert_AABB_Index_ID);

				// There?
				if (itAAAB_V0 == this->map_BP_CubeGrid.end())
				{
					// Nope, so make one

					glm::vec3 minXYZ = this->calcBP_MinXYZ_FromID(vert_AABB_Index_ID, AABBCubeSize_or_Width);
					glm::vec3 maxXYZ = minXYZ + glm::vec3(AABBCubeSize_or_Width);

					cBroad_Cube* pCube = new cBroad_Cube(minXYZ, maxXYZ, AABBCubeSize_or_Width, vert_AABB_Index_ID);

					// Add it to the map
					this->map_BP_CubeGrid[vert_AABB_Index_ID] = pCube;
				}

				// Add that triangle to the AABB 
				// 
				// TODO: We DON'T want to add the TESSELLATED triangles
				//       We want to add the ORIGINAL triangle information
				// 
				// ALSO TODO: We want to make sure we don't add this triangle multiple times
				// (like if the tessellated triangles hit the same box over and over again)
				// 
				this->map_BP_CubeGrid[vert_AABB_Index_ID]->vec_pTriangles.push_back(curTri);

			}//for ( unsigned int vertIndex

	//		unsigned long long vert1_AABB_ID =
	//			this->calcBP_GridIndex(curTri.vertices[1].x, curTri.vertices[1].y, curTri.vertices[1].z, AABBCubeSize_or_Width);
	//		unsigned long long vert2_AABB_ID =
	//			this->calcBP_GridIndex(curTri.vertices[1].x, curTri.vertices[2].y, curTri.vertices[2].z, AABBCubeSize_or_Width);


		}//for (...sTriangle>::iterator itVAOTri...

	}//for ( cPhysics::sTriangle pCurTessTriangle

	return true;
}


//void cPhysics::initBroadPhaseGrid(void)
//{
//	unsigned int GRID_SIZE = 100;
//	//std::vector< std::vector< std::vector< sBroad_Cube* > > > vec_3D_CubeArray;
//	for (unsigned int x = 0; x != GRID_SIZE; x++)
//	{
//		std::vector< std::vector< sBroad_Cube* > > vecTempYRow;
//		for (unsigned int y = 0; y != GRID_SIZE; y++)
//		{
//			std::vector< sBroad_Cube* > vecTempZRow;
//			for (unsigned int z = 0; z != GRID_SIZE; z++)
//			{
//				vecTempZRow.push_back(new sBroad_Cube());
//			}
//			vecTempYRow.push_back(vecTempZRow);
//		}
//		this->vec_3D_CubeArray.push_back(vecTempYRow);
//	}
//	return;
//}

