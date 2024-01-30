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

//HRESULT CInven::Ready_GameObject()
//{
//
//	for (int i = 0; i < 15; i++)
//	{
//		int PixelJump = 0;
//		if (i == 5 || i == 10 || i == 15)
//			PixelJump = 7;
//		m_vecInvenSlot[i]= CUI::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(150.f + PixelJump + (i * 35), 580, 0.f), _vec3(15.f, 15.f, 0.f), L"Proto_UI_Item_Inven_Slot");
//		
//	}
//	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
//	//m_vecInvenSlot
//	return S_OK;
//}

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
		m_vecInvenSlot[i]->SetItemOn(false);

	}
	//m_pUI_Name = L"Berries";
	ItemCreate = false;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CInven::Update_GameObject(const _float& fTimeDelta)
{
	for (int i = 0; i < 15; i++)
		m_vecInvenSlot[i]->Update_GameObject(fTimeDelta);

	if (GetAsyncKeyState('W'))
	{
		//m_pUI_Name = L"Log";
		Push_Item(3, L"Log");

		CheckItemSlotCount++;

	}if (GetAsyncKeyState('S'))
	{
		Push_Item(1, L"Berries");
		//m_pUI_Name = L"Berries";
	}



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
	}

	Get_Scene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for(int i=0; i< 15;i++)
	{
		//아이템이 맨 처음 들어왔는지 확인
		if (ItemCreate)
		{
			//m_vecInvenSlot[t]번 째 칸에 아이템이 있는지 확인
			if ([&]() {for(int t=0;t<15;t++){return m_vecInvenSlot[i]->IsItemOn();}}())
			{

				Engine::CTexture* pTexture = dynamic_cast<Engine::CTexture*>(Engine::Get_Component(ID_STATIC, L"GameLogic", m_pUI_Name, m_pUI_Name));
				NULL_CHECK(pTexture);
				pTexture->Set_Texture(0);
				m_pTransformCom[i]->Set_Pos(m_fX[i] - (WINCX >> 1), -m_fY[i] + (WINCY >> 1), 0.f);

				m_pTransformCom[i]->Set_Scale(_vec3{ 15, 15, 1.f });
				m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->Get_WorldMatrix());
				m_pBufferCom[i]->Render_Buffer();
				continue;
				//break;
			}
		}

		//m_pTextureCom[i]->Set_Texture(0);


	}

		Get_Scene()->EndOrtho();


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

const _tchar* CInven::FindItemName(const _tchar* _ItemName)
{
	m_mapItem.find(_ItemName)->first;


	return nullptr;
}

int CInven::Find_ItemCount(const _tchar* _ItemName)
{
	if(m_mapItem.find(_ItemName) == m_mapItem.end())
		return itemCount = 0;
	else
	return itemCount = m_mapItem.find(_ItemName)->second;
}

void CInven::CallFind_ItemCount(function<void()> func)
{
	//최초 진입시
	ItemCreate = true;
	itemCount = 0;
	//아이템이 들어오면 해당 칸에 슬롯을 true로 만들고 다음 아이템이 들어왔어도 첫 번째 칸에 아이템이 있으니 다음
	if([&](){for (int i = 0; i < 15; i++)
			{
				if (m_vecInvenSlot[i]->IsItemOn())
				{
				continue;

				}
				else 
				{
					m_vecInvenSlot[i]->SetItemOn(true);
					return true;
				}
			
			};}())
	//for (int i = 0; i < 15; i++)
	//{
	//	if (m_vecInvenSlot[i]->IsItemOn())
	//		continue;
	//
	//	m_vecInvenSlot[i]->SetItemOn(true);
	//	return;
	//}

	func();
}



void CInven::Push_Item(int _ItmeCount, const _tchar* _ItemName)
{
	CallFind_ItemCount([&]() {
		itemCount = Find_ItemCount(_ItemName); });

	if (itemCount <= 0)
	{
		itemCount = itemCount + _ItmeCount;
		m_pUI_Name = _ItemName;
		m_mapItem.insert(make_pair(_ItemName, itemCount));
		return;
	}
	else
	{
		itemCount = itemCount + _ItmeCount;
		m_pUI_Name = _ItemName;
		m_mapItem[_ItemName] = itemCount;
		return;
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


		//pComponent = m_pTextureCom[i] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Meat_Monster"));
		//NULL_CHECK_RETURN(pComponent, E_FAIL);
		//m_mapComponent[ID_STATIC].insert({ L"Meat_Monster", pComponent });


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

