#include "ItemBasic.h"
#include "Export_Utility.h"
#include "stdafx.h"
#include "SlotMgr.h"
#include <Terrain.h>

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev), m_bChangeRander(false)
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

void CItemBasic::Pickup_Item(_vec3 vSlotPos)
{
	m_pTransForm->Get_Info(INFO_POS, &m_vPos);
	m_pCalculatorCom->Change_OnObjectMatrix(g_hWnd, &m_vPos);
	m_bChangeRander = true;

	m_vSlotPos = vSlotPos;

	m_fX = m_vPos.x;
	m_fY = m_vPos.y;

	m_fSizeX = 25.f;
	m_fSizeY = 25.f;

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.0f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(180.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);
}

HRESULT CItemBasic::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_ObjState();

    return S_OK;
}

_int CItemBasic::Update_GameObject(const _float& fTimeDelta)
{

	CGameObject::Update_GameObject(fTimeDelta);

	if (m_bChangeRander)
	{
		if (m_vSlotPos.y - m_fY <= 0.1f)
		{
			_vec3 vSlotPos = {};
			CSlotMgr::GetInstance()->AddItem(m_pGraphicDev, m_strObjName, &vSlotPos);
			DeleteObject(this);
			//return 0x80000000;
		}
			
		// 아이템 먹엇을때 슬롯쪽으로 이동
		m_fX += (m_vSlotPos.x - m_fX) * 5.f * fTimeDelta;
		m_fY += (m_vSlotPos.y - m_fY) * 5.f * fTimeDelta;

		m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.1f));
	}

	MousePicking();
	m_pTransForm->BillBoard();

	if (m_bIsCreateByObject)
	{
		DropMotion(fTimeDelta);
	}

	renderer::Add_RenderGroup(m_bChangeRander ? RENDER_UI : RENDER_ALPHA, this);

	return 0;
}

void CItemBasic::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	if (!m_bChangeRander) Height_OnTerrain();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);
}

void CItemBasic::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	if (m_bChangeRander)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (!m_bChangeRander) m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	if (!m_bChangeRander) m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	scenemgr::Get_CurScene()->EndOrtho();
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
