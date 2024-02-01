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
	m_fPreTime += fTimeDelta * 1000;

	if (10000 < m_fPreTime)
	{
		if (iChangeIndex < 2)
			iChangeIndex += 1;
		else
			iChangeIndex = 0;

		m_fPreTime = 0;
	}

	return iChangeIndex;
}

void CLightMgr::Free()
{
	for_each(m_vecLight.begin(), m_vecLight.end(), CDeleteObj());
	m_vecLight.clear();
}
