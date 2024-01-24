#pragma once

#include "Camera.h"

class CUICamera : public Engine::CCamera
{
private:
	explicit CUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fNear,
		const _float& fFar);

	virtual void LateUpdate_GameObject() override;

public:
	static CUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free();

};

