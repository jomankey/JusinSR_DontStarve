#pragma once
#include "CItem.h"
class CItemTool :
    public CItem
{
private:
    explicit CItemTool(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, _bool bFood);
    explicit CItemTool(const CItem& rhs);
    virtual ~CItemTool();

public:
    void Set_FoodItem(_bool _bFood) { m_bFood = _bFood; }
    
    _bool Get_FoodItem() { return m_bFood; }

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject() override;
    void Render_GameObject() override;

public:
    static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, _bool bFood = false);

private:
    virtual void Free();

private:
    _matrix				m_ViewMatrix, m_ProjMatrix;
    _float m_fX, m_fY, m_fSizeX, m_fSizeY;

    _bool m_bFood;
};

