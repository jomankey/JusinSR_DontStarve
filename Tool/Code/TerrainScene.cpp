#include "TerrainScene.h"
#include "Export_Utility.h"
#include "ToolTerrain.h"
#include "ToolCamera.h"
#include "ToolMgr.h"

CTerrainScene::CTerrainScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

CTerrainScene::~CTerrainScene()
{
}

HRESULT CTerrainScene::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain0.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile_Grass_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass1/grass_%d.png", 80)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile_Grass_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass2/grass2_%d.png", 80)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Tile/tile000.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	return S_OK;
}

_int CTerrainScene::Update_Scene(const _float& fTimeDelta)
{
	Ready_LightInfo();
	return __super::Update_Scene(fTimeDelta);
}

void CTerrainScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CTerrainScene::Render_Scene()
{
	//
}

HRESULT CTerrainScene::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	m_pCamera = nullptr;
	_vec3 vEye = { 0.f, 10.f, -5.f };
	_vec3 vAt = { 0.f, 0.f, 1.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };
	pGameObject = m_pCamera = CToolCamera::Create(m_pGraphicDev,
		&vEye,
		&vAt,
		&vUp,
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CTerrainScene::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CToolTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CTerrainScene::Ready_Layer_UI(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CTerrainScene::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(CToolMgr::m_fDirectionDiffuseColor[0], CToolMgr::m_fDirectionDiffuseColor[1], CToolMgr::m_fDirectionDiffuseColor[2], 1.f);
	tLightInfo.Specular = D3DXCOLOR(CToolMgr::m_fDirectionAmbientColor[0], CToolMgr::m_fDirectionAmbientColor[1], CToolMgr::m_fDirectionAmbientColor[2], 1.f);
	tLightInfo.Ambient = D3DXCOLOR(CToolMgr::m_fDirectionSpecularColor[0], CToolMgr::m_fDirectionSpecularColor[1], CToolMgr::m_fDirectionSpecularColor[2], 1.f);
	
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CTerrainScene* CTerrainScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainScene* pInstance = new CTerrainScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CTerrainScene::Free()
{
	__super::Free();
}
