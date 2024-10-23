#pragma once
#include "cLoadModels.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <vector>
#include "cScene.h"

class cPhysicsUpdated
{
public:
	struct sTriangleInPhysics {
		glm::vec3 vertex1;
		glm::vec3 vertex2;
		glm::vec3 vertex3;
	};

	struct sBoundingSphere {
		glm::vec3 center;
		float radius;
	};
	
	struct sAABB {
		glm::vec3 min;
		glm::vec3 max;
	};

	sBoundingSphere* pBoundingSpheres;
	sAABB* pAABB;
	sTriangleInPhysics* pTriangleInPhysics;
	std::vector<std::vector<sTriangleInPhysics>> pAllModelTriangles;

	cPhysicsUpdated(cScene& scene);
	~cPhysicsUpdated();
	void DrawDebugSphere(cScene& scene);
	void CopyFacesTosTriangleInPhysics(cScene& scene);
	void CalculateAABB(cScene& scene);
	void CalculateBoundingSpheres(cScene& scene);
	bool CheckBoundingSphereCollision(cScene& scene);
	bool CheckAABBCollision(const sAABB& aabb1, const sAABB& aabb2);
	bool CheckCollision(cScene& scene);
	bool CheckTriangleTriangleCollision(const sTriangleInPhysics& tri1, const sTriangleInPhysics& tri2, cScene& scene);
	void HandleColllision(cScene& scene);
};