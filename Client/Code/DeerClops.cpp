#include "..\Include\stdafx.h"
#include "DeerClops.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Scene.h"

CDeerClops::CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos), m_eCurLook(LOOK_DOWN), m_ePreLook(LOOK_END),
	m_eCurState(IDLE), m_ePreState(STATE_END), m_Dirchange(false), m_fAcctime(0.f), m_fFrameEnd(0.f), m_bMode(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CDeerClops::CDeerClops(const CDeerClops& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_vPos(rhs.m_vPos), m_Stat(rhs.m_Stat), m_eCurLook(rhs.m_eCurLook), m_ePreLook(rhs.m_ePreLook), m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState), m_Dirchange(rhs.m_Dirchange), m_fAcctime(rhs.m_fAcctime), m_fFrameEnd(rhs.m_fFrameEnd),
	m_bMode(rhs.m_bMode)
{
}

CDeerClops::~CDeerClops()
{

}

HRESULT CDeerClops::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(m_vPos);
	m_pTransForm->Set_Scale(_vec3(3.f, 3.f, 1.f));
	m_fAcctime = float(rand() % 30);
	return S_OK;
}

_int CDeerClops::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);
	Mode_Change();	//플레이어 발견시
	if (m_bMode)
	{
		Fight(fTimeDelta);
	}
	else
	{
		Patroll(fTimeDelta);
	}
	
	Check_State();
	this->Look_Change();
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CDeerClops::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
}

void CDeerClops::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);
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
	/*Deer_Idle_down*/
	//texture com

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

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
	return S_OK;
}

void CDeerClops::BillBoard()
{
	_matrix	matWorld, matView, matBill;

	m_pTransForm->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransForm->Set_WorldMatrix(&(matBill * matWorld));
}

void CDeerClops::Set_ObjStat()
{
	m_Stat.fHP = 100.f;
	m_Stat.fMxHP = 100.f;
	m_Stat.fSpeed = 1.f;
}

void CDeerClops::Check_State()
{
	/*enum DEERSTATE { IDLE, WALK, ATTACK, SLEEP, TAUNT, HIT, DEAD, STATE_END };*/
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
			break;
		case TAUNT:
			m_fFrameEnd = 17;
			break;
		case HIT:
			m_fFrameEnd = 20;
			break;
		case DEAD:
			m_fFrameEnd = 24;
			break;
		}

		m_ePreState = m_eCurState;

	}
}

void CDeerClops::Mode_Change()
{
	_vec3 vPlayerPos, vThisPos,vDir;
	PlayerComponent()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransForm->Get_Info(INFO_POS, &vThisPos);
	vDir = vPlayerPos - vThisPos;

	if (D3DXVec3Length(&vDir) < 5) 
	{
		if (!m_bMode)//발동, 최초발견시에
		{
			m_bMode = true;
			m_eCurState = TAUNT;
		}
	}
	else
	{
		m_bMode = false;
	}
	
}

void CDeerClops::Patroll(const _float& fTimeDelta)
{

	if (m_fFrameEnd < m_fFrame)
		m_fFrame = 0.f;

	m_fFrameChange += m_fAcctime * fTimeDelta;
	if (m_fAcctime < m_fFrameChange)
	{
		m_fFrameChange = 0.f;
		m_fAcctime = rand() % 30;

		if (m_eCurState == IDLE)
		{
			m_eCurState = WALK;
		}
		else
		{
			m_eCurState == IDLE;
		}
	}
	else
	{
		return;
	}
	
	
}

void CDeerClops::Fight(const _float& fTimeDelta)
{
	if (m_ePreState == TAUNT)	//포효 상태에선 리턴
	{
		if (m_fFrameEnd <= m_fFrame)
		{
			m_eCurState = WALK;
			m_fFrame = 0;
		}
		return;
	}
	else                       //플레이어 향한 방향을 바라보는 메소드 추가해야함
	{
		if (m_fFrameEnd < m_fFrame)
		{
			Chase_Player(fTimeDelta);
			m_fFrame = 0;
		}
	}

}

void CDeerClops::Chase_Player(const _float& fTimeDelta)
{
	_vec3 vPlayerPos, vThisPos, vDir;
	PlayerComponent()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransForm->Get_Info(INFO_POS, &vThisPos);
	vDir = vPlayerPos - vThisPos;
	if (D3DXVec3Length(&vDir) < 1)
	{
		m_eCurState = ATTACK;
	}
	else
	{
		m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
		m_eCurState = WALK;
	}

}




CTransform* CDeerClops::PlayerComponent()
{
	Engine::CTransform* pPlayerTransformCom = scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm();
	NULL_CHECK_RETURN(pPlayerTransformCom, NULL);
	return pPlayerTransformCom;
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
