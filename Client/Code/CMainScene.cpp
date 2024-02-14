#include "stdafx.h"
#include "CMainScene.h"

#include "Export_System.h"
#include "Export_Utility.h"


#include "CBossScene.h"
#include "CRoadScene.h"
#include "CTestScene.h"
#include "LoadingScene.h"
#include "CBGLoading.h"

#include "Stage.h"
#include "Layer.h"

//UI Object
#include "CGenerateMap.h"
#include "CGenerateHand.h"
#include "CLoadBackGround.h"

//Camera
#include "DynamicCamera.h"



CMainScene::CMainScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, L"LOGO")
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Ready_Scene()
{

	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
		m_arrLayer[i] = CLayer::Create();

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	return S_OK;
}

Engine::_int CMainScene::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);
	return iExit;
}

void CMainScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CMainScene::Render_Scene()
{
}

HRESULT CMainScene::Ready_Prototype()
{
	
	FAILED_CHECK_RETURN(Ready_LoadingObject(), E_FAIL);

	return S_OK;
}

HRESULT CMainScene::Ready_LoadingObject()
{
	Engine::CAnimation* pAnim = nullptr;
	CGameObject* pInstance = nullptr;


	pAnim = CAnimation::Create(m_pGraphicDev, L"BG_MAIN", L"../Bin/Resource/Texture/UI/BG_Loading/BG_Main.png", 1, 11.f);
	proto::Ready_ProtoAnim(L"BG_MAIN", pAnim);


	pInstance = CBGLoading::Create(m_pGraphicDev, L"BG_MAIN", _vec3(WINCX / 2.f, WINCY / 2.f, 0.f), _vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f));
	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);


	AddGameObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, pInstance);
	m_pCamera = nullptr;
	pInstance = m_pCamera = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(64.f, 3.f, 64.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);

	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pInstance), E_FAIL);


	return S_OK;
}

CMainScene* CMainScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainScene* pInstance = new CMainScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Main Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMainScene::Free()
{
	__super::Free();
}
