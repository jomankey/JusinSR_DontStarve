#include "stdafx.h"
#include "CRoadScene.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "CInven.h"
#include "CPigHouse.h"
#include "Pig.h"
#include "BerryBush.h"

//Scene
#include "Layer.h"

// Monster/Resource/Object
#include "CObjectRock.h"
#include "CObjectGrass.h"
#include "CObjectTree.h"

//Component
#include "Transform.h"

// Object
// Player/Monster
#include "player.h"

#include "Monster.h"
#include "Beefalo.h"
#include "Spider.h"
#include "DeerClops.h"

//Environment

#include "RunTerrain.h"
//

//Resource
#include "CItem.h"
#include "CItemFood.h"
#include <BerryBush.h>


//UI
#include "CUI.h"
#include"CInvenBox.h"
#include"CEquiment.h"
#include"CSlideUI.h"
#include"CAliveUI.h"
#include"CHpUI.h"
#include"CMentalUI.h"
#include"CWorldUI.h"
#include"CHungryUI.h"
#include"CExplainPanel.h"

CRoadScene::CRoadScene(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
	: Engine::CScene(pGraphicDev, _strSceneName)
{
	//test
}

CRoadScene::~CRoadScene()
{

}

HRESULT CRoadScene::Ready_Scene()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	//FAILED_CHECK_RETURN(Load_Data(), E_FAIL);


	return S_OK;
}

Engine::_int CRoadScene::Update_Scene(const _float& fTimeDelta)
{
	Change_LightInfo(fTimeDelta);



	return __super::Update_Scene(fTimeDelta);
}

void CRoadScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CRoadScene::Render_Scene()
{
}

HRESULT CRoadScene::Ready_Layer_Environment()
{
	Engine::CGameObject* pGameObject = nullptr;


	//////카메라
	m_pCamera = nullptr;
	pGameObject = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(64.f, 3.f, 64.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pGameObject), E_FAIL);

	//////////스카이박스
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::BACK_GROUND, pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CRoadScene::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CRunTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::PLAYER, pGameObject), E_FAIL);
	m_pPlayer->GetTransForm()->Set_Pos(_vec3(2.5f, 2.f, 3.5f));

	dynamic_cast<CDynamicCamera*>(m_pCamera)->SetTarget(pGameObject);



	return S_OK;
}

HRESULT CRoadScene::Ready_Layer_UI()
{

	auto& uiLayer = m_arrLayer[(int)eLAYER_TYPE::FORE_GROUND];

	NULL_CHECK_RETURN(uiLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	//Before UI struct 
	//-------------------------------------------
	//왼쪽 판넬
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(30.f, 300.f, 0.f), _vec3(30.f, 200.f, 0.f), L"Proto_UI_Left_Panel");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//아래 판넬
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(420.f, 580.f, 0.f), _vec3(20.f, 340.f, 0.f), L"Proto_UI_Left_Panel", 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//설명 판넬
	pGameObject = CExplainPanel::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(400.f, 300.f, 0.f), _vec3(80.f, 80.f, 0.f), L"Proto_UI_Explain_PopUp_Panel");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	////인벤토리
	//pGameObject = CInven::Create(m_pGraphicDev, UI_STATE::UI_STATIC);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);


	//인벤토리 슬롯 이미지
	for (int i = 0; i < 15; i++)
	{
		int PixelJump = 0;
		if (i == 5 || i == 10 || i == 15)
			PixelJump = 7;

		pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(130.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);
	}

	//인벤토리에 들어갈 아이템 이미지
	for (int i = 0; i < 15; i++)
	{
		int PixelJump = 0;
		if (i == 5 || i == 10 || i == 15)
			PixelJump = 7;

		pGameObject = CInvenBox::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(150.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot", i);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);
	}

	//HpUI
	pGameObject = CHpUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(680.f, 110.f, 0.f), _vec3(30.f, 30.f, 0.f), L"Proto_UI_HP");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//Mental
	pGameObject = CMentalUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(770.f, 110.f, 0.f), _vec3(30.f, 30.f, 0.f), L"Proto_UI_Mental");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);
	//Hunger
	pGameObject = CHungryUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(730.f, 130.f, 0.f), _vec3(30.f, 30.f, 0.f), L"Proto_UI_Hungry");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);


	//WorldUI
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(730.f, 60.f, 0.f), _vec3(40.f, 40.f, 0.f), L"Proto_UI_World_1");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//WorldUI
	pGameObject = CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(730.f, 60.f, 0.f), _vec3(40.f, 40.f, 0.f), L"Proto_UI_World_2");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	//WorldUIClock
	pGameObject = CWorldUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(730.f, 60.f, 0.f), _vec3(10.f, 30.f, 0.f), L"Proto_UI_World_10");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(uiLayer->AddGameObject(eOBJECT_GROUPTYPE::UI, pGameObject), E_FAIL);

	return S_OK;
}


HRESULT CRoadScene::Ready_LightInfo()
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

HRESULT CRoadScene::Load_Data()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/RunStage.dat",
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

	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		TCHAR* pName = new TCHAR[dwStrByte];

		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		dwStrByte = 0;

		Create_Object(pName, vPos);

		delete[] pName;
	}


	for (int i = 0; i < 3; ++i)
	{
		ReadFile(hFile, &m_vDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &m_vDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &m_vDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &m_vDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);
	vector<_int> m_vecPos;
	for (int i = 0; i < iCount; ++i)
	{
		int iTemp = 0;
		ReadFile(hFile, &iTemp, sizeof(_int), &dwByte, nullptr);
		m_vecPos.push_back(iTemp);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Terrain Load", L"성공", MB_OK);

	return S_OK;
}

HRESULT CRoadScene::Create_Object(const _tchar* pName, _vec3 vPos)
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	if (!_tcscmp(L"Tree", pName))
	{
		pGameObject = CObjectTree::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
		vPos.y = 2.3f;
	}
	else if (!_tcscmp(L"Rock", pName))
	{
		pGameObject = CObjectRock::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Grass", pName))
	{
		pGameObject = CObjectGrass::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"PigHouse", pName))
	{
		pGameObject = CPigHouse::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"BerryBush", pName))
	{
		pGameObject = CBerryBush::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::RESOURCE_OBJECT, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"CutGlass", pName))
	{
		pGameObject = CItem::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rocks_0", pName))
	{
		pGameObject = CItem::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rocks_1", pName))
	{
		pGameObject = CItem::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Twigs", pName))
	{
		pGameObject = CItem::Create(m_pGraphicDev, pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::ITEM, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Beefalo", pName))
	{
		pGameObject = CBeefalo::Create(m_pGraphicDev, _vec3(_float(rand() % 30), 1.5f, _float(rand() % 30)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Spider", pName))
	{
		pGameObject = CSpider::Create(m_pGraphicDev, _vec3(_float(rand() % 30), 1.5f, _float(rand() % 30)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Pig", pName))
	{
		pGameObject = CPig::Create(m_pGraphicDev, _vec3(_float(rand() % 30), 1.5f, _float(rand() % 30)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::MONSTER, pGameObject), E_FAIL);
	}

	if (nullptr != pGameObject)
		pGameObject->GetTransForm()->Set_Pos(vPos);
}

HRESULT CRoadScene::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9* tLightInfo = light::Get_Light(0)->Get_Light();

	static _vec3 vLight[3] = {
		{m_vDirectionDiffuseColor[0].x, m_vDirectionDiffuseColor[0].y, m_vDirectionDiffuseColor[0].z },
		{m_vDirectionAmbientColor[0].x, m_vDirectionAmbientColor[0].y, m_vDirectionAmbientColor[0].z },
		{m_vDirectionSpecularColor[0].x, m_vDirectionSpecularColor[0].y, m_vDirectionSpecularColor[0].z } };

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	int iIndex(0);

	iIndex = light::Change_Light(fTimeDelta, 0);

	vLight[0].x += (m_vDirectionDiffuseColor[iIndex].x - vLight[0].x) * fTimeDelta;
	vLight[0].y += (m_vDirectionDiffuseColor[iIndex].y - vLight[0].y) * fTimeDelta;
	vLight[0].z += (m_vDirectionDiffuseColor[iIndex].z - vLight[0].z) * fTimeDelta;

	vLight[1].x += (m_vDirectionAmbientColor[iIndex].x - vLight[1].x) * fTimeDelta;
	vLight[1].y += (m_vDirectionAmbientColor[iIndex].y - vLight[1].y) * fTimeDelta;
	vLight[1].z += (m_vDirectionAmbientColor[iIndex].z - vLight[1].z) * fTimeDelta;

	vLight[2].x += (m_vDirectionSpecularColor[iIndex].x - vLight[2].x) * fTimeDelta;
	vLight[2].y += (m_vDirectionSpecularColor[iIndex].y - vLight[2].y) * fTimeDelta;
	vLight[2].z += (m_vDirectionSpecularColor[iIndex].z - vLight[2].z) * fTimeDelta;

	tLightInfo->Diffuse = D3DXCOLOR(vLight[0].x,
		vLight[0].y,
		vLight[0].z,
		1.f);
	tLightInfo->Specular = D3DXCOLOR(vLight[1].x,
		vLight[1].y,
		vLight[1].z,
		1.f);
	tLightInfo->Ambient = D3DXCOLOR(vLight[2].x,
		vLight[2].y,
		vLight[2].z,
		1.f);

	tLightInfo->Direction = _vec3(1.f, -1.f, 1.f);

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);
	light::Get_Light(0)->Update_Light();

	return S_OK;
}

CRoadScene* CRoadScene::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
{
	CRoadScene* pInstance = new CRoadScene(pGraphicDev, _strSceneName);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TestScene Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CRoadScene::Free()
{
	__super::Free();
}
