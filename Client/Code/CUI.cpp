#include "..\Include\stdafx.h"
#include "..\Header\CUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, const _tchar* _UiName)
	: Engine::CGameObject(pGraphicDev)
	, m_eUIState(_State)
	, m_pUI_Name(_UiName)
	, m_bItemChek(false)
{

}

CUI::CUI(const CUI& rhs)
	: Engine::CGameObject(rhs),
	m_eUIState(rhs.m_eUIState)
	, m_bItemChek(rhs.m_bItemChek)

{

}

CUI::~CUI()
{
	
}

HRESULT CUI::Ready_GameObject(_vec3 _pos, _vec3 _size, float _Angle)
{


	m_fSizeX = _size.x;
	m_fSizeY = _size.y;
	

	m_fX = _pos.x;
	m_fY = _pos.y;


	m_OriginfX = m_fX;
	m_OriginfY = m_fY;
	m_OriginfSizeX = m_fSizeX;
	m_OriginfSizeY = m_fSizeY;
	m_fAngle = _Angle;
	//D3DXMatrixIdentity(&m_matWorld);
	

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(m_fAngle));


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
	if (UI_Collision()&& m_eUIState== UI_DYNAMIC)
	{
		if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
		{
			m_fX = (float)m_MousePoint.x;
			m_fY = (float)m_MousePoint.y;
		}

	}
	else
	{
		m_fX = m_OriginfX;
		m_fY = m_OriginfY;
		m_fSizeX = m_OriginfSizeX;
		m_fSizeY = m_OriginfSizeY;
	}
	__super::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{

	Get_Scene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pTextureCom->Set_Texture(0);
	m_pTransformCom->Set_Pos(m_fX - (WINCX >> 1), -m_fY + (WINCY >> 1), 0.f);
	m_pTransformCom->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 1.f });
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pBufferCom->Render_Buffer();



	Get_Scene()->EndOrtho();


}



BOOL CUI::UI_Collision()
{

	
	GetCursorPos(&m_MousePoint);
	ScreenToClient(g_hWnd, &m_MousePoint);

	if(m_fX - (m_fSizeX ) < m_MousePoint.x && m_MousePoint.x < m_fX + (m_fSizeX ))
		if (m_fY - (m_fSizeY ) < m_MousePoint.y && m_MousePoint.y < m_fY + (m_fSizeY ))
		{
			return true;
		}
		else
			return false;
	else
		return false;


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



CUI* CUI::Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name,float _Angle)
{	CUI* pInstance = new CUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
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


