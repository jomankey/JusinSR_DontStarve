#include "CObjectTree.h"

#include "Export_System.h"
#include "Export_Utility.h"

CObjectTree::CObjectTree(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CGameObject(pGraphicDev), m_vPos(vPos)
{
}

CObjectTree::CObjectTree(const CObjectTree& rhs)
	:CGameObject(rhs.m_pGraphicDev), m_vPos(rhs.m_vPos)
{
}

CObjectTree::~CObjectTree()
{
}

HRESULT CObjectTree::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CObjectTree::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	_matrix	matWorld, matView, matBillY, matBillX;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBillY);
	D3DXMatrixIdentity(&matBillX);

	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	matBillX._21 = matView._21;
	matBillX._22 = matView._22;
	matBillX._32 = matView._32;
	matBillX._33 = matView._33;

	D3DXMatrixInverse(&matBillY, NULL, &matBillY);
	D3DXMatrixInverse(&matBillX, NULL, &matBillX);

	m_pTransformCom->Set_WorldMatrix(&(matBillX * matBillY * matWorld));
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CObjectTree::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);
}

void CObjectTree::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CObjectTree::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Obejct_Tree"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransformCom->Set_Scale(_vec3(1.5f, 1.8f, 1.5f));
	m_pTransformCom->Set_Pos(m_vPos.x, 2.0f, m_vPos.z);
	return S_OK;
}

CObjectTree* CObjectTree::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CObjectTree* pInstance = new CObjectTree(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectTree::Free()
{
	CGameObject::Free();
}
