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
		glm::vec3 size;		// parameters (x = width, y = height, z = depth)
		glm::vec3 center;
	};

	struct sLine {
		glm::vec3 start;
		glm::vec3 end;
		float length = glm::distance(end, start);
	};

	struct sSphereSphere_Collision {
		cLoadModels* model;
		sBoundingSphere* collidingSpheres[2];
	};

	struct sAABBAABB_Collision {
		cLoadModels* model;
		sAABB* collidingAABBs[2];
	};

	struct sTriangleTriangle_Collision {
		sTriangleInPhysics* collidingTriangles[2];
	};

	sBoundingSphere* pBoundingSpheres;
	sAABB* pAABB;
	sTriangleInPhysics* pTriangleInPhysics;
	std::vector<std::vector<sTriangleInPhysics>> pAllModelTriangles;

	std::vector<sSphereSphere_Collision> vecCollidingSpheres;
	std::vector<sAABBAABB_Collision> vecCollidingAABBs;
	std::vector<sTriangleTriangle_Collision> vecCollidingTriangles;

	cPhysicsUpdated(cScene& scene);
	~cPhysicsUpdated();
	void DrawDebugSphere(cScene& scene);
	void CopyFacesTosTriangleInPhysics(cScene& scene);
	void CalculateAABB(cScene& scene);
	void CalculateBoundingSpheres(cScene& scene);
	bool CheckBoundingSphereCollision(sBoundingSphere& sphere1, sBoundingSphere& sphere2);
	bool CheckAABBCollision(sAABB& aabb1, sAABB& aabb2);
	bool CheckCollision(cScene& scene);
	bool CheckLineTriangleCollision(sLine& line, sTriangleInPhysics& triangle);
	bool CheckTriangleTriangleCollision(sTriangleInPhysics& triA, sTriangleInPhysics& triB);
	void HandleColllision(cScene& scene);
	float ScalarTriple(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
	void ProjectionOnAxis(glm::vec3 axis, sTriangleInPhysics& triangle, float& minProj, float& maxProj);
};