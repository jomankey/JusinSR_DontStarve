#pragma once
#include "Export_Utility.h"
class CToolMonster :
    public CGameObject
{
private:
	explicit CToolMonster(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName);
	virtual ~CToolMonster();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	wstring m_pName;

public:
	static CToolMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName);

private:
	virtual void Free();
};

