#include "..\Include\stdafx.h"
#include "..\Header\Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"
#include "Beefalo.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "CUI.h"

// Monster/Resource/Object
#include "CObjectRock.h"
#include "CObjectGrass.h"
#include "CObjectTree.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	//test
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameRes(L"GameRes"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);


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

HRESULT CStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

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

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);



	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	dynamic_cast<CDynamicCamera*>(m_pCamera)->SetTarget(pGameObject);

	pGameObject = CBeefalo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Beefalo", pGameObject), E_FAIL);

	for (_int i = 0; i < 10; ++i)
	{
		pGameObject = CObjectGrass::Create(m_pGraphicDev, _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OBJ_GRASS", pGameObject), E_FAIL);
	}
	for (_int i = 0; i < 10; ++i)
	{
		pGameObject = CObjectRock::Create(m_pGraphicDev, _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OBJ_ROCK", pGameObject), E_FAIL);
	}

	for (_int i = 0; i < 10; ++i)
	{
		pGameObject = CObjectTree::Create(m_pGraphicDev, _vec3(_float(rand() % 20), 1.5f, _float(rand() % 20)));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OBJ_TREE", pGameObject), E_FAIL);
	}
	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
	//sss
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CUI::Create(m_pGraphicDev, CUI::UI_STATE::UI_STATIC, _vec3(40.f, 300.f, 0.f), _vec3(40.f, 200.f, 0.f), L"Proto_UI_Left_Panel");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, CUI::UI_STATE::UI_STATIC, _vec3(20.f, 300.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Item_Inven_Slot");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, CUI::UI_STATE::UI_STATIC, _vec3(20.f, 130.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Equipment");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, CUI::UI_STATE::UI_STATIC, _vec3(20.f, 200.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Cloth");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, CUI::UI_STATE::UI_STATIC, _vec3(20.f, 300.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Alive");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameRes(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;



	m_mapLayer.insert({ pLayerTag, pLayer });

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

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage* pInstance = new CStage(pGraphicDev);

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
