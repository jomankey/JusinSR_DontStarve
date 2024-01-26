#include "..\Include\stdafx.h"
#include "..\Header\Loading.h"
#include "Export_System.h"
#include "Export_Utility.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL,	// 디폴트 보안 속성
		0,		// 디폴트 스택 사이즈(1바이트)
		Thread_Main, // 동작 시킬 쓰레드 함수 이름(__stdcall 규약)
		this,		// 쓰레드 함수의 매개 변수로 전달할 값
		0,			// 쓰레드 생성 및 실행을 조절하기 위한 FLAG
		NULL);		// 쓰레드 ID 반환..

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	Loading_OBJ_Texture();
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Terrain0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HeightTerrainTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Terrain0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player%d.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
	Loading_Player_Texture();
	Loading_Beefalo_Texture();

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	//Objects Resource
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obejct_Tree", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Tree/IDLE_1/IDLE__000.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Nomal_Rock", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Rock/Nomal_Rock/Nomal_Rock_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obejct_Grass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Monster/Resource/Stone/IDLE/IDLE__000.png")), E_FAIL);


	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete");

	return 0;
}

_uint CLoading::Loading_Player_Texture()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_idle_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/idle_down/idle_down__%03d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_idle_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/idle_up/idle_up__%03d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_idle_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/idle_side/idle_side__%03d.png", 23)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_run_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/run_down/run_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_run_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/run_up/run_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_run_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/run_side/run_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_build_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/build_down/build_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_build_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/build_up/build_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_build_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/build_side/build_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_pickup_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/pickup_down/pickup_down__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_pickup_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/pickup_up/pickup_up__%03d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_pickup_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/pickup_side/pickup_side__%03d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_hit_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/hit_std/hit_std__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_hit_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/hit_up/hit_up__%03d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_hit_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/hit_side/hit_side__%03d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_punch_down", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/attack/punch_down/punch_down__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_punch_up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/attack/punch_up/punch_up__%03d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Unarmed_punch_side", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/attack/punch_side/punch_side__%03d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_falldown", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/fall_down/fall_down__%03d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_wakeup", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/wake_up/wake_up__%03d.png", 32)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_eat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/UnArmed/eat/eat__%03d.png", 36)), E_FAIL);


	return 0;
}

_uint CLoading::Loading_UI_Texture()
{
	//s
	//PS2
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Left_Panel", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Left_Panel.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Item_Inven_Slot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Item_Inven_Slot.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Equipment", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Equipment.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Cloth", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Cloth.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Alive", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Hud/Alive.png")), E_FAIL);
}
_uint CLoading::Loading_Beefalo_Texture()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Beefalo_graze", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/beefalo/beefalo_graze/befalo_graze__%03d.png", 10)), E_FAIL);
	return 0;
}


unsigned int CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
