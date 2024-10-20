#include "cPhysics.h"

void cPhysics::StartPhysics(cScene& scene)
{
	physicsObjects[scene.numberOfMeshesToLoad];
	for (int i = 0; i != scene.pModels.size(); i++) {
		meshModelMap.insert(std::make_pair(physicsObjects[i], scene.pModels[i]));
		physicsObjects[i].objectPosition = glm::vec3(scene.pModels[i].pMeshTransform.x, scene.pModels[i].pMeshTransform.y, scene.pModels[i].pMeshTransform.z);
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

bool cPhysics::IsObjectsColliding(sPhysicsMesh& model1, sPhysicsMesh& model2, float deltaTime)
{
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