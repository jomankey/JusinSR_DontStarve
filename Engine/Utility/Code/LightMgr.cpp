#include "..\..\Header\LightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr() : m_fPreTime(0)
{
}

CLightMgr::~CLightMgr()
{
	Free();
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, 
								const D3DLIGHT9 * pLightInfo, 
								const _uint & iIndex)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_vecLight.push_back(pLight);

	return S_OK;
}

_int CLightMgr::Change_Light(const _float& fTimeDelta, _int _iIndex)
{
	static _int iChangeIndex = 0;
	//π„≥∑ √º¿Œ¡ˆ 
	if (iChangeIndex == 0)
	{
		if (m_fPreTime + 10000 == fTimeDelta)
		{
			iChangeIndex += 1;
			m_fPreTime = 0;
			return iChangeIndex;
		}
		else
			m_fPreTime += 1;
	}
	else if (iChangeIndex == 1)
	{
		if (m_fPreTime + 5000 == fTimeDelta)
		{
			iChangeIndex += 1;
			m_fPreTime = 0;
			return iChangeIndex;
		}
		else
			m_fPreTime += 1;
	}
	else if (iChangeIndex == 2)
	{
		if (m_fPreTime + 3000 == fTimeDelta)
		{
			iChangeIndex = 0;
			m_fPreTime = 0;
			return iChangeIndex;
		}
		else
			m_fPreTime += 1;
	}
}

void CLightMgr::Free()
{
	for_each(m_vecLight.begin(), m_vecLight.end(), CDeleteObj());
	m_vecLight.clear();
}
