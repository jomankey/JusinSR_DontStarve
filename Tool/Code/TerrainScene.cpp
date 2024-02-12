#include "TerrainScene.h"
#include "framework.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "ToolTerrain.h"
#include "ToolCamera.h"
#include "ToolMgr.h"
#include "Engine_Define.h"
#include "ToolTree.h"
#include "ToolRock.h"
#include "ToolGrass.h"
#include <ToolSkyBox.h>
#include <ToolItem.h>
#include "ToolObj.h"

#include "Scene.h"
#include "Layer.h"
#include <ToolPigHouse.h>
#include <ToolBerry.h>

CTerrainScene::CTerrainScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev, L"SceneTool")
{
	//test
}

CTerrainScene::~CTerrainScene()
{
}

HRESULT CTerrainScene::Ready_Scene()
{

	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain0.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile_Grass_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass1/grass_%d.png", 80, 1)), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile_Grass_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass2/grass2_%d.png", 80, 1)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../../Client/Bin/Resource/Texture/SkyBox/Ocean.dds")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Berries", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Berries.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CookedMeat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CookedMeat.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CutGlass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutGlass.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"CutStone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutStone.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rocks_0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Rocks_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_1.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Twigs", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Twigs.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Tree", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/idle/tree_idle__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Stone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Rock/Nomal_Rock/Nomal_Rock_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Grass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Glass/IDLE/IDLE__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Pig_House", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/pig_house/idle/idle__0.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Obejct_Berry", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/most/most_idle__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Beefalo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/beefalo/beefalo_graze/befalo_graze__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Spider", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/spider/move/walk_down/walk_down__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Pig", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/pig/idle_happy/happy__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Boss", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/berrybush/most/most_idle__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FireFlies_Pre", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_pre/swarm_pre__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FireFlies_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_loop/swarm_loop__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_FireFlies_Pst", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/FireFlies/swarm_pst/swarm_pst__000.png")), E_FAIL);

	//Obj_Teleporter
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Teleporter_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Teleporter_worm/Idle/WormIdle__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_Teleporter_Open", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Teleporter_worm/Open/WormOpen__000.png")), E_FAIL);

	//Obj_BossDoor
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Open", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/activate/activate__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Close", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/deactive/deactive__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/idle_loop_on/idle_loop_on__000.png")), E_FAIL);
	//FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Object_BossDoor_Off", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/BossDoor/idle_off__000.png")), E_FAIL);


	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));

	return S_OK;
}

_int CTerrainScene::Update_Scene(const _float& fTimeDelta)
{
	Change_LightInfo(fTimeDelta);
	Input_Mouse();

	if (CToolMgr::bObjSaveData)
	{
		Save_ObjFile();
		CToolMgr::bObjSaveData = false;
	}
	if (CToolMgr::bObjLoadData)
	{
		FAILED_CHECK_RETURN(Load_ObjFile(), E_FAIL);
		CToolMgr::bObjLoadData = false;
	}

	if (CToolMgr::bPointSaveData)
	{
		Save_PointFile();
		CToolMgr::bPointSaveData = false;
	}
	if (CToolMgr::bPointLoadData)
	{
		FAILED_CHECK_RETURN(Load_PointFile(), E_FAIL);
		CToolMgr::bPointLoadData = false;
	}

	if (CToolMgr::bLightSaveData)
	{
		Save_PointFile();
		CToolMgr::bLightSaveData = false;
	}
	if (CToolMgr::bLightLoadData)
	{
		FAILED_CHECK_RETURN(Load_PointFile(), E_FAIL);
		CToolMgr::bLightLoadData = false;
	}

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

HRESULT CTerrainScene::Ready_Layer_Environment()
{

	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT], E_FAIL);

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
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::CAMERA, pGameObject), E_FAIL);

	pGameObject = CToolSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::ENVIRONMENT]->AddGameObject(eOBJECT_GROUPTYPE::BACK_GROUND, pGameObject), E_FAIL);


	return S_OK;
}

HRESULT CTerrainScene::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pTerrain = CToolTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CTerrainScene::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	//tLightInfo.Diffuse = D3DXCOLOR(CToolMgr::m_fDirectionDiffuseColor[0], CToolMgr::m_fDirectionDiffuseColor[1], CToolMgr::m_fDirectionDiffuseColor[2], 1.f);
	//tLightInfo.Specular = D3DXCOLOR(CToolMgr::m_fDirectionAmbientColor[0], CToolMgr::m_fDirectionAmbientColor[1], CToolMgr::m_fDirectionAmbientColor[2], 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(CToolMgr::m_fDirectionSpecularColor[0], CToolMgr::m_fDirectionSpecularColor[1], CToolMgr::m_fDirectionSpecularColor[2], 1.f);

	tLightInfo.Diffuse = D3DXCOLOR(CToolMgr::m_fDirectionDiffuseColor[CToolMgr::iTimeLight].x,
		CToolMgr::m_fDirectionDiffuseColor[CToolMgr::iTimeLight].y,
		CToolMgr::m_fDirectionDiffuseColor[CToolMgr::iTimeLight].z, 1.f);
	tLightInfo.Specular = D3DXCOLOR(CToolMgr::m_fDirectionAmbientColor[CToolMgr::iTimeLight].x,
		CToolMgr::m_fDirectionAmbientColor[CToolMgr::iTimeLight].y,
		CToolMgr::m_fDirectionAmbientColor[CToolMgr::iTimeLight].z, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(CToolMgr::m_fDirectionSpecularColor[CToolMgr::iTimeLight].x,
		CToolMgr::m_fDirectionSpecularColor[CToolMgr::iTimeLight].y,
		CToolMgr::m_fDirectionSpecularColor[CToolMgr::iTimeLight].z, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

void CTerrainScene::Save_ObjFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Obj.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	_vec3 vPos{};
	_int iCount(0);
	DWORD	dwByte(0), dwStrByte(0);
	string pName;

	auto list = GetLayer(eLAYER_TYPE::GAME_LOGIC)->GetGroupObject(eOBJECT_GROUPTYPE::OBJECT);

	iCount = list.size();
	WriteFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (auto& objectIter : list)
	{
		dwStrByte = sizeof(TCHAR) * (objectIter->GetObjName().size() +1);

		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, objectIter->GetObjName().c_str(), dwStrByte, &dwByte, nullptr);

		objectIter->GetTransForm()->Get_Info(INFO_POS, &vPos);
		WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
	}

	for (int i = 0; i < 3; ++i)
	{
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	MessageBox(g_hWnd, L"Terrain Save", L"성공", MB_OK);
	CloseHandle(hFile);
}

HRESULT CTerrainScene::Load_ObjFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Obj.dat",
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
	}

	for (int i = 0; i < 3; ++i)
	{
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Terrain Load", L"성공", MB_OK);

	return S_OK;
}

void CTerrainScene::Save_PointFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Point.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	_vec3 vPos{};
	_int iCount(0);
	DWORD	dwByte(0), dwStrByte(0);

	iCount = CToolMgr::vecPickingIdex.size();
	WriteFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < CToolMgr::vecPickingIdex.size(); ++i)
		WriteFile(hFile, &CToolMgr::vecPickingIdex[i], sizeof(_int), &dwByte, nullptr);

	MessageBox(g_hWnd, L"Point Save", L"성공", MB_OK);
	CloseHandle(hFile);
}

HRESULT CTerrainScene::Load_PointFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Point.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_int iCount(0);
	DWORD	dwByte(0);

	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		int iTemp = 0;
		ReadFile(hFile, &iTemp, sizeof(_int), &dwByte, nullptr);
		CToolMgr::vecPickingIdex.push_back(iTemp);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Point Load", L"성공", MB_OK);

	return S_OK;
}

void CTerrainScene::Save_LightFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Light.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	_vec3 vPos{};
	DWORD	dwByte(0);

	for (int i = 0; i < 3; ++i)
	{
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	MessageBox(g_hWnd, L"Light Save", L"성공", MB_OK);
	CloseHandle(hFile);
}

HRESULT CTerrainScene::Load_LightFile()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/MainMap_Light.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte(0);

	for (int i = 0; i < 3; ++i)
	{
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionDiffuseColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionAmbientColor[i].z, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].x, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].y, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &CToolMgr::m_fDirectionSpecularColor[i].z, sizeof(_float), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Light Load", L"성공", MB_OK);

	return S_OK;
}

HRESULT CTerrainScene::Create_Object(const _tchar* pName, _vec3 vPos)
{
	Engine::CLayer* pLayer = scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC);

	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	//if (!_tcscmp(L"Tree", pName))
	//{
	//	pGameObject = CToolTree::Create(m_pGraphicDev);
	//	pGameObject->SetObjName(L"Tree");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 2.5f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"Rock", pName))
	//{
	//	pGameObject = CToolRock::Create(m_pGraphicDev);
	//	pGameObject->SetObjName(L"Rock");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"Grass", pName))
	//{
	//	pGameObject = CToolGrass::Create(m_pGraphicDev);
	//	pGameObject->SetObjName(L"Grass");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"PigHouse", pName))
	//{
	//	pGameObject = CToolPigHouse::Create(m_pGraphicDev);
	//	pGameObject->SetObjName(L"PigHouse");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"BerryBush", pName))
	//{
	//	pGameObject = CToolBerry::Create(m_pGraphicDev);
	//	pGameObject->SetObjName(L"BerryBush");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"Twigs", pName))
	//{
	//	pGameObject = CToolItem::Create(m_pGraphicDev, L"Twigs", vPos);
	//	pGameObject->SetObjName(L"Twigs");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"Rocks_0", pName))
	//{
	//	pGameObject = CToolItem::Create(m_pGraphicDev, L"Rocks_0", vPos);
	//	pGameObject->SetObjName(L"Rocks_0");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"Firestone", pName))
	//{
	//	pGameObject = CToolItem::Create(m_pGraphicDev, L"Firestone", vPos);
	//	pGameObject->SetObjName(L"Firestone");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"CutGlass", pName))
	//{
	//	pGameObject = CToolItem::Create(m_pGraphicDev, L"CutGlass",vPos);
	//	pGameObject->SetObjName(L"CutGlass");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	//	vPos.y = 1.f;
	//	pGameObject->GetTransForm()->Set_Pos(vPos);
	//}
	//else if (!_tcscmp(L"Beefalo", pName) || !_tcscmp(L"Spider", pName) || !_tcscmp(L"Pig", pName) || !_tcscmp(L"Boss", pName) || 
	//	!_tcscmp(L"Proto_Object_FireFlies_Pre", pName) || 
	//	!_tcscmp(L"Proto_Object_Teleporter_Idle", pName)
	//	|| !_tcscmp(L"Proto_Object_BossDoor_Open", pName))
	//{

	//}

	pGameObject = CToolObj::Create(m_pGraphicDev, pName);
	pGameObject->SetObjName(pName);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	vPos.y = 1.f;
	pGameObject->GetTransForm()->Set_Pos(vPos);

	return S_OK;
}

HRESULT CTerrainScene::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9* tLightInfo = light::Get_Light(0)->Get_Light();
	_float fSpeed = 1;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	int iIndex = 0;
	if (CToolMgr::iAUtoTime == 0)
	{
		iIndex = light::Change_Light(fTimeDelta, 0);
	}
	else
		iIndex = CToolMgr::iTimeLight;

	// 낮, 오후, 밤 시간 별 조명 값이 다름.
	// 시간이 변경될 때 마다 조명이 같아질 수 있도록 값을 시간에 따라 변경해줘야 함.

	if (CToolMgr::iAUtoTime == 0)
	{
		vLight[0].x += (CToolMgr::m_fDirectionDiffuseColor[iIndex].x - vLight[0].x) * fTimeDelta;
		vLight[0].y += (CToolMgr::m_fDirectionDiffuseColor[iIndex].y - vLight[0].y) * fTimeDelta;
		vLight[0].z += (CToolMgr::m_fDirectionDiffuseColor[iIndex].z - vLight[0].z) * fTimeDelta;

		vLight[1].x += (CToolMgr::m_fDirectionAmbientColor[iIndex].x - vLight[1].x) * fTimeDelta;
		vLight[1].y += (CToolMgr::m_fDirectionAmbientColor[iIndex].y - vLight[1].y) * fTimeDelta;
		vLight[1].z += (CToolMgr::m_fDirectionAmbientColor[iIndex].z - vLight[1].z) * fTimeDelta;

		vLight[2].x += (CToolMgr::m_fDirectionSpecularColor[iIndex].x - vLight[2].x) * fTimeDelta;
		vLight[2].y += (CToolMgr::m_fDirectionSpecularColor[iIndex].y - vLight[2].y) * fTimeDelta;
		vLight[2].z += (CToolMgr::m_fDirectionSpecularColor[iIndex].z - vLight[2].z) * fTimeDelta;
	}
	else
	{
		vLight[0].x = CToolMgr::m_fDirectionDiffuseColor[iIndex].x;
		vLight[0].y = CToolMgr::m_fDirectionDiffuseColor[iIndex].y;
		vLight[0].z = CToolMgr::m_fDirectionDiffuseColor[iIndex].z;

		vLight[1].x = CToolMgr::m_fDirectionAmbientColor[iIndex].x;
		vLight[1].y = CToolMgr::m_fDirectionAmbientColor[iIndex].y;
		vLight[1].z = CToolMgr::m_fDirectionAmbientColor[iIndex].z;

		vLight[2].x = CToolMgr::m_fDirectionSpecularColor[iIndex].x;
		vLight[2].y = CToolMgr::m_fDirectionSpecularColor[iIndex].y;
		vLight[2].z = CToolMgr::m_fDirectionSpecularColor[iIndex].z;
	}

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

HRESULT CTerrainScene::Input_Mouse()
{
	//test
	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		if (CToolMgr::bMonsterAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			switch (CToolMgr::iItemCurrentMonsterIdx)
			{
			case 0:
				Create_Object(L"Spider", vPickPos);
				break;
			case 1:
				Create_Object(L"Pig", vPickPos);
				break;
			case 2:
				Create_Object(L"Beefalo", vPickPos);
				break;
			case 3:
				//Create_Object(L"Boss", vPickPos);
				break;
			default:
				break;
			}
			CToolMgr::bMonsterAdd = false;
		}
		else if (CToolMgr::bObjectAdd)
		{
			_vec3	vPickPos = Picking_Terrain();

			switch (CToolMgr::iItemCurrentEtcIdx)
			{
			case 0:
				Create_Object(L"Tree", vPickPos);
				break;
			case 1:
				Create_Object(L"Rock", vPickPos);
				break;
			case 2:
				Create_Object(L"Grass", vPickPos);
				break;
			case 3:
				Create_Object(L"PigHouse", vPickPos);
				break;
			case 4:
				Create_Object(L"BerryBush", vPickPos);
				break;
			default:
				break;
			}
			CToolMgr::bObjectAdd = false;
		}

		else if (CToolMgr::bItemAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			switch (CToolMgr::iItemCurrentItemIdx)
			{
			case 0:
				Create_Object(L"Twigs", vPickPos); break;
			case 1:
				Create_Object(L"Rocks_0", vPickPos); break;
			case 2:
				Create_Object(L"Firestone", vPickPos); break;
			case 3:
				Create_Object(L"CutGlass", vPickPos); break;
			default:
				break;
			}
			CToolMgr::bItemAdd = false;
		}
	}
	//"Stick",
	//	"Stone",
	//	"Firestone", //부싯돌
	//	"Grass_Cut",
	
	return S_OK;
}

_vec3 CTerrainScene::Picking_Terrain()
{
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(pTerrain->GetTransForm());
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
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
