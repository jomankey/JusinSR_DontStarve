#include "stdafx.h"
#include "..\Header\DynamicCamera.h"

#include "Export_System.h"
#include "Export_Utility.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fAngle(-180.f)
	, m_fDistance(2.f)
	, m_fHeight(8.f)
	, m_fRoadDistance(6.8f)
	, m_fRoadHeight(4.2f)
	, m_bkeyInput(true)
	, m_fIntensity(3.f)
	, m_fShakeTime(0.3f)
	, m_fShakeAccTime(1.f)
	, m_bRoad(false)
	, m_bLockWidth(false)
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
	if (!m_bRoad)
	{

		Key_Input(fTimeDelta);

		Mouse_Move();

		if (KEY_TAP(DIK_U))
		{
			auto pObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER)[0];
			if (nullptr != pObj)
			{
				SetTarget(pObj);
				m_bMove = true;

			}
		}
		if (KEY_TAP(DIK_I))
		{
			SetTarget(scenemgr::Get_CurScene()->GetPlayerObject());
		}

		if (m_pTarget != nullptr && !m_pTarget->IsDelete())
		{
			_vec3 vTarget;
			m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);

			m_vAt = vTarget;//바라볼곳

			m_vTargetEye.x = vTarget.x + cosf(m_fAngle) * m_fHeight;
			m_vTargetEye.y = vTarget.y + m_fHeight;
			m_vTargetEye.z = vTarget.z + sinf(m_fAngle) * m_fHeight;//사실상 목적지
		}

		if (KEY_TAP(DIK_P))
		{
			SetShakedCamera(3.f, 1.f, false);
			m_fShakeAccTime = 0.f;
		}
		if (KEY_TAP(DIK_O))
		{
			SetShakedCamera(3.f, 1.f, true);
			m_fShakeAccTime = 0.f;
		}

		if (m_fShakeTime > m_fShakeAccTime)
		{
			m_fShakeAccTime += fTimeDelta;
			ShakeCamera();
		}

		if (m_bMove)
		{
			CalDiff(fTimeDelta);
		}
		else
		{
			m_vEye = m_vTargetEye;
		}

		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	}
	else//RoadScene
	{
		if (KEY_TAP(DIK_O))
		{
			m_fRoadHeight += 0.1f;
		}
		if (KEY_TAP(DIK_P))
		{
			m_fRoadHeight -= 0.1f;
		}
		if (KEY_HOLD(DIK_K))
		{
			m_fRoadDistance += 0.1f;
		}
		if (KEY_HOLD(DIK_L))
		{
			m_fRoadDistance -= 0.1f;
		}


		if (m_pTarget != nullptr && !m_pTarget->IsDelete())
		{
			_vec3 vTarget;
			m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);

			m_vAt = vTarget;//바라볼곳

			//카메라위치
			m_vTargetEye.x = vTarget.x - m_fRoadDistance;//4.2 ~4.3
			m_vTargetEye.y = vTarget.y + m_fRoadHeight;//2.4 ~ 4.8
			m_vTargetEye.z = m_vAt.z;
		}
		m_vEye = m_vTargetEye;
	}

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);



	return 0;
}

void CDynamicCamera::LateUpdate_GameObject()
{
	Engine::CCamera::LateUpdate_GameObject();
}

void CDynamicCamera::ShakeCamera()
{
	_vec3 vLook;
	_vec3 vRight;
	_vec3 vUp;
	D3DXVec3Normalize(&vLook, &(m_vAt - m_vEye));
	D3DXVec3Cross(&vRight, &vLook, &m_vUp);
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	// -1.5f ~ 1.5f 사이의값 구함
	float offsetY = ((rand() % 100 / 100.0f) * m_fIntensity) - (m_fIntensity * 0.5f);

	m_vEye += vUp * offsetY;
	m_vAt += vUp * offsetY;

	if (!m_bLockWidth)
	{
		float offsetX = ((rand() % 100 / 100.0f) * m_fIntensity) - (m_fIntensity * 0.5f);
		m_vEye += vRight * offsetX;
		m_vAt += vRight * offsetX;
	}


}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix			matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (KEY_HOLD(DIK_Q))
	{
		m_fAngle += 0.1f;
	}
	if (KEY_HOLD(DIK_E))
	{
		m_fAngle -= 0.1f;

	}
}

void CDynamicCamera::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CDynamicCamera::Mouse_Move()
{
	_long	dwMouseMove(0);
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
	{
		if (dwMouseMove > 0.f)
		{
			m_fHeight -= 0.1f;

		}
		else if (dwMouseMove < 0.f)
		{
			m_fHeight += 0.1f;

		}
	}

}

void CDynamicCamera::CalDiff(const _float& fTimeDelta)
{

	//카메라이동할 거리와 방향

	_vec3	vDir;
	_float	fDistance;

	D3DXVec3Normalize(&vDir, &(m_vTargetEye - m_vEye));
	fDistance = D3DXVec3Length(&(m_vEye - m_vTargetEye));

	if (fDistance <= 1.0f)
	{
		m_bMove = false;
		return;
	}
	m_vEye += 10.f * vDir * fTimeDelta;

	vDir.y = 0.f;
}

void CDynamicCamera::SetTarget(CGameObject* _targetObj)
{
	m_pTarget = _targetObj;
	_vec3 vTarget;
	m_pTarget->GetTransForm()->Get_Info(INFO::INFO_POS, &vTarget);
	m_vAt = vTarget;
}


void CDynamicCamera::Free()
{
	__super::Free();
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
