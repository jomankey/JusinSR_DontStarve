#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CResMgr : public CBase
{
	DECLARE_SINGLETON(CResMgr)

private:
	explicit CResMgr();
	virtual ~CResMgr();
public:
	HRESULT Ready_Resource();
	void Save_TexturePath(wstring _key, TEXTUREID _textureTag, wstring  _subPath, int num);

private:
	virtual void		Free();
};

END