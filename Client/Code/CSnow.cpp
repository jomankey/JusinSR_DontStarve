#include "CSnow.h"

CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CParticle(pGraphicDev, _strObjName)
{
}

CSnow::CSnow(const CParticle& rhs)
	:CParticle(rhs)
{
}

CSnow::~CSnow()
{
}

_int CSnow::Update_GameObject(const _float& fTimeDelta)
{

	std::list<Attribute>::iterator i;

	for (i = m_ParticleList.begin(); i != m_ParticleList.end(); i++)
	{
		i->m_vPos += i->m_vVelocity * fTimeDelta;

		// 포인트가 경계를 벗어났는가?  
		if (i->m_vPos.x < m_vMinBox.x || i->m_vPos.x > m_vMaxBox.x ||
			i->m_vPos.y < m_vMinBox.y || i->m_vPos.y > m_vMaxBox.y ||
			i->m_vPos.z < m_vMinBox.z || i->m_vPos.z > m_vMaxBox.z)
		{
			resetParticle(&(*i));
		}
	}


	__super::Update_GameObject(fTimeDelta);
	return 0;
}


CSnow* CSnow::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _vec3 MinBox, _vec3 MaxBox)
{
	CSnow* pInstance = new CSnow(pGraphicDev, _strObjName);

	pInstance->m_vbSize = 2048;
	pInstance->m_fSize = 0.2f;
	pInstance->m_vbOffset = 0;
	pInstance->m_vbBatchSize = 512;
	pInstance->m_vMinBox = MinBox;
	pInstance->m_vMaxBox = MaxBox;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}



	for (int i = 0; i < numparticles; i++)
	{
		pInstance->AddParticle();
	}

	return pInstance;
}
