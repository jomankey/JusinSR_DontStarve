#include "stdafx.h"
#include "CCatapult.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "CTumbleWeed.h"


CCatapult::CCatapult(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pAnimCom(nullptr)
	, m_eCurState(eTRAP_STATE::NONE)
	, m_fAccTime(0.f)
	, m_fAtkTime(1.f)
{
}

CCatapult::CCatapult(const CCatapult& rhs)
	: CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pAnimCom(nullptr)
{
}

CCatapult::~CCatapult()
{
}

HRESULT CCatapult::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

_int CCatapult::Update_GameObject(const _float& fTimeDelta)
{


	if (m_eCurState == eTRAP_STATE::PLACE && m_pAnimCom->IsFinish(L"PLACE"))
	{
		m_eCurState = eTRAP_STATE::IDLE;
		m_pAnimCom->ChangeAnimation(L"IDLE_DOWN");
		m_pTransForm->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
		m_pAnimCom->SetLoopAnimator(true);
	}

	if (m_eCurState == eTRAP_STATE::IDLE)
	{
		m_fAccTime += fTimeDelta;
		if (m_fAtkTime <= m_fAccTime)
		{
			m_pAnimCom->ChangeAnimation(L"ATK_DOWN");
			m_pAnimCom->SetLoopAnimator(false);
			m_pTransForm->Set_Scale(_vec3(2.f, 2.f, 2.f));
			m_eCurState = eTRAP_STATE::ATK;
		}
	}

	if (m_pAnimCom->GetAnimFrame(L"ATK_DOWN") == 15)
	{
		//createObject
		_vec3 vPos;
		m_pTransForm->Get_Info(INFO_POS, &vPos);

		auto pGameObject =  CTumbleWeed::Create(m_pGraphicDev, L"TRAP_TUMBLE", vPos);
		CreateObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::TRAP, pGameObject);
	}

	if (m_eCurState == eTRAP_STATE::ATK && m_pAnimCom->IsFinish(L"ATK_DOWN"))//공격끝나면
	{
		m_pAnimCom->ChangeAnimation(L"IDLE_DOWN");
		m_pAnimCom->SetLoopAnimator(true);
		m_fAccTime = 0.f;
		m_pTransForm->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
		m_eCurState = eTRAP_STATE::IDLE;
	}



	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 	CGameObject::Update_GameObject(fTimeDelta);
}

void CCatapult::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CCatapult::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pAnimCom->SetAnimTexture();
	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CCatapult::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pAnimCom = dynamic_cast<CAnimator*>(proto::Clone_Proto(L"Proto_Anim"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Anim", pComponent });





	m_pAnimCom->AddAnimation(L"IDLE_DOWN", proto::Clone_ProtoAnim(L"CATAPULT_IDLE_DOWN"));
	m_pAnimCom->AddAnimation(L"ATK_DOWN", proto::Clone_ProtoAnim(L"CATAPULT_ATK_DOWN"));
	m_pAnimCom->AddAnimation(L"PLACE", proto::Clone_ProtoAnim(L"CATAPULT_PLACE"));
	m_pAnimCom->AddAnimation(L"DEATH", proto::Clone_ProtoAnim(L"CATAPULT_DEATH"));


	m_pAnimCom->SetCurAnimation(L"PLACE");
	m_eCurState = eTRAP_STATE::PLACE;


	m_pTransForm->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));

	return S_OK;
}



CGameObject* CCatapult::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameObject* pInstance = new CCatapult(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CCatapult::Free()
{
	__super::Free();
}


