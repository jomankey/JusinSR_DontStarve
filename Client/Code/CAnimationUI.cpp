#include "stdafx.h"
#include "CAnimationUI.h"
#include "Export_Utility.h"


CAnimationUI::CAnimationUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pReverseCom(nullptr)
	, m_pAnimCom(nullptr)
{
}

CAnimationUI::CAnimationUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strObjName)
	:CGameObject(pGraphicDev, _strObjName)
	, m_pBufferCom(nullptr)
	, m_pReverseCom(nullptr)
	, m_pAnimCom(nullptr)
{
}

CAnimationUI::~CAnimationUI()
{
}

HRESULT CAnimationUI::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CAnimationUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	m_pAnimCom->SetAnimTexture();
	m_pBufferCom->Render_Buffer();


	scenemgr::Get_CurScene()->EndOrtho();
}

//HRESULT CAnimationUI::Add_Component()
//{
//	CComponent* pComponent = nullptr;
//	_vec3 vPos;
//
//	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
//
//	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
//
//	pComponent = m_pAnimCom = dynamic_cast<CAnimator*>(proto::Clone_Proto(L"Proto_Anim"));
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Anim", pComponent });
//
//	m_pAnimCom->AddAnimation(L"HIDE", proto::Clone_ProtoAnim(L"TRAP_SPIKE_HIDE"));//숨기
//	m_pAnimCom->AddAnimation(L"GROW", proto::Clone_ProtoAnim(L"TRAP_SPIKE_GROW"));//나타나기
//	m_pAnimCom->AddAnimation(L"IDLE", proto::Clone_ProtoAnim(L"TRAP_SPIKE_IDLE"));//나타난상태지속
//
//
//	m_pAnimCom->SetCurAnimation(L"HIDE");
//	m_pAnimCom->SetCurAnimationFrame(L"HIDE", m_pAnimCom->GetAnimMaxFrame(L"HIDE"));
//
//
//	m_pTransForm->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
//
//	return S_OK;
//}

void CAnimationUI::Free()
{
	__super::Free();
}


