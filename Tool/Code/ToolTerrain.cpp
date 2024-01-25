#include "ToolTerrain.h"
#include "framework.h"
#include "Export_Utility.h"
#include "ToolMgr.h"

CToolTerrain::CToolTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CToolTerrain::CToolTerrain(const CToolTerrain& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CToolTerrain::~CToolTerrain()
{
}

HRESULT CToolTerrain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolTerrain::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CToolTerrain::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CToolTerrain::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pTextureCom->Set_Texture(0);
	
	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_FORCE_DWORD);
}


HRESULT CToolTerrain::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Engine::Clone_Proto(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tile1"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Tile1", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

HRESULT CToolTerrain::SetUp_Material()
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(CToolMgr::m_fMtrlDiffuseColor[0], CToolMgr::m_fMtrlDiffuseColor[1], CToolMgr::m_fMtrlDiffuseColor[2], 1.f);
	tMtrl.Ambient = D3DXCOLOR(CToolMgr::m_fMtrlAmbientColor[0], CToolMgr::m_fMtrlAmbientColor[1], CToolMgr::m_fMtrlAmbientColor[2], 1.f);
	tMtrl.Specular = D3DXCOLOR(CToolMgr::m_fMtrlSpecularColor[0], CToolMgr::m_fMtrlSpecularColor[1], CToolMgr::m_fMtrlSpecularColor[2], 1.f);

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

CToolTerrain* CToolTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolTerrain* pInstance = new CToolTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CToolTerrain::Free()
{
	__super::Free();
}
