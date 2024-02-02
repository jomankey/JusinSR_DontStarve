#include "../Header/ToolItem.h"
#include "framework.h"
#include "Export_System.h"
#include "Export_Utility.h"

CToolItem::CToolItem(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key, _vec3 _vPos)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_strItemKey(_key)
	, m_tItemInfo{}
{

}

CToolItem::CToolItem(const CToolItem& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(rhs.m_pBufferCom)
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
	m_pTransForm->BillBoard();
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return _int();
}

void CToolItem::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);

}

void CToolItem::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
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

HRESULT CToolItem::Add_Component()
{

	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	assert(m_strItemKey.c_str() != L"");

	//TEXTURE
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(m_strItemKey.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ m_strItemKey.c_str(), pComponent });

	//TransForm
	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CToolItem::SetPos(const _vec3& _vPos)
{
	_vec3 vPos = _vPos;
	vPos.y = .3f;
	m_pTransForm->Set_Pos(vPos);
	m_pTransForm->Set_Scale(_vec3(0.5f, .3f, .5f));
}

void CToolItem::Free()
{
	CGameObject::Free();
}
