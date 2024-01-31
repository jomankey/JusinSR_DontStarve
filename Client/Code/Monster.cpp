#include "..\Include\stdafx.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "Scene.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos), m_Dirchange(false),
	m_eCurLook(LOOK_DOWN), m_ePreLook(LOOK_END), m_fAcctime(0.f)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs), m_vPos(rhs.m_vPos), m_Stat(rhs.m_Stat), m_Dirchange(rhs.m_Dirchange),
	m_eCurLook(rhs.m_eCurLook), m_ePreLook(rhs.m_ePreLook), m_fAcctime(rhs.m_fAcctime)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CMonster::LateUpdate_GameObject()
{
}

void CMonster::Render_GameObject()
{

}

HRESULT CMonster::Add_Component()
{

	return S_OK;
}

void CMonster::Free()
{
	CGameObject::Free();
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

