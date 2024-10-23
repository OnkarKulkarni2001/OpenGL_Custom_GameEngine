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
	float avgDistance(0.0f);
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			center.x += scene.pModels[modelIndex].pVertex[vertexIndex].x;

			center.y += scene.pModels[modelIndex].pVertex[vertexIndex].y;
			
			center.z += scene.pModels[modelIndex].pVertex[vertexIndex].z;
		}
		center.x /= scene.pModels[modelIndex].numberOfVertices;	
		center.y /= scene.pModels[modelIndex].numberOfVertices;			
		center.z /= scene.pModels[modelIndex].numberOfVertices;

		for (int vertexIndex = 0; vertexIndex != scene.pModels[modelIndex].numberOfVertices; vertexIndex++) {
			distance += glm::distance(center, glm::vec3(scene.pModels[modelIndex].pVertex[vertexIndex].x, scene.pModels[modelIndex].pVertex[vertexIndex].y, scene.pModels[modelIndex].pVertex[vertexIndex].z));
		}
		avgDistance = distance / (float)scene.pModels[modelIndex].numberOfVertices;

		pBoundingSpheres[modelIndex].center = center;
		pBoundingSpheres[modelIndex].radius = avgDistance;
	}
}

bool cPhysicsUpdated::CheckBoundingSphereCollision(const sBoundingSphere& sphere1, const sBoundingSphere& sphere2)
{
	float distance = glm::distance(sphere1.center, sphere2.center);
	return distance <= (sphere1.radius + sphere2.radius);		// they will collide
}

bool cPhysicsUpdated::CheckAABBCollision(const sAABB& aabb1, const sAABB& aabb2)
{
	return (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x &&
		aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y &&
		aabb1.max.z > aabb2.min.z && aabb1.min.z < aabb2.max.z);	// they will collide
}

bool cPhysicsUpdated::CheckCollision(cScene& scene)
{
	for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
		CalculateBoundingSpheres(scene);
		std::cout << pBoundingSpheres[modelIndex].center.x << " " << pBoundingSpheres[modelIndex].center.y << " " << pBoundingSpheres[modelIndex].center.z << " center of 1st" << std::endl;
		for (int secondModel = 1 + modelIndex; secondModel != scene.numberOfMeshesToLoad; secondModel++) {
			
			std::cout << pBoundingSpheres[secondModel].center.x << " " << pBoundingSpheres[secondModel].center.y << " " << pBoundingSpheres[secondModel].center.z << " center of 2nd" << std::endl;
			if (CheckBoundingSphereCollision(pBoundingSpheres[modelIndex], pBoundingSpheres[secondModel])) {
				std::cout << scene.pModels[modelIndex].numberOfVertices << " colliding with " << scene.pModels[secondModel].numberOfVertices << std::endl;
				return true;
			}
			else {
				std::cout << "not colliding" << std::endl;
				return false;
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