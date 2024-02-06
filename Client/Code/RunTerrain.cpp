#include "RunTerrain.h"

CRunTerrain::CRunTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CRunTerrain::~CRunTerrain()
{
}

HRESULT CRunTerrain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CRunTerrain::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_NONALPHA, this);

	_vec3 vPos;

	m_pTransForm->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	for (auto& iter : m_pCubeBufferCom)
		iter->Update_GameObject(fTimeDelta);

	return 0;
}

void CRunTerrain::LateUpdate_GameObject()
{
	for (auto& iter : m_pCubeBufferCom)
		iter->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CRunTerrain::Render_GameObject()
{
	for (auto& iter : m_pCubeBufferCom)
		iter->Render_GameObject();
}

HRESULT CRunTerrain::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

HRESULT CRunTerrain::SetUp_Material()
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

CRunTerrain* CRunTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRunTerrain* pInstance = new CRunTerrain(pGraphicDev);

	if (pInstance->Ready_GameObject())
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRunTerrain::Free()
{
	for (auto& iter : m_pCubeBufferCom)
		Safe_Release(iter);
	//Safe_Delete_Vec(m_pCubeBufferCom);

	__super::Free();
}
