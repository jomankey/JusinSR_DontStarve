#include "SubScene.h"
#include "framework.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "ToolCamera.h"

#include "Scene.h"
#include "Layer.h"
#include <ToolSkyBox.h>
#include <ToolCubeTerrain.h>
#include <ToolTerrain.h>

#include "ToolMgr.h"
#include <ToolTree.h>
#include <ToolRock.h>
#include <ToolGrass.h>
#include <ToolPigHouse.h>
#include <ToolBerry.h>
#include <ToolItem.h>
#include <ToolMonster.h>

CSubScene::CSubScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev, L"SceneTool")
{
}

CSubScene::~CSubScene()
{
}

HRESULT CSubScene::Ready_Scene()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Tile1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Terrain.png")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../../Client/Bin/Resource/Texture/SkyBox/Ocean.dds")), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_BurgerCube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../../Client/Bin/Resource/Texture/SkyBox/burger0.dds")), E_FAIL);

	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

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

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);

	m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));

	return S_OK;
}

_int CSubScene::Update_Scene(const _float& fTimeDelta)
{
	Input_Mouse();

	return __super::Update_Scene(fTimeDelta);
}

void CSubScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CSubScene::Render_Scene()
{

}

HRESULT CSubScene::Input_Mouse()
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
	//	"Firestone", //ºÎ½Ëµ¹
	//	"Grass_Cut",

	return S_OK;
}

_vec3 CSubScene::Picking_Terrain()
{
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	vector<CGameObject*> pTileBufferCom = m_pCubeTerrain->Get_TileList();

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(m_pTerrain->GetTransForm());
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());

	//return _vec3{};
	return m_pCalculatorCom->Picking_OnCubeTerrain(g_hWnd, pTileBufferCom, pTerrainTransCom, VTXCNTX, 5);
}

HRESULT CSubScene::Ready_Layer_Environment()
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

HRESULT CSubScene::Ready_Layer_GameLogic()
{
	NULL_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC], E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pCubeTerrain = CToolCubeTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::TILE, pGameObject), E_FAIL);

	pGameObject = m_pTerrain = CToolTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[(int)eLAYER_TYPE::GAME_LOGIC]->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
	m_pTerrain->GetTransForm()->Set_Pos(0.f, 1.f, 0.f);

	return S_OK;
}

HRESULT CSubScene::Ready_LightInfo()
{
	return S_OK;
}

void CSubScene::Save_File()
{
}

HRESULT CSubScene::Load_File()
{
	return S_OK;
}

HRESULT CSubScene::Create_Object(const _tchar* pName, _vec3 vPos)
{
	Engine::CLayer* pLayer = scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC);

	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	if (!_tcscmp(L"Tree", pName))
	{
		pGameObject = CToolTree::Create(m_pGraphicDev);
		pGameObject->SetObjName(L"Tree");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 2.5f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"Rock", pName))
	{
		pGameObject = CToolRock::Create(m_pGraphicDev);
		pGameObject->SetObjName(L"Rock");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"Grass", pName))
	{
		pGameObject = CToolGrass::Create(m_pGraphicDev);
		pGameObject->SetObjName(L"Grass");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"PigHouse", pName))
	{
		pGameObject = CToolPigHouse::Create(m_pGraphicDev);
		pGameObject->SetObjName(L"PigHouse");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"BerryBush", pName))
	{
		pGameObject = CToolBerry::Create(m_pGraphicDev);
		pGameObject->SetObjName(L"BerryBush");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"Twigs", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, L"Twigs", vPos);
		pGameObject->SetObjName(L"Twigs");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"Rocks_0", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, L"Rocks_0", vPos);
		pGameObject->SetObjName(L"Rocks_0");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"Firestone", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, L"Firestone", vPos);
		pGameObject->SetObjName(L"Firestone");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"CutGlass", pName))
	{
		pGameObject = CToolItem::Create(m_pGraphicDev, L"CutGlass", vPos);
		pGameObject->SetObjName(L"CutGlass");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}
	else if (!_tcscmp(L"Beefalo", pName) || !_tcscmp(L"Spider", pName) || !_tcscmp(L"Pig", pName) || !_tcscmp(L"Boss", pName))
	{
		pGameObject = CToolMonster::Create(m_pGraphicDev, pName);
		pGameObject->SetObjName(pName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->AddGameObject(eOBJECT_GROUPTYPE::OBJECT, pGameObject), E_FAIL);
		vPos.y = 1.f;
		pGameObject->GetTransForm()->Set_Pos(vPos);
	}

	return S_OK;
}

CSubScene* CSubScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSubScene* pInstance = new CSubScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSubScene::Free()
{
	__super::Free();
}
