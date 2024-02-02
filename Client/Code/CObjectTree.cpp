#include "CObjectTree.h"

#include "Export_System.h"
#include "Export_Utility.h"

CObjectTree::CObjectTree(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CObjectTree::CObjectTree(const CObjectTree& rhs)
	:CGameObject(rhs.m_pGraphicDev)
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
	m_pTransForm->BillBoard();
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CObjectTree::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	//_vec3	vPos;
	//m_pTransForm->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);

}

void CObjectTree::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
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
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Tree_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree_Idle", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	return S_OK;
}

CObjectTree* CObjectTree::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObjectTree* pInstance = new CObjectTree(pGraphicDev);

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
