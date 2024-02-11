#pragma once
#include "Export_Utility.h"
class CCook :
    public CGameObject
{
private:
    explicit CCook(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCook();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

public:
	void IsShow(_bool bShow) { m_bShow = bShow; }

private:
	HRESULT Add_Component();

private:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

public:
	static CCook* Create(LPDIRECT3DDEVICE9	pGraphicDev);

public:
	virtual void Free() override;

private:
	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;

	_bool m_bShow;

	CButtonUI* pButton;

};

