#include "InvenBoxMgr.h"
#include "Engine_Enum.h"
#include <ItemTool.h>

IMPLEMENT_SINGLETON(CInvenBoxMgr)

CInvenBoxMgr::CInvenBoxMgr()
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pItemArr[i] = nullptr;
}

CInvenBoxMgr::~CInvenBoxMgr()
{
	Free();
}

//아이템 습득실패시 false반환
_bool CInvenBoxMgr::AddItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring strItemKey)
{
	vector<CInvenBox*> vecBox = Get_BoxList(INVEN);

	for (int i = 0; i < vecBox.size(); ++i) // 인벤토리 빈칸 있는지 없는지 체크
	{
		if (m_pItemArr[i] == nullptr) // 아이템이 없다면 인벤에 아이템 생성
		{
			_vec3 vPos;
			vPos.x = vecBox[i]->Get_fX();
			vPos.y = vecBox[i]->Get_fY();
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
			CItem* pItem = CItemTool::Create(pGraphicDev, strItemKey, vPos, UI_ITEM_INVEN, bFood);
			dynamic_cast<CItemTool*>(pItem)->Set_BoxIndex(i);
			m_pItemArr[i] = pItem;
			return true;
		}

		if (m_pItemArr[i]->GetObjName() == strItemKey) // 같은 아이템이 있다면 아이템 카운트 증가
		{
			m_pItemArr[i]->AddItemCount(1);
			return true;
		}
	}

	return false;
}

CItem* CInvenBoxMgr::GetItemBox(_uint _uint)

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

void CInvenBoxMgr::Set_Create_Menu(int _iNum, CItem* pItem)
{
	if (3 < _iNum || pItem == nullptr)
		return;

	vector<CInvenBox*> pCreateBox = m_mapBox.find(CREATE)->second;

	pCreateBox[_iNum]->Set_Item(pItem);
}

HRESULT CInvenBoxMgr::Add_InvenBoxList(LPDIRECT3DDEVICE9 pGraphicDev, BOX_TYPE eType, BOX_DIR eDir, int _iNum)
{
	if (eType > BOX_END || eDir > BOX_DIR_END || 0 > _iNum)
		return E_FAIL;

	vector<CInvenBox*> pInvenBox;

	if (eDir == WIDTH) // 세로 방향
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenBox::Create(pGraphicDev, _vec3(120.f + (15.f + 20.f) * i, 582.f, 0.f), i, eType));
	}
	if (eDir == HEIGHT) // 가로 방향
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenBox::Create(pGraphicDev, _vec3(25.f, 140.f + 40.f*i, 0.f), i, eType));
	}
	m_mapBox.emplace(eType, pInvenBox);
	return S_OK;
}

void CInvenBoxMgr::Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType)
{
	vector<CInvenBox*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Update_GameObject(fTimeDelta);

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if(inven) inven->Update_GameObject(fTimeDelta);
	}
}

void CInvenBoxMgr::LateUpdate_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CInvenBox*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->LateUpdate_GameObject();

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if (inven) inven->LateUpdate_GameObject();
	}
}

void CInvenBoxMgr::Render_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CInvenBox*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Render_GameObject();

	if (eType == INVEN)
	{
		for (auto& inven : m_pItemArr)
			if (inven) inven->Render_GameObject();
	}
}

void CInvenBoxMgr::Free()
{
	for (auto& vecBox : m_mapBox)
		for (auto& iter : vecBox.second)
			Safe_Release(iter);

	for (auto& iter : m_pItemArr)
		Safe_Release(iter);
}
