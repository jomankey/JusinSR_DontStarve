#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Slot.h"
#include <SlideBox.h>

class CSlotMgr :
    public CBase
{
    DECLARE_SINGLETON(CSlotMgr)

private:
    explicit CSlotMgr();
    virtual ~CSlotMgr();
    
public:
    void Set_Create_Menu(int _iNum, CItem* pItem);
    void Move_InvenItem(CItem* pItem, _uint _iItemNum, _uint _iMoveNum) // 인벤토리 아이템 이동 
    {
        m_pItemArr[_iMoveNum] = pItem;
        m_pItemArr[_iItemNum] = nullptr;
    }
    void Set_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType, CItem* pItem, _uint _iItemNum);

    void Get_BoxPos(BOX_TYPE eType, int _iNum, _vec3* vPos) {
        vector< CSlot*> pBox = m_mapBox.find(eType)->second;
        vPos->x = pBox[_iNum]->Get_fX();
        vPos->y = pBox[_iNum]->Get_fY();
    }

    vector<CSlot*> Get_BoxList(BOX_TYPE eType) { return m_mapBox.find(eType)->second; }
    CItem* Get_InvenItem(_uint iNum) { return m_pItemArr[iNum]; }

    // 아이템 장착 확인
    // 아이템 장착을 안하고 있을 시 L"" 값이 넘어간다.
    wstring Get_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType) { return m_pArmorArr[eArmorSlotType - 15] != nullptr ? m_pArmorArr[eArmorSlotType - 15]->GetObjName() : L""; }

    void Change_ArmorItem(CItem* pItem, ARMOR_SLOT_TYPE eArmorSlotType, _uint _iItemNum);

public:
    HRESULT Add_InvenBoxList(LPDIRECT3DDEVICE9 pGraphicDev, BOX_TYPE eType, BOX_DIR eDir ,int _iNum);
    void Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType);
    void LateUpdate_InvenBoxMgr(BOX_TYPE eType);
    void Render_InvenBoxMgr(BOX_TYPE eType);
    
    _bool	AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey);	//반환값이False면 아이템습득실패(아이템꽉참)

private:
    virtual void Free();

private:
    map <BOX_TYPE, vector<CSlot*>> m_mapBox;
    CItem* m_pItemArr[INVENCNT]; // 인벤토리

    CItem* m_pArmorArr[3]; // 장착 아이템 

};

