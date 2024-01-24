#include "..\Include\stdafx.h"
#include "..\Header\Player.h"

#include "Export_System.h"
#include "Export_Utility.h"

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

	//m_pTransformCom->m_vScale = { 2.f, 1.f, 1.f };
	m_eCurState = IDLE;
	m_ePreState = STATE_END;
	m_ePlayerLookAt = LOOK_DOWN;
	m_cTex = nullptr;
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
	BillBoard();
	Height_OnTerrain();
}

void CPlayer::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	
	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
	
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	/*for (size_t i = 0; i < PLAYERLOOK::LOOK_END; i++)
	{
		pComponent = m_pTextureCom[i] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"PLAYE_%d",i));

	}
	m_pTextureCom[PLAYERLOOK::LOOK_DOWN0]*/
	pComponent = m_pTextureCom/*[PLAYERLOOK::LOOK_DOWN]*/ = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Unarmed_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PlayerTexture", pComponent });


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pStatusCom = dynamic_cast<CPlayerStatus*>(Engine::Clone_Proto(L"Proto_PlayerStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PlayerStat", pComponent });

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
	//enum PLAYERSTATE {
	//	IDLE, MOVE_DOWN, MOVE_UP, MOVE_RIGHT, MOVE_LEFT,  //플레이어 행동상태
	//	ATTACK, USINGTOOL, DIALOG, SLEEP, HIT, DEAD, STATE_END
	//};
	_vec3		vDir,vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	if (GetAsyncKeyState('W'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, 10.f, fTimeDelta);
		m_eCurState = MOVE_UP;
		m_cTex = L"Proto_Player_Unarmed_run_up";
	}

	if (GetAsyncKeyState('S'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, -10.f, fTimeDelta);
		m_eCurState = MOVE_DOWN;
		m_cTex = L"Proto_Player_Unarmed_run_down";
	}

	if (GetAsyncKeyState('A'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransformCom->Move_Pos(&vRight, -10.f, fTimeDelta);
		m_eCurState = MOVE_LEFT;
		m_cTex = L"Proto_Player_Unarmed_run_side";
		/*m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90.f * fTimeDelta));*/
	}

	if (GetAsyncKeyState('D'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransformCom->Move_Pos(&vRight, +10.f, fTimeDelta);
		m_eCurState = MOVE_RIGHT;
		m_cTex = L"Proto_Player_Unarmed_run_side";
		/*m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-90.f * fTimeDelta));*/
	}

	if (!GetAsyncKeyState('W') &&
		!GetAsyncKeyState('A') &&
		!GetAsyncKeyState('S') &&
		!GetAsyncKeyState('D'))
	{
		m_eCurState = IDLE;
	}

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		_vec3	vPickPos = Picking_OnTerrain();

		m_pTransformCom->Move_Terrain(&vPickPos, fTimeDelta, 5.f);
	}
	
}

void CPlayer::Height_OnTerrain()
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"Terrain", L"Proto_TerrainTex"));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
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
		switch (m_eCurState)
		{
		case IDLE:
			Adjust_IDLE();
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

		}
		Change_Texture();
		m_ePreState = m_eCurState;
	}

}

void CPlayer::Adjust_IDLE()
{
	switch (m_ePlayerLookAt)
	{
	case LOOK_DOWN:
		m_fFrameEnd = 22;
		m_cTex = L"Proto_Player_Unarmed_idle_down";
		break;
	case LOOK_UP:
		m_fFrameEnd = 22;
		m_cTex = L"Proto_Player_Unarmed_idle_up";
		break;
	case LOOK_RIGHT:
		m_fFrameEnd = 23;
		m_cTex = L"Proto_Player_Unarmed_idle_side";
		break;
	case LOOK_LEFT:
		m_fFrameEnd = 23;
		m_cTex = L"Proto_Player_Unarmed_idle_side";
		break;
	}
}

void CPlayer::Change_Texture()
{
	m_pTextureCom->Free();
	m_fFrame = 0.f;
	m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_cTex));

}
