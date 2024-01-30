#include "..\Include\stdafx.h"
#include "..\Header\Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "player.h"
#include "Monster.h"
#include "Beefalo.h"
#include "Spider.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "CUI.h"
#include"CInven.h"

#include "Transform.h"
//Scene
#include "Layer.h"

// Monster/Resource/Object
#include "CObjectRock.h"
#include "CObjectGrass.h"
#include "CObjectTree.h"

// Item/Object
#include "CItem.h"
#include "CItemFood.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev,wstring _strSceneName)
	: Engine::CScene(pGraphicDev, _strSceneName)
{
	//test
}

CStage::~CStage()
{

}

HRESULT CStage::Ready_Scene()
{

	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(),E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Load_Data(), E_FAIL);

	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene()
{
	// DEBUG
}

HRESULT CStage::Ready_Layer_Environment()
{
	Engine::CGameObject* pGameObject = nullptr;

	m_pCamera = nullptr;
	pGameObject = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -5.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);

	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::BACK_GROUND]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pGameObject), E_FAIL);

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::BACK_GROUND]->AddGameObject(eOBJECT_GROUPTYPE::BACK_GROUND, pGameObject), E_FAIL);




	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);

	dynamic_cast<CDynamicCamera*>(m_pCamera)->SetTarget(pGameObject);

	for (_int i = 0; i < 5; ++i)
	{
		pGameObject = CBeefalo::Create(m_pGraphicDev, _vec3(_float(rand() % 30), 1.5f, _float(rand() % 30)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	}
	for (_int i = 0; i < 5; ++i)
	{
		pGameObject = CSpider::Create(m_pGraphicDev, _vec3(_float(rand() % 30), 1.5f, _float(rand() % 30)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	}

	//for (_int i = 0; i < 10; ++i)
	//{
	//	pGameObject = CObjectGrass::Create(m_pGraphicDev, _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(m_vecLayer[(int)eLAYER_TYPE::OBJECTS]->AddGameObject(L"OBJ_GRASS", pGameObject), E_FAIL);
	//}
	//for (_int i = 0; i < 10; ++i)
	//{
	//	pGameObject = CObjectRock::Create(m_pGraphicDev, _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(m_vecLayer[(int)eLAYER_TYPE::OBJECTS]->AddGameObject(L"OBJ_ROCK", pGameObject), E_FAIL);
	//}

	//for (_int i = 0; i < 10; ++i)
	//{
	//	pGameObject = CObjectTree::Create(m_pGraphicDev, _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(m_vecLayer[(int)eLAYER_TYPE::OBJECTS]->AddGameObject(L"OBJ_TREE", pGameObject), E_FAIL);
	//}
	for (_int i = 0; i < 2; ++i)
	{
		pGameObject = CItem::Create(m_pGraphicDev, L"Meat_Monster", _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);

		pGameObject = CItem::Create(m_pGraphicDev, L"Rocks_0", _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);

		pGameObject = CItem::Create(m_pGraphicDev, L"CutGlass", _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);

		pGameObject = CItem::Create(m_pGraphicDev, L"Berries", _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);

		pGameObject = CItem::Create(m_pGraphicDev, L"Log", _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}




	return S_OK;
}

HRESULT CStage::Ready_Layer_UI()
{

	auto& uiLayer = m_arrLayer[(int)eLAYER_TYPE::FORE_GROUND];

	NULL_CHECK_RETURN(uiLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	//¿ÞÂÊ ÆÇ³Ú
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(30.f, 300.f, 0.f), _vec3(30.f, 200.f, 0.f), L"Proto_UI_Left_Panel");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	//¾Æ·¡ ÆÇ³Ú
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(400.f, 580.f, 0.f), _vec3(20.f, 300.f, 0.f), L"Proto_UI_Left_Panel", 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);


	//ÀÎº¥Åä¸®
	pGameObject = CInven::Create(m_pGraphicDev, UI_STATE::UI_STATIC);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);


	//Àåºñ ½½·Ô
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 100.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Equipment");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//¿Ê ½½·Ô
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 120.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Cloth");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//»ýÁ¸ ½½·Ô
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 140.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Alive");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->Add_GameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);




	return S_OK;
}


HRESULT CStage::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Load_Data()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/mainMap.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_vec3 vPos{};
	_int iCount(0);
	DWORD	dwByte(0), dwStrByte(0);

	for (auto& iter : m_mapLayer)
	{
		if (iter.first != L"GameLogic")
			continue;
		
		//object delete

	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		TCHAR* pName = new TCHAR[dwStrByte];

		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
		ReadFile(hFile, &vPos.x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &vPos.y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &vPos.z, sizeof(_float), &dwByte, nullptr);
		dwStrByte = 0;

		NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;

		if (!_tcscmp(L"Tree", pName))
		{
			/*pGameObject = CObjectGrass::Create(m_pGraphicDev, vPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_vecLayer[(int)eLAYER_TYPE::OBJECTS]->AddGameObject(L"Tree", pGameObject), E_FAIL);*/
		}
		else if (!_tcscmp(L"Rock", pName))
		{
			pGameObject = CObjectRock::Create(m_pGraphicDev );
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		}
		else if (!_tcscmp(L"Grass", pName))
		{
			pGameObject = CObjectGrass::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_PLAY]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		}

		if (nullptr != pGameObject)
		{
			pGameObject->GetTransForm()->Set_Pos(vPos);
		}
		delete[] pName;
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Terrain Load", L"¼º°ø", MB_OK);

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring _strSceneName)
{
	CStage* pInstance = new CStage(pGraphicDev, _strSceneName);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStage::Free()
{

	__super::Free();
}
