#include "stdafx.h"
#include "CCatapult.h"

#include "Export_System.h"

CCatapult::CCatapult(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCatapult::CCatapult(const CCatapult& rhs)
	: CGameObject(rhs.m_pGraphicDev)
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
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	if (KEY_TAP(DIK_L))
	{
		m_pAnimCom->ChangeAnimation(L"IDLE_LEFT");
	}
	return 0;
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

	m_pAnimCom->CreateAnimation(m_pGraphicDev, L"IDLE_LEFT", L"../Bin/Resource/Texture/Player/UnArmed/idle_up/idle_up__%03d.png", 22, 0.05f);
	m_pAnimCom->CreateAnimation(m_pGraphicDev, L"IDLE_DOWN", L"../Bin/Resource/Texture/Player/UnArmed/idle_down/idle_down__%03d.png", 22, 0.05f);
	m_pAnimCom->ChangeAnimation(L"IDLE_DOWN");


	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));

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


