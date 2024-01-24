#include "..\Include\stdafx.h"
#include "..\Header\UICamera.h"

#include "Export_System.h"
#include "Export_Utility.h"

CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CUICamera::~CUICamera()
{
}

HRESULT CUICamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;
	m_fNear = fNear;
	m_fFar = fFar;


	//사용안함
	m_fFov = -1.f;
	m_fAspect = -1.f;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, m_fNear, m_fFar);


	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}


void CUICamera::LateUpdate_GameObject()
{
	Engine::CCamera::LateUpdate_GameObject();
}

CUICamera* CUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fNear, const _float& fFar)
{
	CUICamera* pInstance = new CUICamera(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fNear, fFar)))
			{
				Safe_Release(pInstance);
				MSG_BOX("UICamera Create Failed");
				return nullptr;
			}
	return nullptr;
}

void CUICamera::Free()
{
	Engine::CCamera::Free();
}
