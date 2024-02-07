#include "stdafx.h"
#include "DeerClops.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Scene.h"
#include "Player.h"

CDeerClops::CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos), m_eCurLook(LOOK_DOWN), m_ePreLook(LOOK_END),
	m_eCurState(SLEEP), m_ePreState(STATE_END), m_Dirchange(false), m_fAcctime(0.f),
	m_fFrameEnd(0.f), m_bFrameStop(false), m_bAttacking(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
	
}

CDeerClops::CDeerClops(const CDeerClops& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_vPos(rhs.m_vPos), m_Stat(rhs.m_Stat), m_eCurLook(rhs.m_eCurLook), m_ePreLook(rhs.m_ePreLook), m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState), m_Dirchange(rhs.m_Dirchange),
	m_fAcctime(rhs.m_fAcctime), m_fFrameEnd(rhs.m_fFrameEnd),
	m_bFrameStop(rhs.m_bFrameStop), m_bAttacking(rhs.m_bAttacking)
	
{
}

CDeerClops::~CDeerClops()
{

}

HRESULT CDeerClops::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(m_vPos);

	m_fAcctime = float(rand() % 30);
	for (auto i = 0; i < DEER_PHASE::PHASE_END; ++i)
	{
		m_bPhase[i] = false;
	}

	return S_OK;
}

_int CDeerClops::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);
	if (m_bPhase[FIRST])
	{
		if (m_bPhase[SECOND])
		{
			Second_Phase(fTimeDelta);
		}
		else
		{
			First_Phase(fTimeDelta);
		}
	}
	else
	{
		Sleep(fTimeDelta);
	}
	
	if (KEY_TAP(DIK_9))
	{
		Set_WakeUp();
	}


	Check_State();
	Look_Change();
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CDeerClops::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	m_pTransForm->BillBoard();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
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

void CDeerClops::Set_ObjStat()
{
	m_Stat.fHP = 100.f;
	m_Stat.fMxHP = 100.f;
	m_Stat.fSpeed = 2.f;
	m_Stat.fATK = 50.f;
	m_Stat.fATKRange = 4.f;
	m_Stat.fAggroRange = 10.f;
}

void CDeerClops::Check_State()
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

_bool CDeerClops::IsTarget_Approach(float _fDistance)
{
	_vec3 vTargetPos, vPos, vDir;
	vTargetPos = Player_Position();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	vTargetPos.y = 0.f;
	vPos.y = 0.f;
	if (D3DXVec3Length(&(vTargetPos - vPos)) < _fDistance)
	{
		return true;
	}
	else
	{
		return false;
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
	
	if (IsTarget_Approach(m_Stat.fATKRange))
	{
		m_eCurState = ATTACK;
	}
	else if (m_eCurState == ATTACK && IsTarget_Approach(m_Stat.fATKRange + 1.f)&& !m_bAttacking)
	{
		if (12 < m_fFrame)
		{
			dynamic_cast<CPlayer*>(Player_Pointer())->Set_Attack(m_Stat.fATK);
			m_bAttacking = true;
		}
	}
	else
	{
		Chase_Player(fTimeDelta);
		m_eCurState = WALK;
	}

	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
	}

}

void CDeerClops::Chase_Player(const _float& fTimeDelta)
{
	_vec3 PlayerPos, vDir, vPos;
	PlayerComponent()->Get_Info(INFO_POS, &PlayerPos);
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	vDir = PlayerPos - vPos;
	vDir.y = 0;
	m_eCurLook = m_pTransForm->For_Player_Direction(&vDir, m_Stat.fSpeed, fTimeDelta);
}

CTransform* CDeerClops::PlayerComponent()
{
	Engine::CTransform* pPlayerTransformCom = scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm();
	NULL_CHECK_RETURN(pPlayerTransformCom, NULL);
	return pPlayerTransformCom;
}
_vec3 CDeerClops::Player_Position()
{
	_vec3	pPos;
	PlayerComponent()->Get_Info(INFO_POS, &pPos);
	return pPos;
}
CGameObject* CDeerClops::Player_Pointer()
{
	return scenemgr::Get_CurScene()->GetPlayerObject();
}

void CDeerClops::Look_Change()
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

void CDeerClops::Free()
{
	CGameObject::Free();
}

CDeerClops* CDeerClops::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CDeerClops* pInstance = new CDeerClops(pGraphicDev, _vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}
