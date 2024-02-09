#include "stdafx.h"
#include "../Header/CItem.h"

#include "Export_System.h"
#include "Export_Utility.h"




CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, L"NONE"), m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f), m_fPreX(0.f), m_fPreY(0.f)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_tItemInfo{}
{
	
}

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CGameObject(pGraphicDev, _strObjName)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_tItemInfo{}
{
	m_tItemInfo.ItemCount = 1;
}

CItem::CItem(const CItem& rhs)
	:CGameObject(rhs.m_pGraphicDev)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_tItemInfo(rhs.m_tItemInfo)
{
}

CItem::~CItem()
{
}

void CItem::MousePicking()
{
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CItem::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

HRESULT CItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//TEXTURE
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(GetObjName().c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ GetObjName().c_str(), pComponent });

	//TransForm
	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(0.5f, 0.3f, 0.5f));

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}


void CItem::Free()
{
	__super::Free();
}
