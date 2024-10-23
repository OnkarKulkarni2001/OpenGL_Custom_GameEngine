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
	pAABB = new sAABB();
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		glm::vec3 min = scene.pModels[modelIndex].pVerticesToRender[0].vertexPosition;
		glm::vec3 max = scene.pModels[modelIndex].pVerticesToRender[0].vertexPosition;

		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			min = glm::min(min, scene.pModels[modelIndex].pVerticesToRender->vertexPosition[vertexIndex]);
			max = glm::max(max, scene.pModels[modelIndex].pVerticesToRender->vertexPosition[vertexIndex]);
		}

		pAABB[modelIndex].max = max;
		pAABB[modelIndex].min = min;
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
				std::cout << "Intersection point: (" << pBoundingSpheres[model1Index].center.x + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.x - pBoundingSpheres[model1Index].center.x) << ", "
					<< pBoundingSpheres[model1Index].center.y + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.y - pBoundingSpheres[model1Index].center.y) << ", "
					<< pBoundingSpheres[model1Index].center.z + (pBoundingSpheres[model1Index].radius / pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius) * (pBoundingSpheres[model2Index].center.z - pBoundingSpheres[model1Index].center.z) << ")" << std::endl;
			}
			return distance <= (pBoundingSpheres[model1Index].radius + pBoundingSpheres[model2Index].radius);		// they will collide
		}
	}
}

bool cPhysicsUpdated::CheckAABBCollision(const sAABB& aabb1, const sAABB& aabb2)
{
	return (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x &&
		aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y &&
		aabb1.max.z > aabb2.min.z && aabb1.min.z < aabb2.max.z);	// they will collide
}

bool cPhysicsUpdated::CheckCollision(cScene& scene)
{
	CalculateBoundingSpheres(scene);
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		for (int secondModel = modelIndex + 1; secondModel != scene.numberOfMeshesToLoad; secondModel++) {
			
			// Print bounding sphere details for debugging
			std::cout << "Model " << modelIndex << ": Center = ("
				<< pBoundingSpheres[modelIndex].center.x << ", "
				<< pBoundingSpheres[modelIndex].center.y << ", "
				<< pBoundingSpheres[modelIndex].center.z << ")" << std::endl;

			std::cout << "Model " << secondModel << ": Center = ("
				<< pBoundingSpheres[secondModel].center.x << ", "
				<< pBoundingSpheres[secondModel].center.y << ", "
				<< pBoundingSpheres[secondModel].center.z << ")" << std::endl;

			if (CheckBoundingSphereCollision(scene)) {
				std::cout << "Collision detected: Model " << modelIndex << " is colliding with Model " << secondModel << std::endl;
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