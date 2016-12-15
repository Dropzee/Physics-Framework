#include "CommonMeshes.h"
#include <nclgl\OBJMesh.h>

Mesh* CommonMeshes::m_pPlane	= NULL;
Mesh* CommonMeshes::m_pCube		= NULL;
Mesh* CommonMeshes::m_pSphere	= NULL;

GLuint CommonMeshes::m_CheckerboardTex = 0;

GLuint CommonMeshes::m_earthTex = 1;
GLuint CommonMeshes::m_jupiterTex = 2;
GLuint CommonMeshes::m_marsTex = 3;
GLuint CommonMeshes::m_mercuryTex = 4;
GLuint CommonMeshes::m_neptuneTex = 5;
GLuint CommonMeshes::m_saturnTex = 6;
GLuint CommonMeshes::m_sunTex = 7;
GLuint CommonMeshes::m_uranusTex = 8;
GLuint CommonMeshes::m_venusTex = 9;
GLuint CommonMeshes::m_astroid = 10;
GLuint CommonMeshes::m_target = 11;
GLuint CommonMeshes::m_reload = 12;
GLuint CommonMeshes::m_mccree = 13; //...it's high noon...

GLuint CommonMeshes::m_skybox = 14;

void CommonMeshes::InitializeMeshes()
{
	if (m_pPlane == NULL)
	{ 
		m_CheckerboardTex = SOIL_load_OGL_texture(TEXTUREDIR"checkerboard.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_jupiterTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\jupiter.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_marsTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\mars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_mercuryTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_neptuneTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\neptune.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_saturnTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\saturn.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_sunTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_uranusTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\uranus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_venusTex = SOIL_load_OGL_texture(TEXTUREDIR"Planets\\venus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_astroid = SOIL_load_OGL_texture(TEXTUREDIR"astroid.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_target = SOIL_load_OGL_texture(TEXTUREDIR"target.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_reload = SOIL_load_OGL_texture(TEXTUREDIR"reload.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_mccree = SOIL_load_OGL_texture(TEXTUREDIR"mccree.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT); //...it's high noon...

		/*m_skybox = SOIL_load_OGL_cubemap(TEXTUREDIR"/Skybox/GalaxyTex_NegativeX.tga", TEXTUREDIR"/Skybox/GalaxyTex_PositiveX.tga",
			TEXTUREDIR"/Skybox/GalaxyTex_NegativeY.tga", TEXTUREDIR"/Skybox/GalaxyTex_PositiveY.tga",
			TEXTUREDIR"/Skybox/GalaxyTex_NegativeZ.tga", TEXTUREDIR"/Skybox/GalaxyTex_PositiveZ.tga",
			SOIL_LOAD_RGB,
			SOIL_CREATE_NEW_ID, 14
			); */

		glBindTexture(GL_TEXTURE_2D, m_CheckerboardTex);
		glBindTexture(GL_TEXTURE_2D, m_earthTex);
		glBindTexture(GL_TEXTURE_2D, m_jupiterTex);
		glBindTexture(GL_TEXTURE_2D, m_marsTex);
		glBindTexture(GL_TEXTURE_2D, m_mercuryTex);
		glBindTexture(GL_TEXTURE_2D, m_neptuneTex);
		glBindTexture(GL_TEXTURE_2D, m_saturnTex);
		glBindTexture(GL_TEXTURE_2D, m_sunTex);
		glBindTexture(GL_TEXTURE_2D, m_uranusTex);
		glBindTexture(GL_TEXTURE_2D, m_venusTex);
		glBindTexture(GL_TEXTURE_2D, m_astroid);
		glBindTexture(GL_TEXTURE_2D, m_target);
		glBindTexture(GL_TEXTURE_2D, m_reload);
		glBindTexture(GL_TEXTURE_2D, m_mccree); //...it's high noon...

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //No linear interpolation to get crisp checkerboard no matter the scalling

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glBindTexture(GL_TEXTURE_2D, 2);
		glBindTexture(GL_TEXTURE_2D, 3);
		glBindTexture(GL_TEXTURE_2D, 4);
		glBindTexture(GL_TEXTURE_2D, 5);
		glBindTexture(GL_TEXTURE_2D, 6);
		glBindTexture(GL_TEXTURE_2D, 7);
		glBindTexture(GL_TEXTURE_2D, 8);
		glBindTexture(GL_TEXTURE_2D, 9);
		glBindTexture(GL_TEXTURE_2D, 10);
		glBindTexture(GL_TEXTURE_2D, 11);
		glBindTexture(GL_TEXTURE_2D, 12);
		glBindTexture(GL_TEXTURE_2D, 13); //...it's high noon...

		m_pPlane = Mesh::GenerateQuadTexCoordCol(Vector2(1.f, 1.f), Vector2(0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pCube = new OBJMesh(MESHDIR"cube.obj");
		m_pSphere = new OBJMesh(MESHDIR"sphere.obj");

		m_pPlane->SetTexture(m_CheckerboardTex);
		m_pCube->SetTexture(m_CheckerboardTex);
		m_pSphere->SetTexture(m_CheckerboardTex);
	}
}

void CommonMeshes::ReleaseMeshes()
{
	if (m_pPlane != NULL)
	{
		glDeleteTextures(1, &m_CheckerboardTex);
		delete m_pPlane;
		delete m_pCube;
		delete m_pSphere;
	}

	m_pPlane = NULL;
}