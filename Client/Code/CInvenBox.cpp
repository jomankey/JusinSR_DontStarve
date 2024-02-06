#include "stdafx.h"
#include "CInvenBox.h"


#include "Export_Utility.h"
#include "Export_System.h"


#include "CInventoryMgr.h"
#include "CItem.h"
#include "Texture.h"
#include "Scene.h"
CInvenBox::CInvenBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vFontPos)
	:CGameObject(pGraphicDev), m_vFontPos(vFontPos)
{
}

//CInvenBox::CInvenBox(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, _uint _index)
//	:CUI(pGraphicDev, eUIState, _UI_Name)
//	, m_iInvenIndex(_index)
//	, m_ItmeCount(0)
//{
//}

CInvenBox::CInvenBox(const CInvenBox& rhs)
	:CGameObject(rhs.m_pGraphicDev),
	m_vFontPos(rhs.m_vFontPos)
{
}

CInvenBox::~CInvenBox()
{
}



HRESULT CInvenBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CInvenBox::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	CItem* pItemBox = nullptr;
	pItemBox = CInventoryMgr::GetInstance()->GetItemBox(m_iInvenIndex);

	if (nullptr != pItemBox)
	{
		m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(pItemBox->GetObjName().c_str()));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ pItemBox->GetObjName().c_str(), m_pTextureCom });
		m_ItmeCount=pItemBox->GetItemInfo().ItemCount;

		//if (UI_Collision())
		//{
		//	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
		//	{
		//		m_fX = (float)m_MousePoint.x;
		//		m_fY = (float)m_MousePoint.y;
		//	}

		//}
	}
	



	return 0;
}

void CInvenBox::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CInvenBox::Render_GameObject()
{
	if (m_pTextureCom == nullptr)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	scenemgr::Get_CurScene()->BeginOrtho();
	Engine::Render_Font(L"Font_Count", to_wstring(m_ItmeCount).c_str(), &m_vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	

	m_pTextureCom->Set_Texture(0);
	
	//m_pTransForm->Set_Pos(m_fX - (WINCX >> 1), -m_fY + (WINCY >> 1), 0.f);
	//m_pTransForm->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 1.f });
	
	m_pBufferCom->Render_Buffer();
	scenemgr::Get_CurScene()->EndOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//CInvenBox* CInvenBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, _uint _Index, float _Angle)
//{
//	CInvenBox* pInstance = new CInvenBox(pGraphicDev, _State, _UI_Name, _Index);
//	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
//	{
//		Safe_Release(pInstance);
//		MSG_BOX("UI Create Failed");
//		return nullptr;
//	}
//
//	return pInstance;
//}

CInvenBox* CInvenBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vFontPos, _vec3 vPos)
{
	CInvenBox* pInstance = new CInvenBox(pGraphicDev, vFontPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CInvenBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Item_Inven_Slot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Item_Inven_Slot", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}


void CInvenBox::Free()
{
	__super::Free();
}
