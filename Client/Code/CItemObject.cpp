#include "..\Include\stdafx.h"
#include "CItemObject.h"

#include "Export_System.h"
#include "Export_Utility.h"

CItemObject::CItemObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItemObject::CItemObject(const CItemObject& rhs)
	:CGameObject(rhs.m_pGraphicDev)
{
}

CItemObject::~CItemObject()
{
}

HRESULT CItemObject::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CItemObject::Update_GameObject(const _float& fTimeDelta)
{
	return S_OK;
}

void CItemObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CItemObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/*m_pTextureCom->Set_Texture(0);*/
	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CItemObject::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Nomal_Rock"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Nomal_Rock", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CItemObject::Load_Object(const _tchar* _path)
{
}

CItemObject* CItemObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

	CItemObject* pInstance = new CItemObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("NomalRockObj Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CItemObject::Free()
{
}
