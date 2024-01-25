#include "ToolGrass.h"
#include "Export_System.h"
#include "Export_Utility.h"

CToolGrass::CToolGrass(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CGameObject(pGraphicDev), m_vPos(vPos)
{
}

CToolGrass::CToolGrass(const CToolGrass& rhs)
	:CGameObject(rhs.m_pGraphicDev), m_vPos(rhs.m_vPos)
{
}

CToolGrass::~CToolGrass()
{
}

HRESULT CToolGrass::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolGrass::Update_GameObject(const _float& fTimeDelta)
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

void CToolGrass::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CToolGrass::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CToolGrass::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Obejct_Grass"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Grass", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	return S_OK;
}

CToolGrass* CToolGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CToolGrass* pInstance = new CToolGrass(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CToolGrass::Free()
{
	__super::Free();
}
