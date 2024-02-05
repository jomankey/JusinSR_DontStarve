#include "TerrainCubeTex.h"

Engine::CTerrainCubeTex::CTerrainCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

Engine::CTerrainCubeTex::CTerrainCubeTex(const CTerrainCubeTex& rhs)
	: CVIBuffer(rhs.m_pGraphicDev)
{
}

Engine::CTerrainCubeTex::~CTerrainCubeTex()
{
}

HRESULT Engine::CTerrainCubeTex::Ready_Buffer()
{
	return S_OK;
}

void Engine::CTerrainCubeTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CTerrainCubeTex* Engine::CTerrainCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainCubeTex* pInstance = new CTerrainCubeTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

CComponent* CTerrainCubeTex::Clone()
{
	return new CTerrainCubeTex(*this);
}

void Engine::CTerrainCubeTex::Free()
{
	__super::Free();
}