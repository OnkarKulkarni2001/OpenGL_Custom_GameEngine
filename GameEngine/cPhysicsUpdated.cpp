#include "cPhysicsUpdated.h"

cPhysicsUpdated::cPhysicsUpdated(cScene& scene)
{
	vecCollidingAABBs.resize(scene.pModels.size());
	vecCollidingSpheres.resize(scene.pModels.size());
	CopyFacesTosTriangleInPhysics(scene);
}

cPhysicsUpdated::~cPhysicsUpdated()
{
	delete[] pTriangleInPhysics;
	delete[] pBoundingSpheres;
	delete[] pAABB;
}

void cPhysicsUpdated::CopyFacesTosTriangleInPhysics(cScene& scene)
{
	pAllModelTriangles.resize(scene.numberOfMeshesToLoad);
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		//pTriangleInPhysics = new sTriangleInPhysics[scene.pModels[modelIndex].numberOfFaces];
		pAllModelTriangles[modelIndex].resize(scene.pModels[modelIndex].numberOfFaces);
		pTriangleInPhysics = new sTriangleInPhysics[pAllModelTriangles[modelIndex].size()];

		for (int faceIndex = 0; faceIndex != scene.pModels[modelIndex].numberOfFaces; faceIndex++) {
			unsigned int v1 = scene.pModels[modelIndex].pFaces[faceIndex].vertexNumber1;
			unsigned int v2 = scene.pModels[modelIndex].pFaces[faceIndex].vertexNumber2;
			unsigned int v3 = scene.pModels[modelIndex].pFaces[faceIndex].vertexNumber3;

			sTriangleInPhysics triangle;

			triangle.vertex1 = scene.pModels[modelIndex].pTransformedVertices[v1].transformedVertex;
			triangle.vertex2 = scene.pModels[modelIndex].pTransformedVertices[v2].transformedVertex;
			triangle.vertex3 = scene.pModels[modelIndex].pTransformedVertices[v3].transformedVertex;
			//pTriangleInPhysics[faceIndex].vertex1 = scene.pModels[modelIndex].pTransformedVertices[v1].transformedVertex;
			//pTriangleInPhysics[faceIndex].vertex2 = scene.pModels[modelIndex].pTransformedVertices[v2].transformedVertex;
			//pTriangleInPhysics[faceIndex].vertex3 = scene.pModels[modelIndex].pTransformedVertices[v3].transformedVertex;
			pAllModelTriangles[modelIndex].push_back(triangle);

			pTriangleInPhysics = &triangle;
		}
		//pAllModelTriangles[modelIndex].push_back(pTriangleInPhysics);
	}
}

void cPhysicsUpdated::CalculateAABB(cScene& scene)
{
	pAABB = new sAABB[scene.pModels.size()];
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		glm::vec3 min = scene.pModels[modelIndex].pTransformedVertices[0].transformedVertex;
		glm::vec3 max = scene.pModels[modelIndex].pTransformedVertices[0].transformedVertex;

		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			min = glm::min(min, scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex);
			max = glm::max(max, scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex);
		}

		// Just for debug
		//std::cout << "Model " << modelIndex << " AABB: min = ("
		//	<< min.x << ", " << min.y << ", " << min.z
		//	<< ") max = (" << max.x << ", " << max.y << ", " << max.z << ")\n";

		pAABB[modelIndex].max = max;
		pAABB[modelIndex].min = min;

		// Calculate and store the size, height, and width of the AABB
		pAABB[modelIndex].size.x = max.x - min.x;  // Width (X-axis)
		pAABB[modelIndex].size.y = max.y - min.y;  // Height (Y-axis)
		pAABB[modelIndex].size.z = max.z - min.z;  // Depth (Z-axis)
		pAABB[modelIndex].center = (min + max) / 2.0f;
	}
}

void cPhysicsUpdated::CalculateBoundingSpheres(cScene& scene)
{
	pBoundingSpheres = new sBoundingSphere[scene.pModels.size()];
	glm::vec3 center(0.0f);
	float distance(0.0f);
	float maxDistance(0.0f);
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			center.x += scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex.x;
			center.y += scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex.y;
			center.z += scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex.z;
		}
		center.x /= scene.pModels[modelIndex].numberOfVertices;	
		center.y /= scene.pModels[modelIndex].numberOfVertices;			
		center.z /= scene.pModels[modelIndex].numberOfVertices;

		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			distance = glm::distance(center, glm::vec3(scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex.x,
				scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex.y,
				scene.pModels[modelIndex].pTransformedVertices[vertexIndex].transformedVertex.z));

			if (maxDistance < distance) {
				maxDistance = distance;
			}
		}

		pBoundingSpheres[modelIndex].center = center;
		pBoundingSpheres[modelIndex].radius = maxDistance;
	}
}

bool cPhysicsUpdated::CheckBoundingSphereCollision(sBoundingSphere& sphere1, sBoundingSphere& sphere2)
{
	float distance = glm::distance(sphere1.center, sphere2.center);
	if (distance <= (sphere1.radius + sphere2.radius)) {
		return true;		// they will collide
	}
	//for (int model1Index = 0; model1Index != scene.numberOfMeshesToLoad; model1Index++) {
	//	for (int model2Index = model1Index + 1; model2Index != scene.numberOfMeshesToLoad; model2Index++) {
	//		float distance = glm::distance(pBoundingSpheres[model1Index].center, pBoundingSpheres[model2Index].center);
	//		if (distance <= (pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius)) {
	//			/*std::cout << "Intersection point: (" << pBoundingSpheres[model1Index].center.x + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.x - pBoundingSpheres[model1Index].center.x) << ", "
	//				<< pBoundingSpheres[model1Index].center.y + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.y - pBoundingSpheres[model1Index].center.y) << ", "
	//				<< pBoundingSpheres[model1Index].center.z + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.z - pBoundingSpheres[model1Index].center.z) << ")" << std::endl;*/
	//			return true;		// they will collide
	//		}
	//	}
	//}
	return false;
}

bool cPhysicsUpdated::CheckAABBCollision(sAABB& aabb1, sAABB& aabb2)
{
	if (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x &&
		aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y &&
		aabb1.max.z > aabb2.min.z && aabb1.min.z < aabb2.max.z) {
		return true;
	}
	//for (int model1Index = 0; model1Index != scene.numberOfMeshesToLoad; model1Index++) {
	//	for (int model2Index = model1Index + 1; model2Index != scene.numberOfMeshesToLoad; model2Index++) {
	//		if (pAABB[model1Index].max.x > pAABB[model2Index].min.x && pAABB[model1Index].min.x < pAABB[model2Index].max.x &&
	//			pAABB[model1Index].max.y > pAABB[model2Index].min.y && pAABB[model1Index].min.y < pAABB[model2Index].max.y &&
	//			pAABB[model1Index].max.z > pAABB[model2Index].min.z && pAABB[model1Index].min.z < pAABB[model2Index].max.z) {
	//			
	//			//std::cout << "X-axis overlap: " << (pAABB[model1Index].max.x > pAABB[model2Index].min.x &&
	//			//	pAABB[model1Index].min.x < pAABB[model2Index].max.x) << "\n";
	//			//std::cout << "Y-axis overlap: " << (pAABB[model1Index].max.y > pAABB[model2Index].min.y &&
	//			//	pAABB[model1Index].min.y < pAABB[model2Index].max.y) << "\n";
	//			//std::cout << "Z-axis overlap: " << (pAABB[model1Index].max.z > pAABB[model2Index].min.z &&
	//			//	pAABB[model1Index].min.z < pAABB[model2Index].max.z) << "\n";
	//			return true;
	//		}
	//	}
	//}
	return false;
}

bool cPhysicsUpdated::CheckCollision(cScene& scene)
{
	CalculateBoundingSpheres(scene);
	CalculateAABB(scene);

	vecCollidingSpheres.clear();
	vecCollidingAABBs.clear();
	vecCollidingTriangles.clear();

	pTriangleInPhysics = new sTriangleInPhysics[scene.numberOfMeshesToLoad];

	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		for (int secondModel = modelIndex + 1; secondModel != scene.numberOfMeshesToLoad; secondModel++) {
			
			//// Print bounding sphere details for debugging
			//std::cout << "Model " << modelIndex << ": Center = ("
			//	<< pBoundingSpheres[modelIndex].center.x << ", "
			//	<< pBoundingSpheres[modelIndex].center.y << ", "
			//	<< pBoundingSpheres[modelIndex].center.z << ")" << std::endl;

			//std::cout << "Model " << secondModel << ": Center = ("
			//	<< pBoundingSpheres[secondModel].center.x << ", "
			//	<< pBoundingSpheres[secondModel].center.y << ", "
			//	<< pBoundingSpheres[secondModel].center.z << ")" << std::endl;
			sSphereSphere_Collision collidingSpheresInfo;

			if (CheckBoundingSphereCollision(pBoundingSpheres[modelIndex], pBoundingSpheres[secondModel])) {
				
				collidingSpheresInfo.collidingSpheres[0] = &pBoundingSpheres[modelIndex];
				collidingSpheresInfo.collidingSpheres[1] = &pBoundingSpheres[secondModel];
				//collidingSpheresInfo.model[0] = scene.pModels[modelIndex];
				//collidingSpheresInfo.model[1] = scene.pModels[secondModel];

				vecCollidingSpheres.push_back(collidingSpheresInfo);
				//std::cout << "Collision detected of spheres: Model " << modelIndex << " is colliding with Model " << secondModel << std::endl;

				 //Print aabb details for debugging
				//std::cout << "aabb Model " << modelIndex << ": Center = ("
				//	<< pAABB[modelIndex].center.x << ", "
				//	<< pAABB[modelIndex].center.y << ", "
				//	<< pAABB[modelIndex].center.z << ") Size: ("
				//	<< pAABB[modelIndex].size.x << ", "
				//	<< pAABB[modelIndex].size.y << ", "
				//	<< pAABB[modelIndex].size.z << ")" << std::endl;
				//
				//std::cout << "aabb Model " << secondModel << ": Center = ("
				//	<< pAABB[secondModel].center.x << ", "
				//	<< pAABB[secondModel].center.y << ", "
				//	<< pAABB[secondModel].center.z << ") Size: ("
				//	<< pAABB[secondModel].size.x << ", "
				//	<< pAABB[secondModel].size.y << ", "
				//	<< pAABB[secondModel].size.z << ")" << std::endl;
				sAABBAABB_Collision collidingAABBsInfo;

				if (CheckAABBCollision(pAABB[modelIndex], pAABB[secondModel])) {

					collidingAABBsInfo.collidingAABBs[0] = &pAABB[modelIndex];
					collidingAABBsInfo.collidingAABBs[1] = &pAABB[secondModel];
					//collidingAABBsInfo.model[0] = scene.pModels[modelIndex];
					//collidingAABBsInfo.model[1] = scene.pModels[secondModel];

					vecCollidingAABBs.push_back(collidingAABBsInfo);

					//std::cout << "Collision detected of aabbs: Model " << modelIndex << " is colliding with Model " << secondModel << std::endl;
					sTriangleTriangle_Collision collidingTrianglesInfo;

					//std::cout << "Checking collision between triangles:\n";
					//std::cout << "Triangle A: ("
					//	<< pTriangleInPhysics[modelIndex].vertex1.x << ", "
					//	<< pTriangleInPhysics[modelIndex].vertex1.y << ", "
					//	<< pTriangleInPhysics[modelIndex].vertex1.z << ")\n";
					//std::cout << "Triangle B: ("
					//	<< pTriangleInPhysics[secondModel].vertex1.x << ", "
					//	<< pTriangleInPhysics[secondModel].vertex1.y << ", "
					//	<< pTriangleInPhysics[secondModel].vertex1.z << ")\n";

					if (CheckTriangleTriangleCollision(pTriangleInPhysics[modelIndex], pTriangleInPhysics[secondModel])) {
						collidingTrianglesInfo.collidingTriangles[0] = &pTriangleInPhysics[modelIndex];
						collidingTrianglesInfo.collidingTriangles[1] = &pTriangleInPhysics[secondModel];

						vecCollidingTriangles.push_back(collidingTrianglesInfo);
						std::cout << "Boom! Models collide!" << std::endl;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool cPhysicsUpdated::CheckLineTriangleCollision(sLine& line, sTriangleInPhysics& triangle)
{
	glm::vec3 pq = line.end - line.start;		//	Vector pq = q - p;
	glm::vec3 pa = triangle.vertex1 - line.start;	//	Vector pa = a - p;
	glm::vec3 pb = triangle.vertex2 - line.start;	//	Vector pb = b - p;
	glm::vec3 pc = triangle.vertex3 - line.start;	//	Vector pc = c - p;

	float u = ScalarTriple(pq, pc, pb);		//	u = ScalarTriple(pq, pc, pb);
	if (u < 0.0f) {
		return false;
	}
	float v = ScalarTriple(pq, pa, pc);		//	v = ScalarTriple(pq, pa, pc);
	if (v < 0.0f) {
		return false;
	}
	float w = ScalarTriple(pq, pb, pa);		//	w = ScalarTriple(pq, pb, pa);
	if (w < 0.0f) {
		return false;
	}

	float denom = 1.0f / (u + v + w);
	u *= denom;
	v *= denom;
	w *= denom; // w = 1.0f - u - v;

	return true;
}

float cPhysicsUpdated::ScalarTriple(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	return glm::dot(a, glm::cross(b, c));
}

void cPhysicsUpdated::ProjectionOnAxis(glm::vec3 axis, sTriangleInPhysics& triangle, float& minProj, float& maxProj)
{
	float Proj1 = glm::dot(axis, triangle.vertex1);
	float Proj2 = glm::dot(axis, triangle.vertex2);
	float Proj3 = glm::dot(axis, triangle.vertex3);

	minProj = std::min({ Proj1, Proj2, Proj3 });
	maxProj = std::max({ Proj1, Proj2, Proj3 });
}

bool cPhysicsUpdated::CheckTriangleTriangleCollision(sTriangleInPhysics& triA, sTriangleInPhysics& triB)
{
	glm::vec3 triAEdges[3] = { triA.vertex2 - triA.vertex1, triA.vertex3 - triA.vertex2, triA.vertex1 - triA.vertex3 };
	glm::vec3 triBEdges[3] = { triB.vertex2 - triB.vertex1, triB.vertex3 - triB.vertex2, triB.vertex1 - triB.vertex3 };

	glm::vec3 triANormal = glm::cross(triAEdges[0], triAEdges[1]);
	glm::vec3 triBNormal = glm::cross(triBEdges[0], triBEdges[1]);

	std::vector<glm::vec3> axes = { triANormal, triBNormal };

	for (int triAEdgeIndex = 0; triAEdgeIndex < 3; triAEdgeIndex++) {
		for (int triBEdgeIndex = 0; triBEdgeIndex < 3; triBEdgeIndex++) {
			glm::vec3 axis = glm::cross(triAEdges[triAEdgeIndex], triBEdges[triBEdgeIndex]);
			if (glm::length(axis) > 1e-6) {
				axes.push_back(glm::normalize(axis));
			}
		}
	}

	for (const std::vector<glm::vec3>::iterator::value_type& axis : axes) {
		float min1, max1, min2, max2;
		ProjectionOnAxis(axis, triA, min1, max1);
		ProjectionOnAxis(axis, triB, min2, max2);

		if (max1 < min2 || max2 < min1) {
			return false;
		}
	}

	return true;
}

void cPhysicsUpdated::HandleColllision(cScene& scene)
{

}
