#include "TerrainScene.h"
#include "framework.h"
#include "Export_Utility.h"
#include "ToolTerrain.h"
#include "ToolCamera.h"
#include "ToolMgr.h"
#include "Engine_Define.h"
#include "ToolTree.h"
#include "ToolRock.h"
#include "ToolGrass.h"
#include <ToolSkyBox.h>
#include <ToolItem.h>

CTerrainScene::CTerrainScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
	//test
}

CTerrainScene::~CTerrainScene()
{
}

HRESULT CTerrainScene::Ready_Scene()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile_Grass_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass1/grass_%d.png", 80,1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile_Grass_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass2/grass2_%d.png", 80,1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../../Client/Bin/Resource/Texture/SkyBox/Ocean.dds")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obejct_Tree", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/IDLE_1/IDLE__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obejct_Stone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Rock/Nomal_Rock/Nomal_Rock_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obejct_Grass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Glass/IDLE/IDLE__000.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Berries", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Berries.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CookedMeat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CookedMeat.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CutGlass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutGlass.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CutStone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/CutStone.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Rocks_0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Rocks_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Rocks_1.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Twigs", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Item/Twigs.png")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));

	return S_OK;
}

_int CTerrainScene::Update_Scene(const _float& fTimeDelta)
{
	Change_LightInfo(fTimeDelta);
	Input_Mouse();

	if (CToolMgr::bSaveData)
	{
		Save_File();
		CToolMgr::bSaveData = false;
	}
	if (CToolMgr::bLoadData)
	{
		FAILED_CHECK_RETURN( Load_File(), E_FAIL);
		CToolMgr::bLoadData = false;
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

	pGameObject = CToolSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

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

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

void CTerrainScene::Save_File()
{
	HANDLE	hFile = CreateFile(
		L"../../Data/mainMap.dat", 
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

	for (auto& iter : m_mapLayer)
	{
		/*iDrawID = dynamic_cast<CTile*>(iter)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(iter)->Get_Option();

		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);*/

		if (iter.first != L"GameLogic")
			continue;

		iCount = iter.second->Get_MapObject().size() - 1;
		WriteFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

		for (auto& objectIter : iter.second->Get_MapObject())
		{
			if (objectIter.first == L"Terrain")
				continue;

			dwStrByte = sizeof(TCHAR) * (_tcslen(objectIter.first) + 1);
			
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, objectIter.first, dwStrByte, &dwByte, nullptr);

			vPos = objectIter.second->Get_Pos();
			WriteFile(hFile, &vPos.x, sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &vPos.y, sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &vPos.z, sizeof(_float), &dwByte, nullptr);

			
		}
	}

	iCount = CToolMgr::vecPickingIdex.size();
	WriteFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < CToolMgr::vecPickingIdex.size(); ++i)
		WriteFile(hFile, &CToolMgr::vecPickingIdex[i], sizeof(_int), &dwByte, nullptr);

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Terrain Save", L"성공", MB_OK);
}

HRESULT CTerrainScene::Load_File()
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

		for (auto& objectIter : iter.second->Get_MapObject())
		{
			if (objectIter.first == L"Terrain")
				break;

			Safe_Release(objectIter.second);
		}
			
		ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);
		
		for (int i =0; i < iCount; ++i)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
			ReadFile(hFile, &vPos.x, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &vPos.y, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &vPos.z, sizeof(_float), &dwByte, nullptr);

			dwStrByte = 0;

			Create_Object(pName, vPos);
		}
	}
	
	ReadFile(hFile, &iCount, sizeof(_int), &dwByte, nullptr);

	for (int i = 0; i < iCount; ++i)
	{
		int iTemp = 0;
		ReadFile(hFile, &iTemp, sizeof(_int), &dwByte, nullptr);
		CToolMgr::vecPickingIdex.push_back(iTemp);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Terrain Load", L"성공", MB_OK);

	return S_OK;
}

HRESULT CTerrainScene::Create_Object(const _tchar* pName, _vec3 vPos)
{
	Engine::CLayer* pLayer = Get_Layer(L"GameLogic");
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	if (!_tcscmp(L"Tree", pName))
	{
		pGameObject = CToolTree::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Tree", pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rock", pName))
	{
		pGameObject = CToolRock::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock", pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Grass", pName))
	{
		pGameObject = CToolGrass::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass", pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"CutGlass", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, pName, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CutGlass", pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rocks_0", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, pName, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rocks_0", pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Rocks_1", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, pName, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rocks_1", pGameObject), E_FAIL);
	}
	else if (!_tcscmp(L"Twigs", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, pName, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Twigs", pGameObject), E_FAIL);
	}

	pGameObject->Set_Pos(vPos);
	return S_OK;
}

HRESULT CTerrainScene::Change_LightInfo(const _float& fTimeDelta)
{
	D3DLIGHT9*			tLightInfo = Engine::Get_Light(0)->Get_Light();
	_float fSpeed = 1;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	int iIndex = 0;
	if (CToolMgr::iAUtoTime == 0)
	{
		iIndex = Engine::Change_Light(fTimeDelta, 0);
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
	Engine::Get_Light(0)->Update_Light();
	return S_OK;
}

HRESULT CTerrainScene::Input_Mouse()
{
	//test
	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		if (CToolMgr::bObjectAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			switch (CToolMgr::iItemCurrentEtcIdx) // 지형지물 오브젝트 생성
			{
			case 0:
				Create_Object(L"Tree", vPickPos); break;
			case 1:
				Create_Object(L"Rock", vPickPos); break;
			case 2:
				Create_Object(L"Grass", vPickPos); break;
			case 3:
				break;
			default:
				break;
			}
			CToolMgr::bObjectAdd = false;
		}
		//"Stick",
		//	"Stone",
		//	"Firestone", //부싯돌
		//	"Grass_Cut",
		if (CToolMgr::bItemAdd)
		{
			_vec3	vPickPos = Picking_Terrain();
			switch (CToolMgr::iItemCurrentItemIdx)
			{
			case 0:
				Create_Object(L"Twigs", vPickPos); break;
			case 1:
				Create_Object(L"Rocks_0", vPickPos); break;
			case 2:
				//Create_Object(L"Firestone", vPickPos); break;
			case 3:
				Create_Object(L"CutGlass", vPickPos); break;
			}
			CToolMgr::bItemAdd = false;
		}
	}
	return S_OK;
}

_vec3 CTerrainScene::Picking_Terrain()
{
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"Terrain", L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"GameLogic", L"Terrain", L"Proto_Transform"));
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
