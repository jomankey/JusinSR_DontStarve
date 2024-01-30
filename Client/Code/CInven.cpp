#include "..\Include\stdafx.h"
#include "..\Header\CInven.h"


#include "Export_Utility.h"
#include "Export_System.h"

#include"CUI.h"

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
	for (int i = 0; i < 15; i++)
	{
		int PixelJump = 0;
		if (i == 5 || i == 10 || i == 15)
			PixelJump = 7;
		m_vecInvenSlot[i]= CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(150.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot");
		
	}
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_vecInvenSlot
	return S_OK;
}

HRESULT CInven::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (int i = 0; i < 15; i++)
	{
		int PixelJump = 0;
		if (i == 5 || i == 10 || i == 15)
			PixelJump = 7;
		m_vecInvenSlot[i] = CUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(150.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot");

	}

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CInven::Update_GameObject(const _float& fTimeDelta)
{

	renderer::Add_RenderGroup(RENDER_UI, this);
	for (int i = 0; i < 15; i++)
	m_vecInvenSlot[i]->Update_GameObject(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CInven::LateUpdate_GameObject()
{
	for (int i = 0; i < 15; i++)
	m_vecInvenSlot[i]->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CInven::Render_GameObject()
{
	for (int i = 0; i < 15; i++)
	m_vecInvenSlot[i]->Render_GameObject();
}

HRESULT CInven::Add_Component()
{
	for (int i = 0; i < 15; i++)
	m_vecInvenSlot[i]->Add_Component();
	return S_OK;
}



CInven* CInven::Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State)
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
	for (int i = 0; i < 15; i++)
	{
	m_vecInvenSlot[i]->Free();

	Safe_Release(m_vecInvenSlot[i]);
	}
	__super::Free();
}

