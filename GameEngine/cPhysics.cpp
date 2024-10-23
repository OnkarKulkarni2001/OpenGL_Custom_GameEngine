#include "cPhysics.h"
#include <string>

void cPhysics::StartPhysics(cScene& scene)
{
	physicsObjects.resize(scene.pModels.size());
	for (int i = 0; i != scene.pModels.size(); i++) {
		meshModelMap.insert(std::make_pair(i, scene.pModels[i]));
		physicsObjects[i].objectPosition = glm::vec3(scene.pModels[i].pMeshTransform.x, scene.pModels[i].pMeshTransform.y, scene.pModels[i].pMeshTransform.z);
		
		// Create collision mesh based on model vertices
		physicsObjects[i].collisionMesh = new sCollisionMesh();

		// Populate collision mesh from model vertices
		CreateCollisionMeshFromModel(scene.pModels[i], *physicsObjects[i].collisionMesh);
	}
}

bool cPhysics::CollisionCheck(float deltaTime)
{	
	// Loop through each pair of objects
	for (unsigned int i = 0; i < physicsObjects.size(); ++i) {
		for (unsigned int j = i + 1; j < physicsObjects.size(); ++j) {
			// Check for collision between object i and object j
			if (IsObjectsColliding(physicsObjects[i], physicsObjects[j], deltaTime)) {
				// Handle collision here, e.g., resolve collision, apply forces, etc.
				std::cout << "Collision detected between object " << i << " and object " << j << std::endl;
			}
		}
	}
	return false;
}

void cPhysics::UpdateObjects()
{

}

void cPhysics::CreateCollisionMeshFromModel(const cLoadModels& model, sCollisionMesh& collisionMesh)
{
	// Initialize AABB min and max points
	glm::vec3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 maxPoint(FLT_MIN, FLT_MIN, FLT_MIN);

	glm::vec3 center(0.0f);
	float radius = 0.0f;

	// Loop through all vertices to compute bounds
	for (unsigned int i = 0; i < model.numberOfVerticesToRender; i++) {
		const glm::vec3& vertexPos = model.pVerticesToRender[i].vertexPosition;

		// Update AABB min and max points
		minPoint = glm::min(minPoint, vertexPos);
		maxPoint = glm::max(maxPoint, vertexPos);

		// For Sphere: Update the center of the sphere (average position of vertices)
		center += vertexPos;
	}

	// Finalize center of sphere
	center /= model.numberOfVerticesToRender;

	// Compute the bounding sphere radius (maximum distance from the center)
	for (unsigned int i = 0; i < model.numberOfVerticesToRender; i++) {
		const glm::vec3& vertexPos = model.pVerticesToRender[i].vertexPosition;
		float distance = glm::distance(center, vertexPos);
		radius = glm::max(radius, distance);
	}

	// Assign to collision mesh (choose one: AABB or Sphere)
	collisionMesh.collisionMeshType = 1; // AABB
	collisionMesh.aabb.minXYZ = minPoint;
	collisionMesh.aabb.maxXYZ = maxPoint;

	// Alternatively, use the bounding sphere
	collisionMesh.collisionMeshType = 2; // Sphere
	collisionMesh.sphere.sSphereCenter = center;
	collisionMesh.sphere.sSphereRadius = radius;
}

bool cPhysics::IsObjectsColliding(sPhysicsMesh& model1, sPhysicsMesh& model2, float deltaTime)
{
	if (model1.collisionMesh == nullptr || model2.collisionMesh == nullptr) {
		std::cerr << "Collision mesh is null for one or both objects." << std::endl;
		return false;
	}
	// for collision between two AABB
	if (model1.collisionMesh->collisionMeshType == 1 && model2.collisionMesh->collisionMeshType == 1) {
		return (model1.collisionMesh->aabb.maxXYZ.x >= model2.collisionMesh->aabb.minXYZ.x &&
			model1.collisionMesh->aabb.minXYZ.x <= model2.collisionMesh->aabb.maxXYZ.x &&
			model1.collisionMesh->aabb.maxXYZ.y >= model2.collisionMesh->aabb.minXYZ.y &&
			model1.collisionMesh->aabb.minXYZ.y <= model2.collisionMesh->aabb.maxXYZ.y &&
			model1.collisionMesh->aabb.maxXYZ.z >= model2.collisionMesh->aabb.minXYZ.z &&
			model1.collisionMesh->aabb.minXYZ.z <= model2.collisionMesh->aabb.maxXYZ.z);
	}

	// for collsion between two Spheres
	if (model1.collisionMesh->collisionMeshType == 2 && model2.collisionMesh->collisionMeshType == 2) {
		float distance = glm::distance(model1.collisionMesh->sphere.sSphereCenter, model2.collisionMesh->sphere.sSphereCenter);
		float sumOfRadii = model1.collisionMesh->sphere.sSphereRadius + model2.collisionMesh->sphere.sSphereRadius;
		return (distance <= sumOfRadii);
	}

	// for collision between AABB and Sphere
	if (model1.collisionMesh->collisionMeshType == 1 && model2.collisionMesh->collisionMeshType == 2 || model1.collisionMesh->collisionMeshType == 2 && model2.collisionMesh->collisionMeshType == 1) {
		// Get the AABB and sphere data
		const sAABB& aabb = (model1.collisionMesh->collisionMeshType == 1) ? model1.collisionMesh->aabb : model2.collisionMesh->aabb;
		const sSphere& sphere = (model1.collisionMesh->collisionMeshType == 2) ? model1.collisionMesh->sphere : model2.collisionMesh->sphere;

		// Calculate the closest point on the AABB to the sphere's center
		glm::vec3 closestPoint = glm::clamp(sphere.sSphereCenter, aabb.minXYZ, aabb.maxXYZ);

		// Calculate the distance from the sphere's center to the closest point on the AABB
		float distance = glm::distance(sphere.sSphereCenter, closestPoint);

		// If the distance is less than or equal to the sphere's radius, they are colliding
		return (distance <= sphere.sSphereRadius);
	}

	return false;
}