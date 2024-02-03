#include "ToolPigHouse.h"

CToolPigHouse::CToolPigHouse(LPDIRECT3DDEVICE9 pGrahpicDev)
	: CGameObject(pGrahpicDev)
{
}

CToolPigHouse::~CToolPigHouse()
{
}

HRESULT CToolPigHouse::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CToolPigHouse::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	m_pTransForm->BillBoard();

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CToolPigHouse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CToolPigHouse::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	FAILED_CHECK_RETURN(SetUp_Material(), );

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CToolPigHouse::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Pig_House"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Pig_House", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.2f, vPos.z);

	return S_OK;
}

CToolPigHouse* CToolPigHouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolPigHouse* pInstance = new CToolPigHouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CToolPigHouse::Free()
{
	__super::Free();
}
