#pragma once
#include "Export_Utility.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;

END

class CToolTerrain :
    public CGameObject
{
private:
    explicit CToolTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CToolTerrain(const CToolTerrain& rhs);
    virtual ~CToolTerrain();

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()override;

private:
    HRESULT Add_Component();
    HRESULT SetUp_Material();

private:
    Engine::CTerrainTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CTexture* m_pTextureCom;

public:
    static CToolTerrain* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
    virtual void Free() override;
};

