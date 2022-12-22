#pragma once

#include <string>
#include <map>
#include <gl/GLAux.h>

using namespace std;

enum TexMgrErrCode
{
	INVALD_TEXTURE=0
};

typedef unsigned int uint;

/**
	@class MyTexMgr
	@brief Textureπ‹¿Ì
	
*/
class MyTexMgr
{
public:
	MyTexMgr(void);
	virtual ~MyTexMgr(void);

public:
	bool	loadTex(const wstring& texName,const wstring& fileName);
	uint	getTex( const wstring& texName);
	bool	delTex( const wstring& texName);

public:
	bool	clear();

protected:
	AUX_RGBImageRec*	loadBMP(const wchar_t* fileName);
	map<wstring,uint>	m_textures;
};
