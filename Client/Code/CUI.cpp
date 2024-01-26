#include "..\Include\stdafx.h"
#include "..\Header\CUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, const _tchar* _UiName)
	: Engine::CGameObject(pGraphicDev)
	, m_eUIState(_State)
	, m_pUI_Name(_UiName)
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
	//sss
	Engine::Add_RenderGroup(RENDER_UI, this);
	CGameObject::Update_GameObject(fTimeDelta);
	//m_fSizeX = m_fSizeX+ fTimeDelta*30;
	//m_fSizeY = m_fSizeY+ fTimeDelta*30;
	m_pTransformCom->Set_Scale(_vec3{ m_fSizeX ,m_fSizeY,0 });
	//m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f*0.01));
	//m_pTransformCom->Get_WorldMatrix();
	m_pTransformCom->Get_WorldMatrix()->_41 = m_fX - (WINCX >> 1);
	m_pTransformCom->Get_WorldMatrix()->_42 = -m_fY + (WINCY >> 1);

	/*_vec3 vScale = { 100.0f,100.0f,100.0f };

	m_pTransformCom->Set_Scale(vScale);*/

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

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pBufferCom->Render_Buffer();

	Get_Scene()->EndOrtho();
}



HRESULT CUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_pUI_Name));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_pUI_Name, pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	return S_OK;
}



CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UiName)
{
	CUI* pInstance = new CUI(pGraphicDev, _State, _UiName);
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


