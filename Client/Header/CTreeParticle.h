#pragma once
#include "CParticle.h"
class CTreeParticle :
	public CParticle
{
protected:
	explicit CTreeParticle(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	explicit CTreeParticle(const CParticle& rhs);
	virtual ~CTreeParticle() override;

	virtual void resetParticle(Attribute* attribute);


public:
	virtual _int			Update_GameObject(const _float& fTimeDelta)override;
	static CTreeParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _float _fLifeTime, _float _fSpeed);


	_float m_fLifeTime;
	_float m_fAccTime;
	_float m_Speed;

};

