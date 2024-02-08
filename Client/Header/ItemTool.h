#pragma once
#include "CItem.h"
#include "Engine_Define.h"

class CItemTool : // UI 용 아이템 
    public CItem
{
private:
    explicit CItemTool(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, UI_ITEM_TYPE eType, _bool bFood);
    explicit CItemTool(const CItem& rhs);
    virtual ~CItemTool();

public:
    void Set_FoodItem(_bool _bFood) { m_bFood = _bFood; }
    void Set_BoxIndex(_uint iNum) { m_iNum = iNum; }

    _bool Get_FoodItem() { return m_bFood; }

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject() override;
    void Render_GameObject() override;

public:
    static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, UI_ITEM_TYPE eType = UI_ITEM_CREATE, _bool bFood = false);

private:
    virtual void Free();

private:
    _matrix				m_ViewMatrix, m_ProjMatrix;
    _float m_fX, m_fY, m_fSizeX, m_fSizeY;
    _float m_fPreX, m_fPreY;

    _bool m_bFood;

    UI_ITEM_TYPE m_eItemType;
    _uint m_iNum; // 박스 순서 넘버
};

