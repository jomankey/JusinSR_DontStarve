#include "../Include/stdafx.h"
#include "../Header/CGameItem.h"

#include "Export_System.h"
#include "Export_Utility.h"

CGameItem::CGameItem(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_strItemKey(_key)
{

}

CGameItem::CGameItem(const CGameItem& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_strItemKey(rhs.m_strItemKey)
{
}

CGameItem::~CGameItem()
{
}

HRESULT CGameItem::Ready_GameObject()
{
	return E_NOTIMPL;
}

_int CGameItem::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CGameItem::LateUpdate_GameObject()
{
}

void CGameItem::Render_GameObject()
{
}

HRESULT CGameItem::Add_Component()
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

void CGameItem::SetPos(const _vec3& _vPos)
{
	m_pTransformCom->Set_Pos(_vPos);
}

CGameItem* CGameItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CGameItem::Free()
{
}
