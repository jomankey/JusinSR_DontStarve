#include "SlotMgr.h"
#include "Engine_Enum.h"
#include "ArmorSlot.h"
#include <ItemTool.h>
#include "InvenSlot.h"

IMPLEMENT_SINGLETON(CSlotMgr)

CSlotMgr::CSlotMgr()
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pItemArr[i] = nullptr;

	for (int i = 0; i < 3; ++i)
		m_pArmorArr[i] = nullptr;
}

CSlotMgr::~CSlotMgr()
{
	Free();
}

//아이템 습득실패시 false반환
_bool CSlotMgr::AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey, _vec3* vSlotPos)
{
	vector<CSlot*> vecBox = Get_BoxList(INVEN);

	for (int i = 0; i < INVENCNT; ++i) // 인벤토리 빈칸 있는지 없는지 체크
	{
		_vec3 vPos;
		vPos.x = vecBox[i]->Get_fX();
		vPos.y = vecBox[i]->Get_fY();

		if (m_pItemArr[i] == nullptr) // 아이템이 없다면 인벤에 아이템 생성
		{
			/*
			L"Log"
			L"Berries"
			L"Cooked_berries"
			L"Cooked_Meat_Monster"
			L"CookedMeat"
			L"CutGlass"
			L"CutStone"
			L"Meat_Monster"
			L"RawMeat"
			L"Rocks_0"
			L"Rocks_1"
			L"Silk"
			L"Twigs"
			L"Ax"
			L"Lance"
			L"Hammer"
			L"FireSton"
			L"Pickaxe"
			L"Shovel"
			L"Torch"
			L"BonFire"
			*/
			_bool bFood = strItemKey == L"Cooked_berries" || strItemKey == L"Cooked_Meat_Monster" || strItemKey == L"CookedMeat" || strItemKey == L"Meat_Monster" || strItemKey == L"RawMeat";

			ARMOR_SLOT_TYPE eArmor(ARMOR_SLOT_END);
			if (strItemKey == L"Ax" || strItemKey == L"Lance" || strItemKey == L"Hammer" || strItemKey == L"Pickaxe" || strItemKey == L"Torch")
				eArmor = HAND;

			CItem* pItem = CItemTool::Create(pGraphicDev, strItemKey, vPos, UI_ITEM_INVEN, bFood);
			if (eArmor != ARMOR_SLOT_END) dynamic_cast<CItemTool*>(pItem)->Set_ArmorSlotType(eArmor);
			dynamic_cast<CItemTool*>(pItem)->Set_BoxIndex(i);
			m_pItemArr[i] = pItem;
			(*vSlotPos) = vPos;
			return true;
		}

		if (m_pItemArr[i]->GetObjName() == strItemKey) // 같은 아이템이 있다면 아이템 카운트 증가
		{
			m_pItemArr[i]->AddItemCount(1);
			(*vSlotPos) = vPos;
			return true;
		}
	}

	return false;
}

void CSlotMgr::Set_Create_Menu(int _iNum, CItem* pItem)
{
	if (3 < _iNum || pItem == nullptr)
		return;

	vector<CSlot*> pCreateBox = m_mapBox.find(CREATE)->second;

	pCreateBox[_iNum]->Set_Item(pItem);
}

void CSlotMgr::Remove_InvenItem(_uint _iItemNum)
{
	CItem* pItme = m_pItemArr[_iItemNum];

	pItme->MinusItemCount(1);

	if (pItme->Get_ItemCount() <= 0)
	{
		Safe_Release(pItme);
		m_pItemArr[_iItemNum] = nullptr;
	}	
}

void CSlotMgr::Set_ArmorItem(ARMOR_SLOT_TYPE eArmorSlotType, CItem* pItem, _uint _iItemNum)
{
	// 아이템 장착 시 인벤 삭제
	m_pArmorArr[eArmorSlotType - 15] = pItem;
	m_pItemArr[_iItemNum] = nullptr;
}

void CSlotMgr::Change_ArmorItem(CItem* pItem, ARMOR_SLOT_TYPE eArmorSlotType, _uint _iItemNum)
{

	CItem* pMoveItem = m_pArmorArr[eArmorSlotType];

	m_pArmorArr[eArmorSlotType] = pItem;
	m_pItemArr[_iItemNum] = pMoveItem;
	dynamic_cast<CItemTool*>(pMoveItem)->Set_BoxIndex(_iItemNum);
}

HRESULT CSlotMgr::Add_InvenBoxList(LPDIRECT3DDEVICE9 pGraphicDev, BOX_TYPE eType, BOX_DIR eDir, int _iNum)
{
	if (eType > BOX_END || eDir > BOX_DIR_END || 0 > _iNum)
		return E_FAIL;

	vector<CSlot*> pInvenBox;

	if (eDir == WIDTH) // 세로 방향 (인벤)
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenSlot::Create(pGraphicDev, _vec3(120.f + (15.f + 20.f) * i, 582.f, 0.f), i, eType));
		for (int i = INVENCNT; i < _iNum +3; ++i)
			pInvenBox.push_back(CArmorSlot::Create(pGraphicDev, _vec3(120.f + (15.f + 20.f) * i, 582.f, 0.f), i, (ARMOR_SLOT_TYPE)i));
	}
	if (eDir == HEIGHT) // 가로 방향
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenSlot::Create(pGraphicDev, _vec3(25.f, 140.f + 40.f*i, 0.f), i, eType));
	}
	m_mapBox.emplace(eType, pInvenBox);
	return S_OK;
}

void CSlotMgr::Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType)
{
	vector<CSlot*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Update_GameObject(fTimeDelta);

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if(inven) inven->Update_GameObject(fTimeDelta);
		for (auto& iter : m_pArmorArr)
			if (iter) iter->Update_GameObject(fTimeDelta);
	}
}

void CSlotMgr::LateUpdate_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CSlot*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->LateUpdate_GameObject();

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if (inven) inven->LateUpdate_GameObject();
		for (auto& iter : m_pArmorArr)
			if (iter) iter->LateUpdate_GameObject();
	}
}

void CSlotMgr::Render_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CSlot*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Render_GameObject();

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if (inven) inven->Render_GameObject();
		for (auto& iter : m_pArmorArr)
			if (iter) iter->Render_GameObject();
	}
}

void CSlotMgr::Free()
{
	for (auto& vecBox : m_mapBox)
		for (auto& iter : vecBox.second)
			Safe_Release(iter);

	for (auto& iter : m_pItemArr)
		Safe_Release(iter);

	for (auto& iter : m_pArmorArr)
		Safe_Release(iter);
}
