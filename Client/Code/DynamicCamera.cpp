#include "..\Include\stdafx.h"
#include "..\Header\DynamicCamera.h"

#include "Export_System.h"
#include "Export_Utility.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fAngle(-180.f)
	, m_fDistance(2.f)
	, m_fHeight(3.f)
	, m_bkeyInput(true)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt, const _vec3* pUp,
	const _float& fFov, const _float& fAspect,
	const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

Engine::_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	if (m_pTarget != nullptr)
	{
		_vec3 vTarget;
		m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);
		m_vAt = vTarget;
		m_vEye.x = vTarget.x + cosf(m_fAngle) * m_fDistance * m_fHeight;
		m_vEye.y = vTarget.y + m_fHeight * m_fHeight;
		m_vEye.z = vTarget.z + sinf(m_fAngle) * m_fDistance * m_fHeight;
	}

	if (false == m_bFix)
	{
		Mouse_Move();
		//Mouse_Fix();
	}

	return iExit;
}

void CDynamicCamera::LateUpdate_GameObject()
{
	Engine::CCamera::LateUpdate_GameObject();
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix			matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Engine::Get_DIKeyState(DIK_Q) & 0x80)
	{
		m_fAngle += 0.1f;
	}
	if (Engine::Get_DIKeyState(DIK_E) & 0x80)
	{
		m_fAngle -= 0.1f;

	}


	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * 5.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * 5.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;



}

void CDynamicCamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);


}

void CDynamicCamera::Mouse_Move()
{
	_matrix			matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove(0);

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
	{
		if (dwMouseMove > 0.f)
		{
			m_fHeight -= 0.05f;

		}
		else if (dwMouseMove < 0.f)
		{
			m_fHeight += 0.05f;

		}
	}

}


void CDynamicCamera::Free()
{
	Engine::CCamera::Free();
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("dynamiccamera Create Failed");
		return nullptr;
	}

	return pInstance;
}
