#include "stdafx.h"
#include "SlideBox.h"


#include "Export_Utility.h"
#include "Export_System.h"


#include "UIMgr.h"
#include "CItem.h"
#include "Texture.h"
#include "Scene.h"

CSlideBox::CSlideBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CGameObject(pGraphicDev), m_fX(vPos.x), m_fY(vPos.y)
{
}

CSlideBox::CSlideBox(const CSlideBox& rhs)
	: CGameObject(rhs)
{
}

CSlideBox::~CSlideBox()
{
}

HRESULT CSlideBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	m_pTransForm->Set_Pos(_vec3((m_fX - WINCX * 0.5f), -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(90.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);


	return S_OK;
}

_int CSlideBox::Update_GameObject(const _float& fTimeDelta)
{
	_vec3 vPos;
	//m_pTransForm->Get_Info(INFO_POS, &vPos);

	//if (m_fX > vPos.x)
	//{
	//	_float fSpeed = 10.f * fTimeDelta;
	//	m_fX += fSpeed;

	//	m_pTransForm->Set_Pos(_vec3(m_fX, m_fY, 0.f));
	//}

	if (m_pItem) m_pItem->Update_GameObject(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSlideBox::LateUpdate_GameObject()
{
	if (m_pItem) m_pItem->LateUpdate_GameObject();
	__super::LateUpdate_GameObject();
}

void CSlideBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	if (m_pItem) m_pItem->Render_GameObject();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSlideBox* CSlideBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
	CSlideBox* pInstance = new CSlideBox(pGraphicDev, _vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CSlideBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Explain_PopUp_Panel"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Explain_PopUp_Panel", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}


void CSlideBox::Free()
{
	Safe_Release(m_pPanel);
	Safe_Release(m_pItem);

	__super::Free();
}
