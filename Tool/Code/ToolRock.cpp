#include "ToolRock.h"
#include "Export_Utility.h"

CToolRock::CToolRock(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CGameObject(pGraphicDev), m_vPos(vPos)
{
}

CToolRock::CToolRock(const CToolRock& rhs)
	:CGameObject(rhs.m_pGraphicDev), m_vPos(rhs.m_vPos)
{
}

CToolRock::~CToolRock()
{
}

HRESULT CToolRock::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolRock::Update_GameObject(const _float& fTimeDelta)
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

void CToolRock::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CToolRock::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CToolRock::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Obejct_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Stone", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransformCom->Set_Pos(m_vPos.x, 1.5f, m_vPos.z);

	return S_OK;
}

CToolRock* CToolRock::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CToolRock* pInstance = new CToolRock(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CToolRock::Free()
{
	__super::Free();
}
