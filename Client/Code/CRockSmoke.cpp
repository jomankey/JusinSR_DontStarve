#include "stdafx.h"
#include "CRockSmoke.h"
#include "Export_Utility.h"
#include "Export_System.h"


CRockSmoke::CRockSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CParticle(pGraphicDev, _strObjName)
	, m_fAccTime(0.f)
{
}

CRockSmoke::CRockSmoke(const CParticle& rhs)
	:CParticle(rhs)
	, m_fAccTime(0.f)
{
}

CRockSmoke::~CRockSmoke()
{
}
void CRockSmoke::resetParticle(Attribute* attribute)
{
	attribute->m_bAlive = true;
	attribute->m_vPos = m_pTransForm->Get_Pos();

	_vec3	min = _vec3(-0.3f, 0.6f, -0.3f);
	_vec3	max = _vec3(0.3f, 1.f, 0.3f);

	GetRendomVector(&attribute->m_vVelocity, &min, &max);

	attribute->m_vVelocity *= m_Speed;
	attribute->m_fAge = 0.f;
	attribute->m_fLifeTime = m_fLifeTime;
}
_int CRockSmoke::Update_GameObject(const _float& fTimeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = m_ParticleList.begin(); i != m_ParticleList.end(); i++)
	{
		if (i->m_bAlive)
		{
			i->m_vPos += i->m_vVelocity * fTimeDelta;
			i->m_fAge += fTimeDelta;

			if (i->m_fAge > i->m_fLifeTime)
			{
				i->m_bAlive = false;
			}
		}

	}

	m_fAccTime += fTimeDelta;
	if (m_fLifeTime < m_fAccTime)
	{
		DeleteObject(this);
	}


	__super::Update_GameObject(fTimeDelta);
	return 0;
}

CRockSmoke* CRockSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 _vPos, _float _fSize, _float _fLifeTime, _float _fSpeed)
{
	CRockSmoke* pInstance = new CRockSmoke(pGraphicDev, _strObjName);

	pInstance->m_vbSize = 2048;
	pInstance->m_fSize = _fSize;
	pInstance->m_vbOffset = 0;
	pInstance->m_vbBatchSize = 512;
	pInstance->m_vMinBox = _vec3(-1.f, -1.f, -1.f);
	pInstance->m_vMaxBox = _vec3(1.f, 1.f, 1.f);
	pInstance->m_fLifeTime = _fLifeTime;
	pInstance->m_Speed = _fSpeed;
	_vPos.x *= 0.5f;
	_vPos.y *= 0.5f;
	_vPos.z *= 0.5f;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->m_pTransForm->Set_Pos(_vPos);

	for (int i = 0; i < numparticles; i++)
	{
		pInstance->AddParticle();
	}

	return pInstance;
}