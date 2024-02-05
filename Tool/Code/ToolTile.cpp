#include "ToolTile.h"
#include "Export_Utility.h"
#include "Export_System.h"

CToolTile::CToolTile(LPDIRECT3DDEVICE9 pGraphicDev, _int iNum)
	: CGameObject(pGraphicDev), m_iNum(iNum)
{
}

CToolTile::~CToolTile()
{
}

HRESULT CToolTile::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CToolTile::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);
	//renderer::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CToolTile::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CToolTile::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CToolTile::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(proto::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_BurgerCube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_BurgerCube", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CToolTile* CToolTile::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNum)
{
	CToolTile* pInstance = new CToolTile(pGraphicDev, iNum);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CToolTile::Free()
{
	__super::Free();
}
