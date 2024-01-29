#include "..\Include\stdafx.h"
#include "..\Header\CInven.h"
#include"CUI.h"
#include"CItem.h"

#include "Export_Utility.h"
#include "Export_System.h"

CInven::CInven(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State)
	: Engine::CGameObject(pGraphicDev)
	,m_vecInvenSlot()

{
	
}

CInven::CInven(const CInven& rhs)
	: Engine::CGameObject(rhs)

{

}

CInven::~CInven()
{

}

HRESULT CInven::Ready_GameObject()
{

	for (int i = 0; i < 15; i++)
	{
		int PixelJump = 0;
		if (i == 5 || i == 10 || i == 15)
			PixelJump = 7;
		m_vecInvenSlot[i]= CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(150.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot");
		
	}
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_vecInvenSlot
	return S_OK;
}

HRESULT CInven::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (int i = 0; i < 15; i++)
	{
		int PixelJump = 0;
		if (i == 5 || i == 10 || i == 15)
			PixelJump = 7;
		m_vecInvenSlot[i] = CUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(150.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot");
		m_fX[i]= m_vecInvenSlot[i]->Get_fX();
		m_fY[i] = m_vecInvenSlot[i]->Get_fY();

	}

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CInven::Update_GameObject(const _float& fTimeDelta)
{


	for (int i = 0; i < 15; i++)
		m_vecInvenSlot[i]->Update_GameObject(fTimeDelta);


	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CInven::LateUpdate_GameObject()
{
	int collisionResult = UI_Collision(); // 충돌 체크 한 번만 수행

	if (collisionResult != -99)
	{
		if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
		{
			m_fX[collisionResult] = static_cast<float>(m_MousePoint.x);
			m_fY[collisionResult] = static_cast<float>(m_MousePoint.y);
		}
	}

	for (int i = 0; i < 15; i++)
	{
		m_vecInvenSlot[i]->LateUpdate_GameObject();
	}

	__super::LateUpdate_GameObject();
}

void CInven::Render_GameObject()
{
	for (int i = 0; i < 15; i++) {


		m_vecInvenSlot[i]->Render_GameObject();


		Get_Scene()->BeginOrtho();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pTextureCom[i]->Set_Texture(0);

		m_pTransformCom[i]->Set_Pos(m_fX[i] - (WINCX >> 1), -m_fY[i] + (WINCY >> 1), 0.f);

		m_pTransformCom[i]->Set_Scale(_vec3{ 20, 20, 1.f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->Get_WorldMatrix());
		m_pBufferCom[i]->Render_Buffer();
		Get_Scene()->EndOrtho();






	}


}

int CInven::UI_Collision()
{
	GetCursorPos(&m_MousePoint);
	ScreenToClient(g_hWnd, &m_MousePoint);

	for (int i = 0; i < 15; i++)
	{
		if (m_fX[i] - 20 < m_MousePoint.x && m_MousePoint.x < m_fX[i] + 20 &&
			m_fY[i] - 20 < m_MousePoint.y && m_MousePoint.y < m_fY[i] + 20)
		{
			return i; // 충돌이 발생한 칸을 찾으면 해당 인덱스를 반환하고 함수 종료
		}
	}

	return -99; // 모든 칸에 대한 충돌 체크를 완료하고도 충돌이 없으면 -99 반환

}



void CInven::Push_Item(CItem* _pItem, const _tchar* _ItemName)
{


	for (int i = 0; i < 15; i++)
	{

		//현재 아이템 슬롯에 똑같은 아이템이 있는지 없는지 검사, 있다면 해당 칸에 카운트 증가



		//아이템 슬롯에 아이템이 없다면
		if (m_vecInvenSlot[i]->IsItemOn() == false)
		{
			float Tmp = 0;
			float Src = 0;
			Tmp= m_vecInvenSlot[i]->Get_fX();
			Src= m_vecInvenSlot[i]->Get_fY();
			_pItem->SetPos({ Tmp,Src,0 });
			m_vecInvenSlot[i]->SetItemOn(true);
			return;
		}
	}

}

HRESULT CInven::Add_Component()
{
	for (int i = 0; i < 15; i++) {
		m_vecInvenSlot[i]->Add_Component();

		CComponent* pComponent = nullptr;

		pComponent = m_pBufferCom[i] = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });


		pComponent = m_pTextureCom[i] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Meat_Monster"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Meat_Monster", pComponent });


		pComponent = m_pTransformCom[i] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	}

	return S_OK;
}



CInven* CInven::Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State)
{
	CInven* pInstance = new CInven(pGraphicDev, _State);

	if (FAILED(pInstance->Ready_GameObject(pGraphicDev)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CInven::Free()
{
	for (int i = 0; i < 15; i++)
	{
	m_vecInvenSlot[i]->Free();

	Safe_Release(m_vecInvenSlot[i]);
	}
	__super::Free();
}

