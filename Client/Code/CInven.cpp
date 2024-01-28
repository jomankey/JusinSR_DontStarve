#include "..\Include\stdafx.h"
#include "..\Header\CInven.h"
#include"CUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

CInven::CInven(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State)
	: Engine::CGameObject(pGraphicDev)
	,m_vecInvenSlot()

{
	
}

CInven::CInven(const CInven& rhs)
	: Engine::CGameObject(rhs)
{

}

CInven::~CInven()
{
}

HRESULT CInven::Ready_GameObject()
{
	m_vecInvenSlot[0]= CUI::Create(m_pGraphicDev, UI_DYNAMIC, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f),L"Teswt");
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_vecInvenSlot
	return S_OK;
}

HRESULT CInven::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CInven::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CInven::LateUpdate_GameObject()
{

	__super::LateUpdate_GameObject();
}

void CInven::Render_GameObject()
{

}

HRESULT CInven::Add_Component()
{
	return S_OK;
}



CInven* CInven::Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size)
{
	CInven* pInstance = new CInven(pGraphicDev, _State);

	if (FAILED(pInstance->Ready_GameObject(pGraphicDev)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CInven::Free()
{
	__super::Free();
}

