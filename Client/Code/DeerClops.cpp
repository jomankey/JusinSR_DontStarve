#include "stdafx.h"
#include "DeerClops.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Scene.h"
#include "Player.h"
#include "BossEftDown.h"
#include "Layer.h"
#include "Circle.h"
#include "FallMark.h"
CDeerClops::CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	: CMonster(pGraphicDev, _vPos),
	m_eCurState(SLEEP), m_ePreState(STATE_END)
{
}

CDeerClops::CDeerClops(const CDeerClops& rhs)
	:CMonster(rhs)
{
}

CDeerClops::~CDeerClops()
{

}

HRESULT CDeerClops::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(m_vPos);
	Set_ObjStat();
	m_fAcctime = float(rand() % 30);
	for (auto i = 0; i < DEER_PHASE::PHASE_END; ++i)
	{
		m_bPhase[i] = false;
	}

	return S_OK;
}

_int CDeerClops::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bFrameStop)//
	{
		m_fFrame += m_fFrameEnd * fTimeDelta;
	}

	if (m_bPhase[FIRST]) //m_bPhase 불 변수의 첫번째 인덱스부터 true로 켜질때마다 페이즈가 변경됨
	{
		if (m_bPhase[SECOND])		//First Phase 에서 Second Phase 는 자동으로 연결됨
		{
			if (m_bPhase[THIRD])
			{
				if (m_bPhase[DIE])
					Boss_Die(fTimeDelta);
				else
					Third_Phase(fTimeDelta);
			}
			else
				Second_Phase(fTimeDelta);
		}
		else
			First_Phase(fTimeDelta);
	}
	else
		Sleep(fTimeDelta);
	
	if (KEY_TAP(DIK_9))
	{
		Set_WakeUp();
	}

	if (KEY_TAP(DIK_6))
	{
		Set_Hit();
		m_Stat.fHP -= 50;
	}

	State_Change();
	Look_Change();
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CDeerClops::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	m_pTransForm->BillBoard();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	CGameObject::Compute_ViewZ(&vPos);
}

void CDeerClops::Render_GameObject()
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

void CDeerClops::Set_WakeUp()
{
	m_bPhase[FIRST] = true;
	m_eCurState = SLEEP_PST;
}

HRESULT CDeerClops::Add_Component()
{
	CComponent* pComponent = nullptr;
	//d
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });


#pragma region TEXCOM

	//IDLE
	pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Idle_side", pComponent });


	//WALK
	pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_walk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_walk_side", pComponent });

	//ATTACK
	pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_Atk_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_Atk_side", pComponent });

	//HIT
	pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_hit_side", pComponent });

	//Sleep Loop
	pComponent = m_pTextureCom[LOOK_DOWN][SLEEP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_sleep_loop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_sleep_loop", pComponent });

	//Sleep Pst
	pComponent = m_pTextureCom[LOOK_DOWN][SLEEP_PST] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_sleep_pst"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_sleep_pst", pComponent });

	//Taunt
	pComponent = m_pTextureCom[LOOK_DOWN][TAUNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_taunt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_taunt", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][LONG_TAUNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_long_taunt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_long_taunt", pComponent });

	//DEAD
	pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Deer_dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Deer_dead", pComponent });
#pragma endregion TEXCOM
	


	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale({ 5.f, 5.f, 5.f });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
	return S_OK;
}

void CDeerClops::Set_Hit()
{
	if (m_Stat.fHP > 0)
	{
		if (m_ePreState != ATTACK)
		{
			m_eCurState = HIT;
			m_bHit = true;
		}
		
		



		if (m_Stat.fHP < 250.f && !m_bPhase[THIRD])
		{
			m_bPhase[THIRD] = true;
			m_eCurState = LONG_TAUNT;
			_vec3 vThisPos;
			m_pTransForm->Get_Info(INFO_POS, &vThisPos);
			CGameObject* pGameObject = CCircle::Create(m_pGraphicDev, vThisPos);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC)->AddGameObject(eOBJECT_GROUPTYPE::EFFECT, pGameObject), );
			dynamic_cast<CCircle*>(pGameObject)->Set_Count(2);
			m_fAcctime = 0.f;
		}
		else
			return;
	}
}

void CDeerClops::Set_ObjStat()
{
	m_Stat.strObjName = L"외눈사슴";
	m_Stat.fHP = 100.f;
	m_Stat.fMxHP = 100.f;
	m_Stat.fSpeed = 2.f;
	m_Stat.fATK = 50.f;
	m_Stat.fATKRange = 4.f;
	m_Stat.fAggroRange = 10.f;
	m_Stat.bDead = false;
}

void CDeerClops::State_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_fFrameEnd = 19;
			break;
		case WALK:
			m_fFrameEnd = 16;
			break;
		case ATTACK:
			m_fFrameEnd = 20;
			break;
		case SLEEP:
			m_fFrameEnd = 23;
			m_eCurLook = LOOK_DOWN;
			break;
		case SLEEP_PST:
			m_fFrameEnd = 18;
			m_eCurLook = LOOK_DOWN;
			break;
		case TAUNT:
			m_fFrameEnd = 17;
			m_eCurLook = LOOK_DOWN;
			break;
		case LONG_TAUNT:
			m_fFrameEnd = 33;
			m_eCurLook = LOOK_DOWN;
			break;
		case HIT:
			m_fFrameEnd = 20;
			break;
		case DEAD:
			m_fFrameEnd = 24;
			m_eCurLook = LOOK_DOWN;
			break;
		}
		m_fFrame = 0.f;
		m_ePreState = m_eCurState;
	}
}


void CDeerClops::Sleep(const _float& fTimeDelta)
{
	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}

void CDeerClops::First_Phase(const _float& fTimeDelta)
{
	
	if (m_ePreState == SLEEP_PST && m_fFrameEnd < m_fFrame)
	{
		m_eCurState = IDLE;
	}
	else if (m_ePreState == IDLE && m_fFrameEnd < m_fFrame)	//포효 지르기
	{
		m_eCurState = TAUNT;
		_vec3 vThisPos;
		m_pTransForm->Get_Info(INFO_POS, &vThisPos);
		CGameObject* pGameObject = CCircle::Create(m_pGraphicDev, vThisPos);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(scenemgr::Get_CurScene()->GetLayer(eLAYER_TYPE::GAME_LOGIC)->AddGameObject(eOBJECT_GROUPTYPE::EFFECT, pGameObject), );

	}
	else if (m_ePreState == TAUNT && m_fFrameEnd < m_fFrame)
	{
		m_eCurState = WALK;
		m_bPhase[SECOND] = true;
	}



	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}
void CDeerClops::Second_Phase(const _float& fTimeDelta)
{
	m_fAcctime += fTimeDelta;
	


	if (0.5f < m_fAcctime)
	{
		m_fAcctime = 0.f;
		if (m_bAttackCooltime)
			m_bAttackCooltime = false;

		_vec3 pPos = Get_Player_Pos();
		
		CGameObject* pGameObject = FallMark::Create(m_pGraphicDev, pPos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
	}

	if (IsTarget_Approach(m_Stat.fATKRange) && !m_bAttackCooltime)
	{
		m_eCurState = ATTACK;
		m_bAttackCooltime = true;
	}
	else if (IsTarget_Approach(m_Stat.fATKRange) && m_bAttackCooltime)
	{
		if (m_fFrameEnd < m_fFrame)
		{
			m_eCurState = IDLE;
		}
	}
	else if (m_ePreState == IDLE)
	{
		if (!IsTarget_Approach(m_Stat.fATKRange))
		{
			m_eCurState = WALK;
		}
	}
	else if (m_ePreState == ATTACK)
	{
		if (9 < m_fFrame && !m_bAttacking)
		{
			_vec3 vThisPos; // 이펙트 생성 기점.
			m_pTransForm->Get_Info(INFO_POS, &vThisPos);

			CGameObject* pGameObject = CBossEftDown::Create(m_pGraphicDev, vThisPos);
			NULL_CHECK_RETURN(pGameObject, );
			CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::EFFECT, pGameObject);
			dynamic_cast<CBossEftDown*>(pGameObject)->Set_Look_Dir(m_ePreLook);
			m_bAttacking = true;
		}

		if (!IsTarget_Approach(m_Stat.fATKRange))
		{
			if (m_fFrameEnd < m_fFrame)
			{
				m_eCurState = WALK;
			}
		}
	}
	else if(m_ePreState == WALK)
	{
		Player_Chase(fTimeDelta);
	}

	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
		if (m_bAttacking)
			m_bAttacking = false;
	}

}

void CDeerClops::Third_Phase(const _float& fTimeDelta) //아직 제작중
{
	m_Stat.fSpeed = 3.5f;

	m_fAcctime += fTimeDelta;

	if (m_ePreState == LONG_TAUNT)
	{

	}



	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
		if (m_bAttacking)
			m_bAttacking = false;
	}


}

void CDeerClops::Boss_Die(const _float& fTimeDelta)
{
}

void CDeerClops::Free()
{
	__super::Free();
}

CDeerClops* CDeerClops::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CDeerClops* pInstance = new CDeerClops(pGraphicDev, _vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Boss Create Failed");
		return nullptr;
	}

	return pInstance;
}
