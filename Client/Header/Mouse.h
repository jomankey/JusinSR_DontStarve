#pragma once
#include "Export_Utility.h"
class CMouse :
    public CGameObject
{
private:
    explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMouse();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject() override;
	virtual void		Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

	_bool m_bColl;

	_vec3 m_vMousePos;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float m_fPreX, m_fPreY;
};

