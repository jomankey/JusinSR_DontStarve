#include "../Header/ToolItem.h"
#include "framework.h"
#include "Export_System.h"
#include "Export_Utility.h"

CToolItem::CToolItem(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key, _vec3 _vPos)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_strItemKey(_key)
	, m_tItemInfo{}
{

}

CToolItem::CToolItem(const CToolItem& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_strItemKey(rhs.m_strItemKey)
	, m_tItemInfo(rhs.m_tItemInfo)
{

}

CToolItem::~CToolItem()
{
}

HRESULT CToolItem::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolItem::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	MousePicking();
	Billboard();
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return _int();
}

void CToolItem::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);

}

void CToolItem::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CToolItem* CToolItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key, _vec3 _vPos)
{
	CToolItem* pInstance = new CToolItem(pGraphicDev, _key, _vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	pInstance->SetPos(_vPos);
	return pInstance;
}

void CToolItem::MousePicking()
{
}

void CToolItem::Billboard()
{
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

}


HRESULT CToolItem::Add_Component()
{

	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	assert(m_strItemKey.c_str() != L"");

	//TEXTURE
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_strItemKey.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_strItemKey.c_str(), pComponent });

	//TransForm
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


}

void CToolItem::SetPos(const _vec3& _vPos)
{
	_vec3 vPos = _vPos;
	vPos.y = .3f;
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Scale(_vec3(0.5f, .3f, .5f));
}

void CToolItem::Free()
{
	CGameObject::Free();
}
