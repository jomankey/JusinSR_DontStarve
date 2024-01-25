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

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;

public:
	virtual CComponent*	Clone();
	// iNum�� �߰� 
<<<<<<< HEAD
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1, const _uint& iNum = 0);
=======
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1, const _uint& iNum =0);
>>>>>>> bb1bf657de05d89d12a7a5547e0ecf374fc5ee4c

public:
	virtual void		Free();

};

END