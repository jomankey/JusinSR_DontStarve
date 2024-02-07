#include "stdafx.h"
#include "UIMgr.h"
#include"CItem.h"
#include "InvenBoxMgr.h"
#include <ItemTool.h>

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
	for (int i = 0; i < 15; i++)
	{
		m_pItemArr[i] = nullptr;
	}
}

CUIMgr::~CUIMgr()
{
	Free();
}

CREATEINFO CUIMgr::Get_CreateInfo(wstring pKey)
{
	for (auto& iter : m_vecCreateInfo)
	{
		if (iter.strKeyName == pKey)
			return iter;
	}

	return CREATEINFO();
}

//아이템 습득실패시 false반환
_bool CUIMgr::AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey)
{
	//int i = 0;

	//for (auto& itemBox : m_pItemArr)
	//{
	//	if (nullptr == itemBox && _pItem->IsEquipment())// 아이템칸이 비었고 장비아이템을 습득했을경우
	//	{
	//		itemBox = _pItem;
	//		return true;
	//	}

	//	if (nullptr != itemBox && (!_pItem->IsEquipment()))//아이템이있고 소모품을습득했을경우
	//	{
	//		if (itemBox->GetObjName()== _pItem->GetObjName())//해당칸의 이름이 같은 아이템이있을경우
	//		{
	//			itemBox->AddItemCount(_pItem->GetItemInfo().ItemCount);
	//			return true;
	//		}
	//	}
	//}

	//for (auto& itemBox : m_pItemArr)
	//{
	//	if (itemBox == nullptr)
	//	{
	//		itemBox = _pItem;
	//		return true;
	//	}
	//}
	vector<CInvenBox*> vecBox = CInvenBoxMgr::GetInstance()->Get_BoxList(INVEN);

	for (auto& box : vecBox) // 인벤토리 빈칸 있는지 없는지 체크
	{
		if (box->Get_Item() == nullptr) // 아이템이 없다면 인벤에 아이템 생성
		{
			_vec3 vPos;
			vPos.x = box->Get_fX();
			vPos.y = box->Get_fY();

			CItem* pItem = CItemTool::Create(pGraphicDev, strItemKey, vPos, UI_ITEM_INVEN);
			box->Set_Item(pItem);
			return true;
		}

		if (box->Get_Item()->GetObjName() == strItemKey) // 같은 아이템이 있다면 아이템 카운트 증가
		{
			box->Get_Item()->AddItemCount(1);
			return true;
		}
	}

	return false;
}

CItem* CUIMgr::GetItemBox(_uint _uint)

{
	if (nullptr != m_pItemArr[_uint])
	{
		return m_pItemArr[_uint];
	}
	else
	{
		return nullptr;
	}
}

void CUIMgr::Ready_CreateInfo()
{
	CREATEINFO tInfo;

	ZeroMemory(&tInfo, sizeof(tInfo));

	// 생활용품
	tInfo.strKeyName = L"Ax";
	tInfo.strName = L"도끼";
	tInfo.strInfo = L"나무를 밸 수 있다.";
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 1;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 5;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Pickaxe";
	tInfo.strName = L"곡괭이";
	tInfo.strInfo = L"바위를 캘 수 있다.";
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 1;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 5;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"Hammer";
	tInfo.strName = L"망치";
	tInfo.strInfo = L"무엇이든 부술 수 있다.";
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 1;
	tInfo.tItemInfo[1].strItemName = L"Rocks_0";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	//생존 용품
	tInfo.strKeyName = L"Torch";
	tInfo.strName = L"횃불";
	tInfo.strInfo = L"들고있으면 주변을 밝혀준다.";
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"CutGlass";
	tInfo.tItemInfo[1].iCount = 2;
	m_vecCreateInfo.push_back(tInfo);

	tInfo.strKeyName = L"BonFire";
	tInfo.strName = L"모닥불";
	tInfo.strInfo = L"설치하면 주변을 환하게 밝혀준다.";
	tInfo.tItemInfo[0].strItemName = L"Log";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"CutGlass";
	tInfo.tItemInfo[1].iCount = 3;
	m_vecCreateInfo.push_back(tInfo);

	//전투용품
	tInfo.strKeyName = L"Lance";
	tInfo.strName = L"전투창";
	tInfo.strInfo = L"들고 공격한다면 몸을 지킬 수 있다.";
	tInfo.tItemInfo[0].strItemName = L"Twigs";
	tInfo.tItemInfo[0].iCount = 2;
	tInfo.tItemInfo[1].strItemName = L"FireSton";
	tInfo.tItemInfo[1].iCount = 1;
	m_vecCreateInfo.push_back(tInfo);
}

void CUIMgr::Free()
{
	for (auto iter : m_pItemArr)
	{
		
	}
	//m_pItemArr[0].
	//__super::Free();
}