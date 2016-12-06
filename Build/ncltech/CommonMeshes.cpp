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