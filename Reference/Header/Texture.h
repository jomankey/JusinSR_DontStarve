#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT		Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt);
	void		Set_Texture(const _uint& iIndex = 0);
	D3DSURFACE_DESC Get_TexInfo() { return m_TexInfo; }


public:
	virtual CComponent*	Clone();
	// iNum°ª Ãß°¡ 
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1, const _uint& iNum =0);

public:
	virtual void		Free();
private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	D3DSURFACE_DESC	m_TexInfo;
};

END