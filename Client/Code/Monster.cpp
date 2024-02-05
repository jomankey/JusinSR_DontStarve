#include "stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "Scene.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos)
	, m_Dirchange(false)
	, m_eCurLook(LOOK_DOWN)
	, m_ePreLook(LOOK_END)
	, m_fAcctime(0.f)
	, m_vDir(0.f, 0.f, 0.f)
	, m_Attacked(false)
	, m_bFrameStop(false)
	, m_bHit(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs)
	, m_vPos(rhs.m_vPos)
	, m_Stat(rhs.m_Stat)
	, m_Dirchange(rhs.m_Dirchange)
	, m_eCurLook(rhs.m_eCurLook)
	, m_ePreLook(rhs.m_ePreLook)
	, m_fAcctime(rhs.m_fAcctime)
	, m_vDir(0.f, 0.f, 0.f)
	, m_Attacked(rhs.m_Attacked)
	, m_bFrameStop(rhs.m_bFrameStop)
	, m_bHit(rhs.m_bHit)

{
}

CMonster::~CMonster()
{
}

void CMonster::Set_Attack(_float _fAttack)
{
	if (!m_Stat.bDead)
	{
		m_Stat.fHP -= _fAttack;
		if (!m_Attacked)
		{
			m_Attacked = true;		
		}
		Set_Hit();
	}
	else
		return;
}

void CMonster::Free()
{
	__super::Free();
}

void CMonster::Player_Chase(const _float& fTimeDelta)
{
	_vec3 PlayerPos;
	PlayerPos = Get_Player_Pos();
	m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);
}

_bool CMonster::IsTarget_Approach(float _fDistance)
{
	_vec3 vTargetPos, vPos,vDir;
	vTargetPos = Get_Player_Pos();
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

void CMonster::Look_Change()
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

_vec3 CMonster::Get_Player_Pos()
{
	auto pPlayer = dynamic_cast<CScene*>(scenemgr::Get_CurScene())->GetPlayerObject();
	Engine::CTransform* pPlayerTransformCom = pPlayer->GetTransForm();
	NULL_CHECK_RETURN(pPlayerTransformCom, _vec3());

	_vec3 PlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &PlayerPos);

	return PlayerPos;
}

