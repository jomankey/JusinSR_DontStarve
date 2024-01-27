#include "CObjectRock.h"
#include "Export_Utility.h"

CObjectRock::CObjectRock(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CGameObject(pGraphicDev), m_vPos(vPos)
{
}

CObjectRock::CObjectRock(const CObjectRock& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_vPos(rhs.m_vPos)
{
}

CObjectRock::~CObjectRock()
{
}

HRESULT CObjectRock::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CObjectRock::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	_matrix matTransform = matBill * matWorld;
	m_pTransformCom->Set_WorldMatrix(&(matTransform));

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectRock::LateUpdate_GameObject()
{

	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CObjectRock::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CObjectRock::Add_Component()
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

	m_pTransformCom->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
	//s
	m_pTransformCom->Set_Pos(m_vPos.x, 1.5f, m_vPos.z);
	return S_OK;
}

CObjectRock* CObjectRock::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CObjectRock* pInstance = new CObjectRock(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectRock::Free()
{
	__super::Free();
}
