#pragma once
#include "Export_Utility.h"
#include <SlideUI.h>
class CCreateUI :
    public CGameObject
{
private:
    explicit CCreateUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCreateUI();

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject() override;

private:
    HRESULT			Add_Component();

    void Picking_Mouse();

private:
    Engine::CTexture* m_pTextureCom;
    Engine::CRcTex* m_pBufferCom;

public:
    static CCreateUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

private:
    vector<CSlideUI*> m_vecSlide;
    _matrix				m_ViewMatrix, m_ProjMatrix;
    _float m_fX, m_fY, m_fSizeX, m_fSizeY;

    LPDIRECT3DDEVICE9 m_pGraphicDev;
};

