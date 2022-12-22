#include <GL/glaux.h>
#pragma comment ( lib, "glaux.lib" )
#include "MyTexMgr.h"
#include <fstream>
#include <gl/GL.h>
#include <gl/GLU.h>

MyTexMgr::MyTexMgr(void)
{
}

MyTexMgr::~MyTexMgr(void)
{
	if (m_textures.empty())
	{
		return;
	}
	map<wstring,uint>::iterator it;
	for (it=m_textures.begin();it!=m_textures.end();++it)
	{
		glDeleteTextures(1,&it->second);
	}
}

bool MyTexMgr::loadTex( const wstring& texName,const wstring& fileName )
{
	GLuint	newElem;
	glGenTextures(1,&newElem);

	AUX_RGBImageRec*	rec=loadBMP(fileName.c_str());
	if (!rec)
	{
		glDeleteTextures(1,&newElem);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D,newElem);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,rec->sizeX,rec->sizeY,
		0,GL_RGB,GL_UNSIGNED_BYTE,rec->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	if (getTex(texName)!=INVALD_TEXTURE)
	{
		delTex(texName);
	}
	m_textures.insert(make_pair(texName,newElem));
	delete	rec->data;
	free(rec);

	return true;

}

uint MyTexMgr::getTex( const wstring& texName )
{
	map<wstring,uint>::iterator it;
	it=m_textures.find(texName);
	if (it!=m_textures.end())
	{
		return it->second;
	}
	else
	{
		return INVALD_TEXTURE;
	}
}

bool MyTexMgr::delTex( const wstring& texName )
{
	map<wstring,uint>::iterator it;
	it=m_textures.find(texName);
	if (it!=m_textures.end())
	{
		glDeleteTextures(1,&it->second);
		m_textures.erase(it);
	}
	return true;
}

AUX_RGBImageRec* MyTexMgr::loadBMP( const wchar_t* fileName )
{
	wifstream infile(fileName);
	if (infile.is_open())
	{
		return auxDIBImageLoadW(fileName);
	} 
	else
	{
		return NULL;
	}

}

bool MyTexMgr::clear()
{
	if (m_textures.empty())
	{
		return true;
	}
	map<wstring,uint>::iterator it;
	for (it=m_textures.begin();it!=m_textures.end();++it)
	{
		glDeleteTextures(1,&it->second);
	}
	m_textures.clear();
	return true;
}
