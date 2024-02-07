#include "CreateUI.h"
#include "InvenBoxMgr.h"
#include <ItemTool.h>
#include "Export_System.h"
#include "stdafx.h"

CCreateUI::CCreateUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pGraphicDev(pGraphicDev)
{
}

CCreateUI::~CCreateUI()
{
}

HRESULT CCreateUI::Ready_GameObject()
{
	CInvenBoxMgr::GetInstance()->Add_InvenBoxList(m_pGraphicDev, CREATE, HEIGHT, 3);

	// 고정 아이템 이미지 넣어주기 
	_vec3 vPos;
	CInvenBoxMgr::GetInstance()->Get_BoxPos(CREATE, 0, &vPos);
	CItem* pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Weapon", vPos);
	CInvenBoxMgr::GetInstance()->Set_Create_Menu(0, pItem);

	CInvenBoxMgr::GetInstance()->Get_BoxPos(CREATE, 1, &vPos);
	pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Alive", vPos);
	CInvenBoxMgr::GetInstance()->Set_Create_Menu(1, pItem);

	CInvenBoxMgr::GetInstance()->Get_BoxPos(CREATE, 2, &vPos);
	pItem = CItemTool::Create(m_pGraphicDev, L"Proto_UI_Equipment", vPos);
	CInvenBoxMgr::GetInstance()->Set_Create_Menu(2, pItem);

	for (size_t i = 0; i < 3; ++i)
	{
		CSlideUI* pUI = CSlideUI::Create(m_pGraphicDev, (eITEMTOOL_TYPE)i);
		m_vecSlide.push_back(pUI);
	}


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 30.f;
	m_fY = 300.f;

	m_fSizeX = 30.f;
	m_fSizeY = 200.f;

	//직교투영
	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CCreateUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	CInvenBoxMgr::GetInstance()->Update_InvenBoxMgr(fTimeDelta, CREATE);
	for (auto& iter : m_vecSlide) iter->Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CCreateUI::LateUpdate_GameObject()
{
	CInvenBoxMgr::GetInstance()->LateUpdate_InvenBoxMgr(CREATE);
	for (auto& iter : m_vecSlide) iter->LateUpdate_GameObject();

	Picking_Mouse();

	__super::LateUpdate_GameObject();
}

void CCreateUI::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	CInvenBoxMgr::GetInstance()->Render_InvenBoxMgr(CREATE);
	for (auto& iter : m_vecSlide) iter->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CCreateUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Left_Panel"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Left_Panel", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CCreateUI::Picking_Mouse() // 제작 목록 클릭 시 설정
{
	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		POINT tPt;
		GetCursorPos(&tPt);
		ScreenToClient(g_hWnd, &tPt);
		_vec2 vMousePos = _vec2(tPt.x, tPt.y);

		vector<CInvenBox*> vecInvenBox = CInvenBoxMgr::GetInstance()->Get_BoxList(CREATE);

		for (int i = 0; i < vecInvenBox.size(); ++i)
		{
			if (!Engine::Collision_Mouse(vMousePos, vecInvenBox[i]->Get_fX(), vecInvenBox[i]->Get_fY(), vecInvenBox[i]->Get_SizeX(), vecInvenBox[i]->Get_SizeY()))
				continue;

			if (m_vecSlide[i]->Get_Show()) // 누른 목록이 이미 보여지고 있다면
			{
				m_vecSlide[i]->Set_Show(false);
				break;
			}

			// 누른 목록이 보여지고 있지 않다면 누른 목록을 제외하고 모두 보여지지 않도록 설정한다.
			for (int j = 0; j < m_vecSlide.size(); ++j)
				m_vecSlide[j]->Set_Show(i == j ? true : false);

			break;
		}
	}
}

CCreateUI* CCreateUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCreateUI* pInstance = new CCreateUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CCreateUI::Free()
{
	for (auto& iter : m_vecSlide)
		Safe_Release(iter);

	__super::Free();
}
