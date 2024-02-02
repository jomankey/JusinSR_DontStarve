#include "stdafx.h"
#include "Pig.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "Scene.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
	:CMonster(pGraphicDev, _vPos), m_eCurState(HAPPY), m_ePreState(STATE_END)
{
}

CPig::CPig(const CPig& rhs)
	:CMonster(rhs)
{
}

CPig::~CPig()
{
}

HRESULT CPig::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(m_vPos);
	Set_ObjStat();
	/*m_pTransForm->m_vScale = { 1.f, 1.f, 1.f };*/
	m_fFrameEnd = 17;

	return S_OK;
}

_int CPig::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
		m_fFrame = 0.f;

	CGameObject::Update_GameObject(fTimeDelta);
	State_Change();
	Look_Change(); // ÀÓÀÇ·Î
	/*Player_Chase(fTimeDelta);*/
	m_pTransForm->m_vScale = { 1.5f, 1.5f, 1.f };
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CPig::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	m_pTransForm->BillBoard();
}

void CPig::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	/* Set_Scale();*/

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

HRESULT CPig::Add_Component()
{
	CComponent* pComponent = nullptr;
	//d
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM
	 pComponent = m_pTextureCom[LOOK_DOWN][HAPPY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Pig_happy"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_MultiMap[ID_STATIC].insert({ L"Proto_Pig_happy", pComponent });

	 /*pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_up"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_up", pComponent });

	 pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });

	 pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
	 NULL_CHECK_RETURN(pComponent, E_FAIL);
	 m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });*/
#pragma endregion TEXCOM
	

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
	return S_OK;
}


void CPig::Set_ObjStat()
{
	m_Stat.fHP = 100.f;
	m_Stat.fMxHP = 100.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.fATK = 10.f;
	m_Stat.bDead = false;
}

void CPig::Player_Chase(const _float& fTimeDelta)
{
	_vec3 PlayerPos;
	PlayerPos = Get_Player_Pos();
	m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);
	Look_Change();
}

void CPig::State_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case WALK:
			m_fFrameEnd = 20;
			break;

		case HAPPY:
			m_fFrameEnd = 17;
		}

		m_ePreState = m_eCurState;

	}
}

CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CPig* pInstance = new CPig(pGraphicDev, _vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Beefalo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPig::Free()
{
	CMonster::Free();
}
