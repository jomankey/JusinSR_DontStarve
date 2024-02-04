#include "stdafx.h"
#include "..\Header\CUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"

#include"CToolUI.h"


CUI* CUI::m_pToolUI = nullptr;



CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, const _tchar* _UiName)
	: Engine::CGameObject(pGraphicDev, _UiName)
	, m_eUIState(_State)
	, m_bItemChek(false)
	, m_pTextureCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_fX(0)
	, m_fY(0)
	, m_fSizeX(0)
	, m_fSizeY(0)


{

}

CUI::CUI(const CUI& rhs)
	: Engine::CGameObject(rhs),
	m_eUIState(rhs.m_eUIState)
	, m_bItemChek(rhs.m_bItemChek)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pBufferCom(rhs.m_pBufferCom)

{

}

CUI::~CUI()
{

}

HRESULT CUI::Ready_GameObject(_vec3 _pos, _vec3 _size, float _Angle)
{
	m_fX = _pos.x;
	m_fY = _pos.y;
	m_fSizeX = _size.x;
	m_fSizeY = _size.y;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(_Angle));

	m_pTransForm->Set_Pos(_pos);
	m_pTransForm->Set_Scale(_size);

	return S_OK;
}

Engine::_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	GetCursorPos(&m_MousePoint);
	ScreenToClient(g_hWnd, &m_MousePoint);

	__super::Update_GameObject(fTimeDelta);
	m_pTransForm->Get_WorldMatrix()->_41 = m_fX - (WINCX >> 1);
	m_pTransForm->Get_WorldMatrix()->_42 = -m_fY + (WINCY >> 1);
	renderer::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CUI::LateUpdate_GameObject()
{
	if (m_eUIState == UI_DYNAMIC && UI_Collision())
	{
		if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
		{
			m_fX = (float)m_MousePoint.x;
			m_fY = (float)m_MousePoint.y;
		}
	}
	else
	{
	}
	__super::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	//m_pTransForm->Set_Pos(m_fX - (WINCX >> 1), -m_fY + (WINCY >> 1), 0.f);
	//m_pTransForm->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 1.f });

	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();


}



BOOL CUI::UI_Collision()
{


	

	if (m_fX - (m_fSizeX) < m_MousePoint.x && m_MousePoint.x < m_fX + (m_fSizeX))
		if (m_fY - (m_fSizeY) < m_MousePoint.y && m_MousePoint.y < m_fY + (m_fSizeY))
		{
			return true;
		}
		else
			return false;
	else
		return false;


}

BOOL CUI::UI_Collision(CUI* _Target)
{

if (_Target->m_fX - (_Target->m_fSizeX) < m_MousePoint.x && m_MousePoint.x < _Target->m_fX + (_Target->m_fSizeX))
		if (_Target->m_fY - (_Target->m_fSizeY) < m_MousePoint.y && m_MousePoint.y < _Target->m_fY + (_Target->m_fSizeY))
		{
			return true;
		}
		else
			return false;
	else
		return false;
	
}

BOOL CUI::MouseDistanceOver()
{
	GetCursorPos(&m_MousePoint);
	ScreenToClient(g_hWnd, &m_MousePoint);

	if(m_MousePoint.x>300.f)
		return true;
	else
		return false;
}


HRESULT CUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(GetObjName().c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ GetObjName().c_str(), pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });




	return S_OK;
}



CUI* CUI::Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CUI* pInstance = new CUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	if (m_pToolUI == nullptr)
	{
		m_pToolUI = CToolUI::Create(pGraphicDev, _UI_Name);
	}


	return pInstance;
}

void CUI::Free()
{	

	Safe_Release(m_pToolUI);
	//m_pEquimentUI->Release();
	//m_pAliveUI->Release();
	//m_pToolUI->Release();
	__super::Free();
}


