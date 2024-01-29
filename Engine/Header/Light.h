#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight :	public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight();

public:
	D3DLIGHT9* Get_Light() { return &m_tLightInfo; }
	void Update_Light() { m_pGraphicDev->SetLight(m_iIndex, &m_tLightInfo); }

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DLIGHT9					m_tLightInfo;
	_uint						m_iIndex;

public:
	HRESULT		Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const D3DLIGHT9* pLightInfo, 
								const _uint& iIndex);
private:
	virtual void		Free();

};

END