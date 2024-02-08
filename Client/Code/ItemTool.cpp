#include "ItemTool.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "stdafx.h"
#include <Slot.h>
#include "SlotMgr.h"
CItemTool::CItemTool(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, UI_ITEM_TYPE eType, _bool bFood)
	: CItem(pGraphicDev, _strObjName), m_fX(vPos.x), m_fY(vPos.y), m_fSizeX(0.f), m_fSizeY(0.f), m_bFood(bFood), m_eItemType(eType), m_fPreX(0.f), m_fPreY(0.f), m_eArmorSlotType(ARMOR_SLOT_END)
{
	m_tItemInfo.ItemCount = 1;
}

CItemTool::CItemTool(const CItem& rhs)
	: CItem(rhs)
{
}

CItemTool::~CItemTool()
{
}

HRESULT CItemTool::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSizeX = 15.f;
	m_fSizeY = 15.f;

	m_fPreX = m_fX;
	m_fPreY = m_fY;

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	//m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(90.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CItemTool::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Input_Mouse();

	return 0;
}

void CItemTool::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CItemTool::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CItemTool::Input_Mouse()
{
	POINT tPt;
	GetCursorPos(&tPt);
	ScreenToClient(g_hWnd, &tPt);
	_vec2 vMousePos = _vec2(tPt.x, tPt.y);

	if ((Engine::Get_DIMouseState(DIM_LB) & 0x8000) && m_eItemType == UI_ITEM_INVEN) // 마우스 피킹 후 움직이면 
	{
		if (Engine::Collision_Mouse(vMousePos, m_fX, m_fY, m_fSizeX, m_fSizeY))
		{
			m_fX += Engine::Get_DIMouseMove(DIMS_X);
			m_fY += Engine::Get_DIMouseMove(DIMS_Y);

			m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
		}
	}
	else if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::AWAY && m_eItemType == UI_ITEM_INVEN) // 마우스 UP
	{
		vector<CSlot*> vecBox = CSlotMgr::GetInstance()->Get_BoxList(INVEN);
		for (int i = 0; i < INVENCNT; ++i)
		{
			//아이템 사이즈
			_vec2 vItemPos = _vec2(m_fX, m_fY);

			if (Engine::Collision_Mouse(vItemPos, vecBox[i]->Get_fX(), vecBox[i]->Get_fY(), vecBox[i]->Get_SizeX(), vecBox[i]->Get_fY()))
			{
				//아이템과 박스 충돌
				if (CSlotMgr::GetInstance()->Get_InvenItem(i) != nullptr) // 충돌한 박스가 null이 아니라면
					break;

				m_fX = vecBox[i]->Get_fX();
				m_fY = vecBox[i]->Get_fY();
				m_fPreX = m_fX;
				m_fPreY = m_fY;

				m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
				CSlotMgr::GetInstance()->Move_InvenItem(this, m_iNum, i);
				m_iNum = i;
			}
		}

		for (int i = 15; i < INVENCNT + 3; ++i) // 장착칸에 닿았다면
		{
			_vec2 vItemPos = _vec2(m_fX, m_fY);

			if (Engine::Collision_Mouse(vItemPos, vecBox[i]->Get_fX(), vecBox[i]->Get_fY(), vecBox[i]->Get_SizeX(), vecBox[i]->Get_fY()))
			{
				//아이템과 박스 충돌
				// 아이템이 장착 아이템이 아니라면 
				// 충돌한 아이템이 알맞은 장착칸에 들어갔는지 확인
				if (m_eArmorSlotType == ARMOR_SLOT_END || m_eArmorSlotType != (ARMOR_SLOT_TYPE)i)
					return;

				// 충돌한 박스가 null이 아니라면 -> 장착하고 있던 아이템 인벤토리로 보내고, 새로 장착하기
				if (CSlotMgr::GetInstance()->Get_InvenItem(i) != nullptr) // 충돌한 박스가 null이 아니라면
					//CSlotMgr::GetInstance()->Change_ArmorItem(this, m_eArmorSlotType, m_iNum);
					return;

				m_fX = vecBox[i]->Get_fX();
				m_fY = vecBox[i]->Get_fY();
				m_fPreX = m_fX;
				m_fPreY = m_fY;

				m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));

				CSlotMgr::GetInstance()->Set_ArmorItem(m_eArmorSlotType, this, m_iNum);

				m_iNum = i;
			}
		}
	}
	else if (m_eItemType == UI_ITEM_INVEN)
	{
		m_fX = m_fPreX;
		m_fY = m_fPreY;

		m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	}
}

CItem* CItemTool::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, _vec3 vPos, UI_ITEM_TYPE eType, _bool bFood)
{
	CItem* pInstance = new CItemTool(pGraphicDev, _strObjName, vPos, eType, bFood);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItemTool::Free()
{
	__super::Free();
}
