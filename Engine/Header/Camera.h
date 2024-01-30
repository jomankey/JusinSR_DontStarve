#pragma once
#include "GameObject.h"

namespace Engine
{
	class ENGINE_DLL CCamera : public CGameObject
	{
	protected:
		explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual ~CCamera();

	public:
		virtual HRESULT Ready_GameObject() override;
		virtual _int Update_GameObject(const _float& fTimeDelta) override;
		virtual void LateUpdate_GameObject() override;

	public://직교투영설정
		void BeginOrtho();
		void EndOrtho();
	protected:
		_vec3			m_vEye;
		_vec3			m_vAt;
		_vec3			m_vUp;
		_float			m_fFov, m_fAspect, m_fNear, m_fFar;
		_matrix			m_matView;
		_matrix			m_matProj;

	protected:
		virtual void	Free();
	};

}
