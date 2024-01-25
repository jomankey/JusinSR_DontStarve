#include "Camera.h"
#include "Pipeline.h"

#include "Engine_Macro.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCamera::~CCamera()
{
}

void Engine::CCamera::Free()
{
	CGameObject::Free();
}

HRESULT Engine::CCamera::Ready_GameObject()
{	
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	//D3DXMatrixOrthoLH(&m_matProj, 800, 600, m_fNear, m_fFar);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	
	return S_OK;
}

Engine::_int Engine::CCamera::Update_GameObject(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return 0;
}

void Engine::CCamera::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CCamera::BeginOrtho()
{
	_matrix matOrtho;
	_matrix matOrthoView;

	_vec3 vEye, vAt, vUp;
	vEye = _vec3(0.f, 0.f, 0.f);  // ���� ��ġ
	vAt = _vec3(0.f, 1.f, 1.f);    // �ٶ󺸴� ����
	vUp = _vec3(0.f, 1.f, 0.f);    // ���� ����
	ZeroMemory(&matOrtho, sizeof(_matrix));

	D3DXMatrixLookAtLH(&matOrthoView, &vEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOrthoView);

	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrtho);

}

void CCamera::EndOrtho()
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}


