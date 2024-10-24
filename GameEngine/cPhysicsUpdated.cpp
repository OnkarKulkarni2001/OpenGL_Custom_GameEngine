#include "cPhysicsUpdated.h"

cPhysicsUpdated::cPhysicsUpdated(cScene& scene)
{
	CopyFacesTosTriangleInPhysics(scene);
}

cPhysicsUpdated::~cPhysicsUpdated()
{
	delete[] pTriangleInPhysics;
	delete[] pBoundingSpheres;
	delete[] pAABB;
}

void cPhysicsUpdated::DrawDebugSphere(cScene& scene)
{

}

void cPhysicsUpdated::CopyFacesTosTriangleInPhysics(cScene& scene)
{
	pAllModelTriangles.resize(scene.numberOfMeshesToLoad);

	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		pTriangleInPhysics = new sTriangleInPhysics[scene.pModels[modelIndex].numberOfFaces];

		for (int faceIndex = 0; faceIndex != scene.pModels[modelIndex].numberOfFaces; faceIndex++) {
			unsigned int v1 = scene.pModels[modelIndex].pFaces[faceIndex].vertexNumber1;
			unsigned int v2 = scene.pModels[modelIndex].pFaces[faceIndex].vertexNumber2;
			unsigned int v3 = scene.pModels[modelIndex].pFaces[faceIndex].vertexNumber3;

			pTriangleInPhysics[faceIndex].vertex1 = glm::vec3(scene.pModels[modelIndex].pVertex[v1].x, scene.pModels[modelIndex].pVertex[v1].y, scene.pModels[modelIndex].pVertex[v1].z);
			pTriangleInPhysics[faceIndex].vertex2 = glm::vec3(scene.pModels[modelIndex].pVertex[v2].x, scene.pModels[modelIndex].pVertex[v2].y, scene.pModels[modelIndex].pVertex[v2].z);
			pTriangleInPhysics[faceIndex].vertex3 = glm::vec3(scene.pModels[modelIndex].pVertex[v3].x, scene.pModels[modelIndex].pVertex[v3].y, scene.pModels[modelIndex].pVertex[v3].z);

			pAllModelTriangles[modelIndex].push_back(pTriangleInPhysics[faceIndex]);
		}
	}
}

void cPhysicsUpdated::CalculateAABB(cScene& scene)
{
	pAABB = new sAABB[scene.pModels.size()];
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		glm::vec3 min = glm::vec3(scene.pModels[modelIndex].pVertex[0].x, scene.pModels[modelIndex].pVertex[0].y, scene.pModels[modelIndex].pVertex[0].z);
		glm::vec3 max = glm::vec3(scene.pModels[modelIndex].pVertex[0].x, scene.pModels[modelIndex].pVertex[0].y, scene.pModels[modelIndex].pVertex[0].z);

		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			min = glm::min(min, glm::vec3(scene.pModels[modelIndex].pVertex[vertexIndex].x, scene.pModels[modelIndex].pVertex[vertexIndex].y, scene.pModels[modelIndex].pVertex[vertexIndex].z));
			max = glm::max(max, glm::vec3(scene.pModels[modelIndex].pVertex[vertexIndex].x, scene.pModels[modelIndex].pVertex[vertexIndex].y, scene.pModels[modelIndex].pVertex[vertexIndex].z));
		}

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
			center.x += scene.pModels[modelIndex].pVertex[vertexIndex].x + scene.pModels[modelIndex].pMeshTransform.x;

			center.y += scene.pModels[modelIndex].pVertex[vertexIndex].y + scene.pModels[modelIndex].pMeshTransform.y;
			
			center.z += scene.pModels[modelIndex].pVertex[vertexIndex].z + scene.pModels[modelIndex].pMeshTransform.z;
		}
		center.x /= scene.pModels[modelIndex].numberOfVertices;	
		center.y /= scene.pModels[modelIndex].numberOfVertices;			
		center.z /= scene.pModels[modelIndex].numberOfVertices;

		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			distance = glm::distance(center, glm::vec3(scene.pModels[modelIndex].pVertex[vertexIndex].x + scene.pModels[modelIndex].pMeshTransform.x,
				scene.pModels[modelIndex].pVertex[vertexIndex].y + scene.pModels[modelIndex].pMeshTransform.y, 
				scene.pModels[modelIndex].pVertex[vertexIndex].z + scene.pModels[modelIndex].pMeshTransform.z));

			if (maxDistance < distance) {
				maxDistance = distance;
			}
		}

		pBoundingSpheres[modelIndex].center = center;
		pBoundingSpheres[modelIndex].radius = maxDistance;
	}
}

bool cPhysicsUpdated::CheckBoundingSphereCollision(cScene& scene)
{
	float distance = 0.0f;
	for (int model1Index = 0; model1Index != scene.numberOfMeshesToLoad; model1Index++) {
		for (int model2Index = model1Index + 1; model2Index != scene.numberOfMeshesToLoad; model2Index++) {
			distance = glm::distance(pBoundingSpheres[model1Index].center, pBoundingSpheres[model2Index].center);
			if (distance <= (pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius)) {
				/*std::cout << "Intersection point: (" << pBoundingSpheres[model1Index].center.x + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.x - pBoundingSpheres[model1Index].center.x) << ", "
					<< pBoundingSpheres[model1Index].center.y + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.y - pBoundingSpheres[model1Index].center.y) << ", "
					<< pBoundingSpheres[model1Index].center.z + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.z - pBoundingSpheres[model1Index].center.z) << ")" << std::endl;*/
			}
			return distance <= (pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius);		// they will collide
		}
	}
}

bool cPhysicsUpdated::CheckAABBCollision(cScene& scene)
{
	for (int model1Index = 0; model1Index != scene.numberOfMeshesToLoad; model1Index++) {
		for (int model2Index = model1Index + 1; model2Index != scene.numberOfMeshesToLoad; model2Index++) {
			if (pAABB[model1Index].max.x > pAABB[model2Index].min.x && pAABB[model1Index].min.x < pAABB[model2Index].max.x &&
				pAABB[model1Index].max.y > pAABB[model2Index].min.y && pAABB[model1Index].min.y < pAABB[model2Index].max.y &&
				pAABB[model1Index].max.z > pAABB[model2Index].min.z && pAABB[model1Index].min.z < pAABB[model2Index].max.z) {
				
				return true;
			}
		}
	}
	return false;
}

bool cPhysicsUpdated::CheckCollision(cScene& scene)
{
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		for (int secondModel = modelIndex + 1; secondModel != scene.numberOfMeshesToLoad; secondModel++) {
			CalculateBoundingSpheres(scene);
			
			//// Print bounding sphere details for debugging
			//std::cout << "Model " << modelIndex << ": Center = ("
			//	<< pBoundingSpheres[modelIndex].center.x << ", "
			//	<< pBoundingSpheres[modelIndex].center.y << ", "
			//	<< pBoundingSpheres[modelIndex].center.z << ")" << std::endl;

			//std::cout << "Model " << secondModel << ": Center = ("
			//	<< pBoundingSpheres[secondModel].center.x << ", "
			//	<< pBoundingSpheres[secondModel].center.y << ", "
			//	<< pBoundingSpheres[secondModel].center.z << ")" << std::endl;

			if (CheckBoundingSphereCollision(scene)) {
				//std::cout << "Collision detected of spheres: Model " << modelIndex << " is colliding with Model " << secondModel << std::endl;

				CalculateAABB(scene);

				// Print aabb details for debugging
				std::cout << "aabb Model " << modelIndex << ": Center = ("
					<< pAABB[modelIndex].center.x << ", "
					<< pAABB[modelIndex].center.y << ", "
					<< pAABB[modelIndex].center.z << ")" << std::endl;

				std::cout << "aabb Model " << secondModel << ": Center = ("
					<< pAABB[secondModel].center.x << ", "
					<< pAABB[secondModel].center.y << ", "
					<< pAABB[secondModel].center.z << ")" << std::endl;

				if (CheckAABBCollision(scene)) {
					std::cout << "Collision detected of aabbs: Model " << modelIndex << " is colliding with Model " << secondModel << std::endl;
				}
			}
		}
	}
	return false;
}

bool cPhysicsUpdated::CheckTriangleTriangleCollision(const sTriangleInPhysics& tri1, const sTriangleInPhysics& tri2, cScene& scene)
{
	return false;
}

void cPhysicsUpdated::HandleColllision(cScene& scene)
{

}