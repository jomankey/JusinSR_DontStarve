#pragma once
#include "Export_Utility.h"

class CToolTile :
    public CGameObject
{
private:
    explicit CToolTile(LPDIRECT3DDEVICE9 pGraphicDev, _int iNum);
    virtual ~CToolTile();

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()override;

private:
    HRESULT Add_Component();

private:
    Engine::CCubeTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CTexture* m_pTextureCom;
    Engine::CCalculator* m_pCalculatorCom;

    _int m_iNum;

public:
    static CToolTile* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNum);

private:
    virtual void Free();
};
