#pragma once
#include "Export_Utility.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CCalculator;

END

class CToolTile :
    public CGameObject
{
private:
    explicit CToolTile(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolTile();

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()override;

private:
    HRESULT Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CTexture* m_pTextureCom;
    Engine::CCalculator* m_pCalculatorCom;

public:
    static CToolTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();
};

