#include "stdafx.h"
#include "CInvenBox.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "UIMgr.h"
#include "CItem.h"
#include "Texture.h"
#include "Scene.h"
#include <ItemTool.h>
CInvenBox::CInvenBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, BOX_TYPE eType)
	:CGameObject(pGraphicDev), m_fX(vPos.x), m_fY(vPos.y), m_pItem(nullptr), m_eType(eType)
{
}

CInvenBox::CInvenBox(const CInvenBox& rhs)
	:CGameObject(rhs.m_pGraphicDev),
	m_vFontPos(rhs.m_vFontPos),
	m_fX(rhs.m_fX),
	m_fY(rhs.m_fY),
	m_pItem(rhs.m_pItem)
{
}

CInvenBox::~CInvenBox()
{
}

HRESULT CInvenBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 15.f;
	m_fSizeY = 15.f;

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(90.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CInvenBox::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	if (m_pItem) m_pItem->Update_GameObject(fTimeDelta);
	return 0;
}

void CInvenBox::LateUpdate_GameObject()
{
	if (m_pItem) m_pItem->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CInvenBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_bool bFood = m_pItem != nullptr ? dynamic_cast<CItemTool*>(m_pItem)->Get_FoodItem() : 0;
	m_pTextureCom->Set_Texture(bFood);

	m_pBufferCom->Render_Buffer();

	if (m_pItem != nullptr)
	{
		m_pItem->Render_GameObject();
		_tchar strItemCount[32];
		_itow_s(m_pItem->Get_ItemCount(), strItemCount, 10);
		if (m_eType == INVEN)
			Engine::Render_Font(L"Panel_Info", strItemCount, &_vec2(m_fX + 8.f, m_fY +5.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
		

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CInvenBox* CInvenBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, BOX_TYPE eType)
{
	CInvenBox* pInstance = new CInvenBox(pGraphicDev, vPos, eType);
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
	Safe_Release(m_pItem);

	__super::Free();
}
