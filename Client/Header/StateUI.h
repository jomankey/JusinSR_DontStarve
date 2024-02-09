#pragma once
#include "Export_Utility.h"
class CStateUI :
    public CGameObject
{
protected:
    explicit CStateUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CStateUI();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT Add_Component() PURE;
	virtual void Check_State() PURE;

protected:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

protected:
	virtual void Free();

protected:
	_float m_frame; //출력될 사진 인덱스
	_float m_frameEnd; //프레임이 시작되고나서 움직이는 값
	_float m_PreFrame;
	BOOL m_bFrameStop = true;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;
};

