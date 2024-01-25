#include "..\Include\stdafx.h"
#include "..\Header\CUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State)
	: Engine::CGameObject(pGraphicDev)
	, m_eUIState(_State)
{

}

CUI::CUI(const CUI& rhs)
	: Engine::CGameObject(rhs),
	m_eUIState(rhs.m_eUIState)
{

}

CUI::~CUI()
{
}

HRESULT CUI::Ready_GameObject(_vec3 _pos, _vec3 _size)
{


	m_fSizeX = _size.x;
	m_fSizeY = _size.y;


	m_fX = _pos.x;
	m_fY = _pos.y;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	return S_OK;
}

Engine::_int CUI::Update_GameObject(const _float& fTimeDelta)
{



	Engine::Add_RenderGroup(RENDER_UI, this);
	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CUI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{
	Get_Scene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = m_fSizeX;
	m_matWorld._22 = m_fSizeY;

	m_matWorld._41 = m_fX - (WINCX>>1);
	m_matWorld._42 = -m_fY + (WINCY >> 1);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pBufferCom->Render_Buffer();

	Get_Scene()->EndOrtho();
}



HRESULT CUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Nomal_Rock"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Nomal_Rock", pComponent });


	return S_OK;
}



CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size)
{
	CUI* pInstance = new CUI(pGraphicDev, _State);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI::Free()
{
	__super::Free();
}


