#include "stdafx.h"
#include "..\Header\Effect.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "Player.h"
#include "ResObject.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos)
	: Engine::CGameObject(pGraphicDev)
	, m_vPos(vPos)
	, m_fFrameEnd(0.f)
	, m_fAcctime(0.f)
	, m_bFrameStop(false)
	, m_Dirchange(false)
	, m_eCurLook(LOOK_DOWN)
	, m_ePreLook(LOOK_END)
	, m_fDamage(0.f)
{
}

CEffect::CEffect(const CEffect& rhs)
	: Engine::CGameObject(rhs)
	, m_vPos(rhs.m_vPos)
	, m_fFrameEnd(rhs.m_fFrameEnd)
	, m_fAcctime(rhs.m_fAcctime)
	, m_bFrameStop(rhs.m_bFrameStop)
	, m_Dirchange(rhs.m_Dirchange)
	, m_eCurLook(rhs.m_eCurLook)
	, m_ePreLook(rhs.m_ePreLook)
	, m_fDamage(rhs.m_fDamage)
{

}

CEffect::~CEffect()
{
}

void CEffect::Look_Change()
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

void CEffect::Check_Collision()
{
	auto iter = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	for (auto find : iter)
	{
		if (find != nullptr)
		{
			if (Collision_Transform(this->m_pTransForm, find->GetTransForm()))
			{
				dynamic_cast<CMonster*>(find)->Set_Attack(m_fDamage);
			}
		}
	}

	auto obj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::RESOURCE_OBJECT);
	for (auto find : obj)
	{
		if (find != nullptr)
		{
			if (Collision_Transform(this->m_pTransForm, find->GetTransForm()))
			{
				dynamic_cast<CResObject*>(find)->Set_Attack();
			}
		}
	}

	auto player = scenemgr::Get_CurScene()->GetPlayerObject();
	if (player != nullptr)
	{
		if (Collision_Transform(this->m_pTransForm, player->GetTransForm()))
		{
			dynamic_cast<CPlayer*>(player)->Set_Attack((int)m_fDamage);
		}
	}
}





void CEffect::Free()
{	
	__super::Free();
}




