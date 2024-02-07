#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "CInven.h"
#include "Monster.h"
#include "ResObject.h"
#include "Component.h"
#include "Layer.h"
#include "Scene.h"
#include "CItem.h"

//Manager
#include "UIMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_bAttack(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
	: Engine::CGameObject(pGraphicDev, _strName)
	, m_bAttack(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
	, m_bAttack(rhs.m_bAttack)
	, m_Stat(rhs.m_Stat)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	/*m_pTransForm->m_vScale = { 1.2f, 1.f, 1.f };*/
	m_eCurState = IDLE;
	m_ePreState = STATE_END;
	m_eCurLook = LOOK_DOWN;

	m_eCurWeapon = UNARMED;
	m_ePreWeapon = WEAPON_END;

	m_Dirchange = false;
	m_KeyLock = false;
	m_bFrameLock = false;
	
	m_vPlayerActing = false;
	m_bIsRoadScene - false;
	m_TargetObject = RSOBJ_END;
	m_fFrameEnd = 22;
	Set_Stat();
	return S_OK;
}

Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	if (scenemgr::Get_CurScene()->Get_Scene_Name() == L"ROAD" && !m_bIsRoadScene)
	{
		m_bIsRoadScene = true;
	}

	if (!m_bFrameLock)		//프레임 락이 걸리면 프레임이 오르지 않음
	{
		m_fFrame += m_fFrameEnd * fTimeDelta;
	}

	if (m_fFrameEnd <= m_fFrame)		//프레임이 끝에 다다르면 진입
	{
		if (m_KeyLock == true)			//KeyLock을 풀고 IDLE 상태로 만듦
		{
			m_KeyLock = false;
			m_eCurState = IDLE;
		}
		if (m_vPlayerActing)
		{
			m_vPlayerActing = false;
		}
		m_fFrame = 0.f;
	}
	if (!m_KeyLock)			//특정 행동에는 KeyLock 을 걸어서 행동중에 다른 행동을 못하게 함
	{
		Key_Input(fTimeDelta);
	}
	Weapon_Change();
	Check_State();
	Look_Change();
	Set_Scale();
	Fire_Light(); // 횃불 모션 시 켜짐 / 꺼짐 추가해야함
	CGameObject::Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	/*Engine::IsPermit_Call(L"Unarmed_IDLE", fTimeDelta);*/
	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3 vPos;
	BillBoard();
	m_pTransForm->Get_Info(INFO::INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);

	FAILED_CHECK_RETURN(SetUp_Material(), );

	if (m_Dirchange)
	{
		m_pReverseCom->Render_Buffer();
	}
	else if (!m_Dirchange)
	{
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM

	//IDLE
	pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });


	//RUN(UNARMED)
	pComponent = m_pTextureCom[LOOK_DOWN][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	//Build
	pComponent = m_pTextureCom[LOOK_DOWN][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	//Pickup
	pComponent = m_pTextureCom[LOOK_UP][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_up", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_down", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });


	//Hit
	pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });


	//Attack
	pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	//Torch(IDLE)
	pComponent = m_pTextureCom[LOOK_DOWN][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_side", pComponent });

	//Torch(Run)
	pComponent = m_pTextureCom[LOOK_DOWN][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_side", pComponent });

	//Axe pre
	pComponent = m_pTextureCom[LOOK_DOWN][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][AXE_CHOP_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_preaxe_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_preaxe_side", pComponent });

	//Axe loop
	/*pComponent = m_pTextureCom[LOOK_DOWN][AXE_CHOP_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_axe_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_axe_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][AXE_CHOP_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_axe_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_axe_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][AXE_CHOP_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_axe_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_axe_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][AXE_CHOP_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_axe_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_axe_side", pComponent });*/

	//Hammer
	pComponent = m_pTextureCom[LOOK_DOWN][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HAMMERING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_hammer_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_hammer_side", pComponent });

	//Pick
	pComponent = m_pTextureCom[LOOK_DOWN][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][PICKING_OBJECT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_picking_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_picking_side", pComponent });


	//Spear
	pComponent = m_pTextureCom[LOOK_DOWN][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][SPEAR_ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_spear_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_spear_side", pComponent });


	//Other
	pComponent = m_pTextureCom[LOOK_DOWN][FALLDOWN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_falldown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_falldown", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][WAKEUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_wakeup"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_wakeup", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][EAT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_eat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_eat", pComponent });
#pragma endregion TEXCOM


	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Pos(0.f, 1.f, 0.f);

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev, _strName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	_vec3		vDir, vRight, vCurPos;
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	m_pTransForm->Get_Info(INFO_LOOK, &vDir);
	m_pTransForm->Get_Info(INFO_RIGHT, &vRight);


	if (GetAsyncKeyState('W'))
		{
			D3DXVec3Normalize(&vDir, &vDir);
			vDir.y = 0.f;
			m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
			m_pTransForm->Get_Info(INFO_POS, &vCurPos);
			if (!m_bIsRoadScene)
			{
				if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
					m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);
			}
			else
			{
				m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);
			}

			if (m_ePreWeapon == TORCH)
			{
				m_eCurState = TORCH_RUN;
			}
			else
			{
				m_eCurState = MOVE;
			}
			m_eCurLook = LOOK_UP;
		}

	if (GetAsyncKeyState('S'))
		{ //f
			D3DXVec3Normalize(&vDir, &vDir);
			vDir.y = 0.f;
			m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);
			m_pTransForm->Get_Info(INFO_POS, &vCurPos);

			if (!m_bIsRoadScene)
			{
				if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
					m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
			}
			else
			{
				m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
			}

			if (m_ePreWeapon == TORCH)
			{
				m_eCurState = TORCH_RUN;
			}
			else
			{
				m_eCurState = MOVE;
			}
			m_eCurLook = LOOK_DOWN;

		}
	if (GetAsyncKeyState('A'))
		{
			D3DXVec3Normalize(&vRight, &vRight);
			vDir.y = 0.f;
			m_pTransForm->Move_Pos(&vRight, -m_Stat.fSpeed, fTimeDelta);
			m_pTransForm->Get_Info(INFO_POS, &vCurPos);
			vCurPos.x += 0.5f;

			if (!m_bIsRoadScene)
			{
				if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
					m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);
			}
			else
			{
				m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);
			}

			if (m_ePreWeapon == TORCH)
			{
				m_eCurState = TORCH_RUN;
			}
			else
			{
				m_eCurState = MOVE;
			}
			m_eCurLook = LOOK_LEFT;
			/*m_pTransForm->Rotation(ROT_Y, D3DXToRadian(90.f * fTimeDelta));*/
		}
	if (GetAsyncKeyState('D'))
		{
			D3DXVec3Normalize(&vRight, &vRight);
			vDir.y = 0.f;
			m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);
			m_pTransForm->Get_Info(INFO_POS, &vCurPos);
			vCurPos.x -= 0.5f;
			if (!m_bIsRoadScene)
			{
				if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
					m_pTransForm->Set_Pos(vCurPos);
			}
			else
			{
				m_pTransForm->Set_Pos(vCurPos);
			}

			if (m_ePreWeapon == TORCH)
			{
				m_eCurState = TORCH_RUN;
			}
			else
			{
				m_eCurState = MOVE;
			}
			m_eCurLook = LOOK_RIGHT;

		}


	if (!GetAsyncKeyState('W') &&              //이동중이지 않을 때 IDLE로 변경
		!GetAsyncKeyState('S') &&
		!GetAsyncKeyState('A') &&
		!GetAsyncKeyState('D'))
	{
		if (m_ePreWeapon == TORCH)				//횃불을 들고 있을 시에는 횃불모션으로 변경
		{
			m_eCurState = TORCH_IDLE;
		}
		else
			m_eCurState = IDLE;

	}



	if (GetAsyncKeyState('Z'))
	{
		//Find_NeerObject: 못찾았을경우 nullptr반환

		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::RESOURCE_OBJECT);
		if (nullptr != findObj)
		{
			_vec3 vPos;
			m_pTransForm->Get_Info(INFO_POS, &vPos);
			findObj->GetTransForm()->Get_Info(INFO_POS, &m_vTargetPos);
			m_TargetObject = dynamic_cast<CResObject*>(findObj)->Get_Resourse_ID();
			m_vTargetDir = m_vTargetPos - vPos;
			m_vTargetDir.y = 0.f;
			if (D3DXVec3Length(&m_vTargetDir) < 1.f)
			{
				ResObj_Mining(m_TargetObject , findObj);
			}
			else
			{
				m_eCurLook = m_pTransForm->For_Player_Direction(&m_vTargetDir, m_Stat.fSpeed, fTimeDelta);
				if (m_ePreWeapon == TORCH)
				{
					m_eCurState = TORCH_RUN;
				}
				else
					m_eCurState = MOVE;
			}

			
		}
	}
	if (KEY_TAP(DIK_X))// KEY_TAP(누르는시점) , KEY_AWAY (키를떼는시점), KEY_NONE(키를안누른상태), KEY_HOLD(키를누르고있는상태)
	{
		//Find_NeerObject: 못찾았을경우 nullptr반환
		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::ITEM);

		if (nullptr != findObj)
		{
			if (CUIMgr::GetInstance()->AddItem(m_pGraphicDev, findObj->GetObjName()));
				DeleteObject(findObj);
		}
	}

	if (GetAsyncKeyState('F')) // 공격
	{
		m_eCurState = ATTACK;
		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::MONSTER);
		if (nullptr != findObj && !findObj->IsDelete()
			&& dynamic_cast<CMonster*>(findObj)->IsTarget_Approach(m_Stat.fATKRange))
		{
			dynamic_cast<CMonster*>(findObj)->Set_Attack(m_Stat.fATK);
			
		}
	}

	if (GetAsyncKeyState('G')) // 횃불
	{
		m_eCurWeapon = TORCH;
	}

	if (GetAsyncKeyState('V')) // 줍기
	{
		m_eCurState = PICKUP;
		auto pLayer = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM);
		_vec3 vPlayerPos, vPlayerScale, vItemPos, vItemScale;
		m_pTransForm->Get_Info(INFO_POS, &vPlayerPos);
		vPlayerScale = m_pTransForm->Get_Scale();

		for (auto& object : pLayer)
		{
			//_vec3 pPlayerPos, _vec3 pItemPos, _vec3 vPlayerScale, _vec3 vItemScale
			CTransform* pItemTransform = object->GetTransForm();
			pItemTransform->Get_Info(INFO_POS, &vItemPos);
			vItemScale = pItemTransform->Get_Scale();

			if (Engine::Collision_Item(vPlayerPos, vItemPos, vPlayerScale, vItemScale))
				break;
		}
	}
}

HRESULT CPlayer::SetUp_Material()
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;
	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}


void CPlayer::Check_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_fFrameEnd = 22;
			break;
		case HIT:
			m_fFrameEnd = 7;
			m_KeyLock = true;
			break;
		case BUILD:
			m_fFrameEnd = 6;
			break;
		case ATTACK:
			m_fFrameEnd = 11;
			m_KeyLock = true;
			break;
		case FALLDOWN:
			m_fFrameEnd = 8;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		case WAKEUP:
			m_fFrameEnd = 32;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		case EAT:
			m_fFrameEnd = 36;
			m_KeyLock = true;
			m_eCurLook = LOOK_DOWN;
			break;
		case MOVE:
			m_fFrameEnd = 6;
			break;
		case TORCH_IDLE:
			m_fFrameEnd = 22;
			break;
		case TORCH_RUN:
			m_fFrameEnd = 6;
			break;
		case PICKING_OBJECT:
			m_fFrameEnd = 9;
			break;
		case AXE_CHOP_PRE:
			m_fFrameEnd = 15;
			break;
		}
		m_ePreState = m_eCurState;
		m_fFrame = 0.f;
	}
	else
		return;

}

void CPlayer::Set_Scale()
{
	if (m_eCurState == BUILD) //B
		m_pTransForm->m_vScale = { 0.85f, 0.7f, 0.85f };

	else if ((m_eCurLook == LOOK_LEFT || m_eCurLook == LOOK_RIGHT) && m_eCurState == PICKUP)
		m_pTransForm->m_vScale = { 1.0f, 0.1f, 1.0f };

	else if (m_eCurState == PICKUP)
		m_pTransForm->m_vScale = { 0.73f, 0.63f, 0.73f };

	else if ((m_eCurLook == LOOK_LEFT || m_eCurLook == LOOK_RIGHT) && m_eCurState == ATTACK)
		m_pTransForm->m_vScale = { 1.f, 0.3f, 1.f };

	else if (m_eCurLook == LOOK_UP && m_eCurState == ATTACK)
		m_pTransForm->m_vScale = { 0.8f, 0.3f, 0.8f };

	else if (m_eCurState == ATTACK)
		m_pTransForm->m_vScale = { 0.85f, 0.8f, 0.85f };

	else if (m_eCurState == HIT) //H
		m_pTransForm->m_vScale = { 0.9f, 1.f, 0.9f };

	else if (m_eCurState == FALLDOWN)
		m_pTransForm->m_vScale = { 1.f, 1.f,1.0f };

	else if (m_eCurState == WAKEUP)
		m_pTransForm->m_vScale = { 1.1f, 1.f, 1.1f };

	else if (m_eCurState == EAT) // H
		m_pTransForm->m_vScale = { 1.1f, 0.3f, 1.1f };

	else if (m_eCurState == MOVE && (m_eCurLook == LOOK_LEFT || m_eCurLook == LOOK_RIGHT))
		m_pTransForm->m_vScale = { 0.9f, 0.6f, 0.8f };

	else if (m_eCurState == MOVE)
		m_pTransForm->m_vScale = { 0.7f, 1.f, 0.7f };

	else if (m_eCurState == PICKING_OBJECT && (m_eCurLook == LOOK_LEFT || m_eCurLook == LOOK_RIGHT))
		m_pTransForm->m_vScale = { 1.7f,0.05f,1.f };

	else if (m_eCurState == PICKING_OBJECT && (m_eCurLook == LOOK_UP || m_eCurLook == LOOK_DOWN))
		m_pTransForm->m_vScale = { 1.2f,0.5f,1.f };

	//else if (m_eCurState == AXE_CHOP_PRE)
	//	m_pTransForm->m_vScale = { 1.f, 0.5f, 0.7f };

	else
		m_pTransForm->m_vScale = { 0.7f, 0.5f, 0.7f };

}

void CPlayer::Set_Stat()
{
	m_Stat.fHP = 200.f;
	m_Stat.fMxHP = 150.f;
	m_Stat.fSpeed = 5.f;
	m_Stat.fATK = 10.f;
	m_Stat.fATKRange = 1.f;
	m_Stat.fAggroRange = 5.f;
	m_Stat.bDead = false;

}

void CPlayer::Set_Attack(int _Atk)
{
	if (!m_Stat.bDead && m_Stat.fHP > 0)
	{
		m_Stat.fHP -= _Atk;
		m_eCurState = HIT;
		m_KeyLock = true;
	}
}

void CPlayer::Weapon_Change()
{
	if (m_ePreWeapon != m_eCurWeapon)
	{
		switch (m_eCurWeapon)
		{
		case UNARMED:
			m_Stat.fATK = 20;
			m_Stat.fATKRange = 1.f;
			break;
		case AXE:
			break;
		case TORCH:
			if (m_eCurState = IDLE)
			{
				m_eCurState = TORCH_IDLE;
			}
			break;
		case HAMMER:
			break;
		case PICK:
			break;
		case SPEAR:
			m_Stat.fATK = 50;
			m_Stat.fATKRange = 2.f;
			break;
		}
		m_ePreWeapon = m_eCurWeapon;
	}

}

void CPlayer::Look_Change()
{
	if (m_ePreLook != m_eCurLook)
	{
		if (m_eCurLook == LOOK_LEFT)
		{
			m_Dirchange = true;
		}
		else
		{
			m_Dirchange = false;
		}

		m_ePreLook = m_eCurLook;
	}

}

void CPlayer::ResObj_Mining(RESOBJID _ObjID, CGameObject* _Obj)
{
	switch (_ObjID)
	{
	case ROCK:
		m_eCurState = PICKING_OBJECT;
		if ((m_fFrameEnd-1) < m_fFrame && !m_vPlayerActing)
		{
			dynamic_cast<CResObject*>(_Obj)->Set_Attack();
			m_vPlayerActing = true;
		}
		break;
	case TREE:
		if (7.f < m_fFrame && !m_vPlayerActing)
		{
			dynamic_cast<CResObject*>(_Obj)->Set_Attack();
			dynamic_cast<CResObject*>(_Obj)->Set_Attack_State(true);
			m_vPlayerActing = true;
		}
		m_eCurState = AXE_CHOP_PRE;
		break;
	case PIG_HOUSE:
		m_eCurState = HAMMERING;
		break;
	case GRASS:
		if ((m_fFrameEnd - 1) < m_fFrame && !m_vPlayerActing)
		{
			dynamic_cast<CResObject*>(_Obj)->Set_Attack();
			m_vPlayerActing = true;
		}
		m_eCurState = BUILD;
		break;
	case BERRY_BUSH:
		if ((m_fFrameEnd - 1) < m_fFrame && !m_vPlayerActing)
		{
			dynamic_cast<CResObject*>(_Obj)->Set_Attack();
			m_vPlayerActing = true;
		}
		m_eCurState = BUILD;
		break;
	}
}

HRESULT CPlayer::Ready_Light()
{
	//점광원
// 최초 생성 후 플레이어 횟불 사용 시에만 켜지도록 
	D3DLIGHT9 tPointLightInfo;
	ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo.Type = D3DLIGHT_POINT;

	tPointLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Attenuation0 = 0.00000001f;
	tPointLightInfo.Range = 5.f;
	tPointLightInfo.Position = { 0.f, 0.f, 0.f };

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tPointLightInfo, 1), E_FAIL);
	light::Get_Light(1)->Close_Light();

}

void CPlayer::Fire_Light()
{
	if (m_ePreWeapon != TORCH)
		return;

	D3DLIGHT9* tPointLightInfo = light::Get_Light(1)->Get_Light();
	//ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo->Type = D3DLIGHT_POINT;

	tPointLightInfo->Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.6f);
	tPointLightInfo->Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.6f);
	tPointLightInfo->Attenuation0 = 0.000000001f;
	tPointLightInfo->Range = 5.f;

	_vec3 pPlayerPos;
	m_pTransForm->Get_Info(INFO_POS, &pPlayerPos); // player pos 값 설정
	tPointLightInfo->Position = pPlayerPos;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);
	light::Get_Light(1)->Update_Light();
}


//못찾았을경우 nullptr반환
CGameObject* CPlayer::Find_NeerObject(float _fRange, eOBJECT_GROUPTYPE _findTarget)
{
	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, _findTarget);

	_vec3 vPlayerPos, vTargetPos, vLook;
	_float fNeerDistance = FLT_MAX;
	_float fFindDistance = 0.f;
	CGameObject* pFindObject = nullptr;

	ZeroMemory(vPlayerPos, sizeof(_vec3));
	ZeroMemory(vTargetPos, sizeof(_vec3));
	ZeroMemory(vLook, sizeof(_vec3));

	m_pTransForm->Get_Info(INFO_POS, &vPlayerPos);//플레이어좌표 구하기
	vPlayerPos.y = 0.f;

	//반복문돌면서 모든 오브젝트그룹 순회하기
	for (auto& obj : vecObj)
	{
		if (obj->IsDelete())//해당오브젝트가 삭제될예정이면 무시
			continue;

		obj->GetTransForm()->Get_Info(INFO_POS, &vTargetPos);
		vTargetPos.y = 0.f;

		vLook = vTargetPos - vPlayerPos;

		fFindDistance = D3DXVec3Length(&vLook);
		if (_fRange < fFindDistance)//현재 찾은거리가 범위보다 클경우
			continue;

		if (fNeerDistance > fFindDistance)//현재 찾은 거리가 이전에찾았던 가장가까운거리보다 작을경우
		{
			pFindObject = obj;
			fNeerDistance = fFindDistance;
		}


	}

	return pFindObject;
}


void CPlayer::BillBoard()
{
	_matrix	matWorld, matView, matBill;

	m_pTransForm->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill._23 = matView._23;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransForm->Set_WorldMatrix(&(matBill * matWorld));
}



void CPlayer::Free()
{
	__super::Free();
}

//DeleteObject(삭제할게임오브젝트포인터 (GameObject*));
//CreateObject(레이어타입,오브젝트타입,생성한 오브젝트포인터);
