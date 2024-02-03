#pragma once
#include "Export_Utility.h"
class CToolBerry :
    public CGameObject
{
private:
    explicit CToolBerry(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolBerry();

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

public:
	static CToolBerry* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

