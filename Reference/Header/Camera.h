#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :	public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();
	
public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

	_matrix* Get_ViewSpaceMatrix() { return &m_matView; }
	_matrix* Get_ProjectionMatrix() { return &m_matProj; }
	_matrix* Get_OrthoMatrix() { return &matOrtho; }
	_matrix* Get_OrthoViewMatrix() { return &matOrthoView; }
	
public://직교투영설정
	void BeginOrtho();
	void EndOrtho();
protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix			m_matView, m_matProj;
	_matrix			matOrtho;
	_matrix			matOrthoView;

protected:
	virtual void	Free();
};

END

