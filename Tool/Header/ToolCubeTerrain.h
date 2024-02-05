#pragma once
#include "Export_Utility.h"
#include "ToolTile.h"

class CToolCubeTerrain :
    public CGameObject
{
private:
    explicit CToolCubeTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolCubeTerrain();

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()override;

private:
    HRESULT Add_Component();

    void Input_Mouse();

    HRESULT Picking_OnTerrain();

private:
    CToolTile* m_pCubeCom[129 * 5 * 4]; // 좌, 우, 위, 아래 4방향 큐브
    Engine::CCalculator* m_pCalculatorCom;

    LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
    static CToolCubeTerrain* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
    virtual void Free() override;

};

