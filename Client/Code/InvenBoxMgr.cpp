#include "InvenBoxMgr.h"
#include "Engine_Enum.h"
#include <ItemTool.h>

IMPLEMENT_SINGLETON(CInvenBoxMgr)

CInvenBoxMgr::CInvenBoxMgr()
{
}

CInvenBoxMgr::~CInvenBoxMgr()
{
	Free();
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
			pInvenBox.push_back(CInvenBox::Create(pGraphicDev, _vec3(120.f + (15.f + 20.f) * i, 582.f, 0.f), CREATE));
	}
	if (eDir == HEIGHT) // 가로 방향
	{
		for (int i = 0; i < _iNum; ++i)
			pInvenBox.push_back(CInvenBox::Create(pGraphicDev, _vec3(25.f, 140.f + 40.f*i, 0.f)));
	}
	m_mapBox.emplace(eType, pInvenBox);
	return S_OK;
}

void CInvenBoxMgr::Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType)
{
	vector<CInvenBox*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Update_GameObject(fTimeDelta);
}

void CInvenBoxMgr::LateUpdate_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CInvenBox*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->LateUpdate_GameObject();
}

void CInvenBoxMgr::Render_InvenBoxMgr(BOX_TYPE eType)
{
	vector<CInvenBox*> vecBox = m_mapBox.find(eType)->second;

	for (auto& iter : vecBox)
		iter->Render_GameObject();
}

void CInvenBoxMgr::Free()
{
	for (auto& vecBox : m_mapBox)
		for (auto& iter : vecBox.second)
			Safe_Release(iter);
}
