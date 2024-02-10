#include "ItemBasic.h"
#include "Export_Utility.h"

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	: CItem(pGraphicDev, _strObjName)
{
}

CItemBasic::CItemBasic(const CItem& rhs)
	: CItem(rhs)
{
}

CItemBasic::~CItemBasic()
{
}

HRESULT CItemBasic::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CItemBasic::Update_GameObject(const _float& fTimeDelta)
{

	CGameObject::Update_GameObject(fTimeDelta);

	MousePicking();
	m_pTransForm->BillBoard();

	if (m_bIsCreateByObject)
	{
		DropMotion(fTimeDelta);
	}

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CItemBasic::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);
}

void CItemBasic::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CItemBasic::DropMotion(const _float& fTimeDelta)
{
	_vec3	vDir;
	_vec3   vOrigin;
	m_pTransForm->Get_Info(INFO_POS, &vOrigin);
	m_pTransForm->Get_Info(INFO_UP, &vDir);
	m_pTransForm->Move_Pos(&-vDir, 20.0f,  fTimeDelta);
	//float fDistance = D3DXVec3Length(&(_vec3{ vOrigin.x,3.f,vOrigin.z } - vOrigin));
	if (vOrigin.y < 0.7f)
	{
		m_bIsCreateByObject = false;
	}
	//if (fDistance < 20.f)
	//{
	//	m_bIsCreateByObject = false;
	//}


}

CItem* CItemBasic::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _szName)
{
	CItem* pInstance = new CItemBasic(pGraphicDev, _szName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

    return pInstance;
}

void CItemBasic::Free()
{
	__super::Free();
}
