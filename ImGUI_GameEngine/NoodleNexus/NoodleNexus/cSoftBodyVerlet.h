#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <vector>

//#include "cVAOManager/sModelDrawInfo.h"
#include "cVAOManager/cVAOManager.h"	// for: sModelDrawInfo	

class cSoftBodyVerlet
{
public:
	cSoftBodyVerlet();
	~cSoftBodyVerlet();

	// This is for loading the original model
	// The 2nd param is the identy matrix 
	bool CreateSoftBody(sModelDrawInfo ModelDrawInfo, glm::mat4 matInitalTransform = glm::mat4(1.0f));

	// Will update the vertex information to match the particles
	void UpdateVertexPositions(void);

	void UpdateNormals(void);


	// Create random constraints within the object to 'brace' the shape
	// These are invisible, though
	void CreateRandomBracing(unsigned int numberOfBraces,
							 float minDistanceBetweenVertices);

	// This will update the draw info vertex information from the 
	//	soft body simulation state (at the current moment)
//	bool UpdateDrawInfo(void);

	void VerletUpdate(double deltaTime);

	// This is for the ground, hitting something, etc.
	void ApplyCollision(double deltaTime);

	void SatisfyConstraints(void);


	typedef sVertex_SHADER_FORMAT_xyz_rgb_N_UV sVertex;

	// This represents a vertex or point or whatever in the soft body
	struct sParticle
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 old_position = glm::vec3(0.0f);
		// Pointer back to the model vertex info
		sVertex* pModelVertex = NULL;					// See typedef above...
		//
		// If "fixed" then don't update with Verlet step
		bool bIsFixed_DontUpdate = false;
	};

	void cleanZeros(glm::vec3& value);


	float calcDistanceBetween(sParticle* pPartA, sParticle* pPartB);

	// These are the "rods" or "springs" between vertices
	// We get these from the mesh, BUT we might need additional
	//	constraints to make sure this stays in the desired shape
	struct sConstraint
	{
		sConstraint() {}
		sParticle* pParticleA = NULL;
		sParticle* pParticleB = NULL;
		// How far away they are supposed to be 'at rest'
		float restLength = 0.0f;

		unsigned int maxIterations = 1;

		// if false, this isnt' checked
		// Like if the constraint is 'broken'
		bool bIsActive = true;		

		// Maybe a maximum distance between vertices:
		// If it was stiff cloth, maybe set this to just longer than the rest length.
		// It it's really "springy" (like spandex?) then set this to a lot longer.
		float breakingDistance = FLT_MAX;	
		bool bIsBreakable = false;

		// This is interesting, too
		// From: I Spent a Week Making an AI's Video Game Idea - YouTube
		// https://www.youtube.com/watch?v=PGk0rnyTa1U&ab_channel=SebastianLague
		// He's using it as to fix the end of the constraint
		bool bIsLocked = false;
	};

	// Force, like gravity or whatever
	glm::vec3 acceleration = glm::vec3(0.0f);

	// This is the "guts" of the soft body
	std::vector< sParticle* > vec_pParticles;			// All the vertices
	std::vector< sConstraint* > vec_pConstraints;		// All "springs" or whatever

	// The mesh model name that the VAO has 
	// i.e. the drawing mesh that matches this particular soft body
	std::string matching_VAO_MeshName;	
	sModelDrawInfo m_ModelVertexInfo;

	// This method is public and has THE SAME critical section
	//	we have in the Update vertex and normal code
	sModelDrawInfo* pGetModelDrawInfo(void);

	// Max update time is 60Hz
	const double MAX_DELTATIME = 1.0 / 60.0;



	// This just gets the average of all the points (based on location)
	glm::vec3 getGeometricCentrePoint(void);

	// This would  take into account the mass, so would "look" more accurate, maybe?
	glm::vec3 getCentreOfMass(void);


};

