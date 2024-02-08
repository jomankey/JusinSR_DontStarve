#include "stdafx.h"
#include "UIMgr.h"
#include"CItem.h"
#include "SlotMgr.h"
#include <ItemTool.h>

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
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
	//m_pItemArr[0].
	//__super::Free();
}