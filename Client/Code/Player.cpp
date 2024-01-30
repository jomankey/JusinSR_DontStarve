#include "..\Include\stdafx.h"
#include "..\Header\Player.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include"CInven.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
{

}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/*m_pTransformCom->m_vScale = { 1.2f, 1.f, 1.f };*/
	m_eCurState = IDLE;
	m_ePreState = STATE_END;
	m_ePlayerLookAt = LOOK_DOWN;
	m_cTex = nullptr;
	m_Dirchange = false;
	m_fFrameEnd = 22;
	return S_OK;
}
Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
		m_fFrame = 0.f;

	Key_Input(fTimeDelta);
	Check_State();
	
	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	
	/*Engine::IsPermit_Call(L"Unarmed_IDLE", fTimeDelta);*/
	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	BillBoard();
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);

	/*Height_OnTerrain();*/
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	Set_Scale();

	m_pTextureCom[m_ePlayerLookAt][m_ePreState]->Set_Texture((_uint)m_fFrame);

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
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;
	
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	
	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(Engine::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });


	//enum PLAYERSTATE {
	//	IDLE, MOVE, BUILD, PICKUP, HIT, ATTACK, FALLDOWN, WAKEUP, EAT, STATE_END
	//};
	/*enum PLAYERLOOK { LOOK_DOWN, LOOK_RIGHT, LOOK_LEFT, LOOK_UP, LOOK_END };*/
	pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][MOVE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][MOVE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][MOVE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][MOVE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][BUILD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_build_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][BUILD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_build_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][BUILD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][BUILD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][PICKUP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_pickup_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_up", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][PICKUP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_pickup_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_down", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][PICKUP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][PICKUP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_hit_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HIT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_hit_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_punch_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_punch_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][FALLDOWN] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_falldown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_falldown", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][WAKEUP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_wakeup"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_wakeup", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][EAT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_eat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_eat", pComponent });

	

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	/*pComponent = m_pStatusCom = dynamic_cast<CPlayerStatus*>(Engine::Clone_Proto(L"Proto_PlayerStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PlayerStat", pComponent });*/

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free()
{	
	__super::Free();
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	
	_vec3		vDir,vRight , vCurPos;
	CTerrainTex * pTerrainTex = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"Terrain", L"Proto_TerrainTex"));
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState('W'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);
		m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransformCom->Move_Pos(&vDir, -5.f, fTimeDelta);

		m_eCurState = MOVE;
		m_ePlayerLookAt = LOOK_UP;
	}

	if (GetAsyncKeyState('S'))
	{ //f
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, -5.f, fTimeDelta);
		m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);
		m_eCurState = MOVE;
		m_ePlayerLookAt = LOOK_DOWN;
		
	}

	if (GetAsyncKeyState('A'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransformCom->Move_Pos(&vRight, -5.f, fTimeDelta);
		m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
		vCurPos.x += 0.5f;
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransformCom->Move_Pos(&vRight, 5.f, fTimeDelta);
			
		m_eCurState = MOVE;
		m_ePlayerLookAt = LOOK_LEFT;
		if (!m_Dirchange)
		{
			m_Dirchange = true;
		}
		/*m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90.f * fTimeDelta));*/
	}

	if (GetAsyncKeyState('D'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransformCom->Move_Pos(&vRight, 5.f, fTimeDelta);
		m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
		vCurPos.x -= 0.5f;
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransformCom->Set_Pos(vCurPos);
			
		m_eCurState = MOVE;
		m_ePlayerLookAt = LOOK_RIGHT;
		if (m_Dirchange)
		{
			m_Dirchange = false;
		}
		/*m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-90.f * fTimeDelta));*/
	}

	if (!GetAsyncKeyState('W') &&
		!GetAsyncKeyState('A') &&
		!GetAsyncKeyState('S') &&
		!GetAsyncKeyState('D'))
	{
	
		m_eCurState = IDLE;
	}

	if (GetAsyncKeyState('F'))
	{
		m_eCurState = ATTACK;
	}
	if (GetAsyncKeyState('G'))
	{
		m_eCurState = HIT;
	}
	if (GetAsyncKeyState('V'))
	{
		m_eCurState = PICKUP;
	}
	if (GetAsyncKeyState('B'))
	{
		m_eCurState = BUILD;
	}
	if (GetAsyncKeyState('N'))
	{
		m_eCurState = FALLDOWN;
		m_ePlayerLookAt = LOOK_DOWN;
	}
	if (GetAsyncKeyState('M'))
	{
		m_eCurState = WAKEUP;
		m_ePlayerLookAt = LOOK_DOWN;
	}
	if (GetAsyncKeyState('H'))
	{
		m_eCurState = EAT;
		m_ePlayerLookAt = LOOK_DOWN;
	}

	//enum PLAYERSTATE {
	//	IDLE, MOVE, BUILD, PICKUP, HIT, ATTACK, FALLDOWN, WAKEUP, EAT, STATE_END
	//};

	//if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	//{
	//	_vec3	vPickPos = Picking_OnTerrain();
	//
	//	m_pTransformCom->Move_Terrain(&vPickPos, fTimeDelta, 5.f);
	//}
	
}

void CPlayer::Height_OnTerrain()
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"Terrain", L"Proto_TerrainTex"));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.5f, vPos.z);
}

_vec3 CPlayer::Picking_OnTerrain()
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"Terrain", L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*			pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"GameLogic", L"Terrain", L"Proto_Transform"));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}
void CPlayer::BillBoard()
{
	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

}
void CPlayer::Check_State()
{
	if (m_ePreState != m_eCurState)
	{
		/*switch (m_eCurState)
		{
		case IDLE_DOWN:
	
			break;
		case MOVE_DOWN:
			m_ePlayerLookAt = LOOK_DOWN;
			m_fFrameEnd = 6;
			m_cTex = L"Proto_Player_Unarmed_run_down";
			break;
		case MOVE_UP:
			m_ePlayerLookAt = LOOK_UP;
			m_fFrameEnd = 6;
			m_cTex = L"Proto_Player_Unarmed_run_up";
			break;
		case MOVE_RIGHT:
			m_ePlayerLookAt = LOOK_RIGHT;
			m_fFrameEnd = 6;
			m_cTex = L"Proto_Player_Unarmed_run_side";
			break;
		case MOVE_LEFT:
			m_ePlayerLookAt = LOOK_LEFT;
			m_fFrameEnd = 6;
			m_cTex = L"Proto_Player_Unarmed_run_side";
			break;

		}*/
		/*enum PLAYERSTATE {
		IDLE_DOWN, IDLE_UP, IDLE_SIDE, MOVE_DOWN, MOVE_UP, MOVE_SIDE,
		BUILD_DOWN, BUILD_UP, BUILD_SIDE, PICKUP_UP, PICKUP_DOWN, PICKUP_SIDE,
		HIT_DOWN, HIT_UP, HIT_SIDE, ATTACK_DOWN, ATTACK_UP, ATTACK_SIDE,
		FALLDOWN, WAKEUP, EAT,
		STATE_END
	};*/
		if (m_eCurState == IDLE)
		{
			m_fFrameEnd = 22;
		}
		else if(m_eCurState == HIT)
		{
			m_fFrameEnd = 7;
		}
		else if (m_eCurState == ATTACK)
		{
			m_fFrameEnd = 11;
		}
		else if (m_eCurState == FALLDOWN)
		{
			m_fFrameEnd = 8;
		}
		else if (m_eCurState == WAKEUP)
		{
			m_fFrameEnd = 32;
		}
		else if (m_eCurState == EAT)
		{
			m_fFrameEnd = 36;
		}
		else
		{
			m_fFrameEnd = 6;
		}
		/*Change_Texture();*/
		m_ePreState = m_eCurState;
		m_fFrame = 0.f;
	}

}


void CPlayer::Set_Scale()
{
	if (m_eCurState == BUILD )
	{
		m_pTransformCom->m_vScale = { 1.5f, 1.0f,1.0f };
	}
	else if ((m_ePlayerLookAt == LOOK_LEFT || m_ePlayerLookAt == LOOK_RIGHT) && m_eCurState == PICKUP)
	{
		m_pTransformCom->m_vScale = { 1.5f, 1.0f,1.0f };
	}
	else if ((m_ePlayerLookAt == LOOK_LEFT || m_ePlayerLookAt == LOOK_RIGHT)&& m_eCurState == ATTACK)
	{
		m_pTransformCom->m_vScale = { 1.5f, 1.5f,1.0f };
	}
	else if (m_eCurState == BUILD || m_eCurState == HIT)
	{
		m_pTransformCom->m_vScale = { 1.5f, 1.5f,1.0f };
	}
	else if (m_eCurState == FALLDOWN)
	{
		m_pTransformCom->m_vScale = { 1.5f, 5.0f,1.0f };
	}
	else if (m_eCurState == WAKEUP)
	{
		m_pTransformCom->m_vScale = { 2.f, 2.0f,1.0f };
	}
	else if (m_eCurState == EAT)
	{
		m_pTransformCom->m_vScale = { 2.f, 1.5f,1.0f };
	}
	else
	{
		m_pTransformCom->m_vScale = { 1.0f, 1.0f,1.0f };
	}

}


