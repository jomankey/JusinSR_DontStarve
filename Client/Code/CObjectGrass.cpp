#include "CObjectGrass.h"
#include "Export_System.h"
#include "Export_Utility.h"

CObjectGrass::CObjectGrass(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

CObjectGrass::CObjectGrass(const CObjectGrass& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

CObjectGrass::~CObjectGrass()
{
}

HRESULT CObjectGrass::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CObjectGrass::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	_matrix matWorld, matView, matBillY, matBillX;

	m_pTransForm->Get_WorldMatrix(&matWorld);
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

	// matBillY와 matBillX의 역행렬을 곱하는 순서를 변경
	m_pTransForm->Set_WorldMatrix(&(matBillX* matBillY * matWorld));

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectGrass::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);
}

void CObjectGrass::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CObjectGrass::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Grass"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Grass", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos);

	return S_OK;
}

CObjectGrass* CObjectGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObjectGrass* pInstance = new CObjectGrass(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectGrass::Free()
{
	CGameObject::Free();
}
