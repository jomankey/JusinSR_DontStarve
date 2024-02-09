#include "StateUI.h"

CStateUI::CStateUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), 
	m_fSizeX(0.f), 
	m_fSizeY(0.f), 
	m_fX(0.f), 
	m_fY(0.f),
	m_frame(0.f),
	m_frameEnd(0.f),
	m_PreFrame(0.f),
	m_bFrameStop(true)
{
}

CStateUI::~CStateUI()
{
}

HRESULT CStateUI::Ready_GameObject()
{
	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CStateUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CStateUI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStateUI::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_frame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStateUI::Free()
{
	__super::Free();
}
