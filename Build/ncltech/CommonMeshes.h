/******************************************************************************
Class: CommonMeshes
Implements:
Author: Pieran Marris <p.marris@newcastle.ac.uk>
Description: 
A quick and dirty library of common meshes, to save loading the same common meshes over and over again.

These are loaded when the scene is first initialised and released when it is deleted, so will 
be globally availible for the entirity of the program. 

If they are being used within an ObjectMesh instance, remember to set the 'deleteOnCleanup' flag to 
false in order to prevent them being deleted when the ObjectMesh instance is deleted.


		(\_/)
		( '_')
	 /""""""""""""\=========     -----D
	/"""""""""""""""""""""""\
....\_@____@____@____@____@_/

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <nclgl\Mesh.h>

class Scene;

class CommonMeshes
{
	friend class SceneRenderer; //Initializes/Destroys the given meshes within it's own lifecycle

public:

	//Quad
	static Mesh* Plane()			{ return m_pPlane; }

	//Cube
	static Mesh* Cube()				{ return m_pCube; }

	//Sphere
	static Mesh* Sphere()			{ return m_pSphere; }



	//PhysicsEngine Checkerboard - Hidden here for reasons of laziness
	static GLuint CheckerboardTex() { return m_CheckerboardTex; }
	static GLuint EarthTex() { return m_earthTex; }
	static GLuint JupiterTex() { return m_jupiterTex; }
	static GLuint MarsTex() { return m_marsTex; }
	static GLuint MercuryTex() { return m_mercuryTex; }
	static GLuint NeptuneTex() { return m_neptuneTex; }
	static GLuint SaturnTex() { return m_saturnTex; }
	static GLuint SunTex() { return m_sunTex; }
	static GLuint UranusTex() { return m_uranusTex; }
	static GLuint VenusTex() { return m_venusTex; }
	static GLuint AstroidTex() { return m_astroid; }
	static GLuint TargetTex() { return m_target; }
	static GLuint ReloadTex() { return m_reload; }
	static GLuint MccreeTex() { return m_mccree; } //...it's high noon...

protected:
	//Called by SceneRenderer
	static void InitializeMeshes();
	static void ReleaseMeshes();

protected:
	static Mesh* m_pCube;
	static Mesh* m_pSphere;
	static Mesh* m_pPlane;

	static GLuint m_CheckerboardTex;

	static GLuint m_earthTex;
	static GLuint m_jupiterTex;
	static GLuint m_marsTex;
	static GLuint m_mercuryTex;
	static GLuint m_neptuneTex;
	static GLuint m_saturnTex;
	static GLuint m_sunTex;
	static GLuint m_uranusTex;
	static GLuint m_venusTex;
	static GLuint m_astroid;
	static GLuint m_target;
	static GLuint m_reload;
	static GLuint m_mccree; //...it's high noon...

	static GLuint m_skybox;
};