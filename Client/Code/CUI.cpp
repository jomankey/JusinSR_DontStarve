#include "..\Include\stdafx.h"
#include "..\Header\CUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, const _tchar* _UiName)
	: Engine::CGameObject(pGraphicDev)
	, m_eUIState(_State)
	, m_pUI_Name(_UiName)
	, m_bIsCollision(false)
{

}

CUI::CUI(const CUI& rhs)
	: Engine::CGameObject(rhs),
	m_eUIState(rhs.m_eUIState)
	, m_bIsCollision(false)
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
	D3DXMatrixIdentity(&m_matWorld);

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
	if (UI_Collision())
	{
		m_fX += 0.1f;
	}
	
	__super::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{

	Get_Scene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	
	m_matWorld._11 = m_fSizeX;
	m_matWorld._22 = m_fSizeY;
	

	m_matWorld._41 = m_fX - (WINCX >> 1);
	m_matWorld._42 = -m_fY + (WINCY >> 1);



	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pBufferCom->Render_Buffer();



	Get_Scene()->EndOrtho();


}



BOOL CUI::UI_Collision()
{

	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if(m_fX - (m_fSizeX ) < ptMouse.x && ptMouse.x < m_fX + (m_fSizeX ))
		if (m_fY - (m_fSizeY ) < ptMouse.y && ptMouse.y < m_fY + (m_fSizeY ))
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


