#pragma once
#include "cScene.h"
#include "cLoadModels.h"
#include "cLightManager.h"
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <string>

class cPhysics
{
public:
	float gravity = 9.81;
	
	struct sAABB {
		glm::vec3 minXYZ;		// min xyz coordinates of bounding box
		glm::vec3 maxXYZ;		// max -------------------------------

		glm::vec3 sAABBCenter() const {
			return (minXYZ + maxXYZ) * 0.5f;
		}

		glm::vec3 sAABBSize() const {
			return maxXYZ - minXYZ;
		}
	};

	struct sSphere {
		glm::vec3 sSphereCenter;
		float sSphereRadius;
	};

	struct sCollisionMesh {
		int collisionMeshType;			// if 1 then AABB if 2 then Sphere
		sAABB aabb;
		sSphere sphere;
		//cLoadModels& modelMesh;
		//cLightManager& lightMesh;		// If light also wants to move by using physics
	};

	struct sPhysicsMesh {
		float mass = 1.0f;;
		float restitution = 0.8f;		// usually used as velocity.y *= -restitution;
		float friction = 0.01f;			// usually used as velocity.x *= friction;
		bool bIsOpaque = false;

		sCollisionMesh* collisionMesh;

		glm::vec3 objectPosition;
		glm::vec3 objectVelocity = glm::vec3(0.0f);
		glm::vec3 objectAccelaration = glm::vec3(0.0f);
	};

	std::vector<cPhysics::sPhysicsMesh> physicsObjects;

	std::map<int, cLoadModels> meshModelMap;

	void StartPhysics(cScene& scene);
	bool CollisionCheck(float deltaTime);		// Runs collision check and if objects collide returns true/false
	void UpdateObjects();
	void CreateCollisionMeshFromModel(const cLoadModels& model, sCollisionMesh& collisionMesh);

	bool IsObjectsColliding(sPhysicsMesh& model1, sPhysicsMesh& model2, float deltaTime);		// Checks if objects are colliding or not
};