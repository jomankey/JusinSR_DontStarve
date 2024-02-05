#include "ToolCubeTerrain.h"
CToolCubeTerrain::CToolCubeTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pGraphicDev(pGraphicDev)
{
}

CToolCubeTerrain::~CToolCubeTerrain()
{
}

HRESULT CToolCubeTerrain::Ready_GameObject()
{
	ZeroMemory(&m_pCubeCom, sizeof(m_pCubeCom));

	for (int i = 0; i < (129 * 5 * 4); ++i)
	{
		m_pCubeCom[i] = CToolTile::Create(m_pGraphicDev, i);
		FAILED_CHECK_RETURN(m_pCubeCom[i], E_FAIL);
	}

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolCubeTerrain::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	for (auto& iter : m_pCubeCom)
	{
		iter->Update_GameObject(fTimeDelta);
	}
	renderer::Add_RenderGroup(RENDER_NONALPHA, this);
	//m_pCubeCom[0]->Update_GameObject(fTimeDelta);
	return 0;
}

void CToolCubeTerrain::LateUpdate_GameObject()
{
	for (auto& iter : m_pCubeCom)
	{
		iter->LateUpdate_GameObject();
	}
	//m_pCubeCom[0]->LateUpdate_GameObject();
	__super::LateUpdate_GameObject();
}

void CToolCubeTerrain::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& iter : m_pCubeCom)
	{
		iter->Render_GameObject();
	}
	//m_pCubeCom[0]->Render_GameObject();

}

HRESULT CToolCubeTerrain::Add_Component()
{
	
	CComponent* pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CToolCubeTerrain::Input_Mouse()
{
}

HRESULT CToolCubeTerrain::Picking_OnTerrain()
{
	return S_OK;
}

CToolCubeTerrain* CToolCubeTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolCubeTerrain* pInstance = new CToolCubeTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		
		return nullptr;
	}

	return pInstance;
}

void CToolCubeTerrain::Free()
{
	__super::Free();
}
