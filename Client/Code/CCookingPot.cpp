#include "stdafx.h"
#include "CCookingPot.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include <Mouse.h>
#include "SlotMgr.h"

CCookingPot::CCookingPot(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
	: CResObject(pGraphicDev), m_bInstall(bInstall)
{
}

CCookingPot::CCookingPot(const CCookingPot& rhs)
	: CResObject(rhs.m_pGraphicDev), m_bInstall(rhs.m_bInstall)
{

}

CCookingPot::~CCookingPot()
{
}

HRESULT CCookingPot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObject_id = COOKING_POT;
	m_eCookingpotCurState = COOKINGPOT_DEFAULT;
	
	m_fDiffY = 1.f;
	m_fFrame = 0.0f;
	return S_OK;
}

_int CCookingPot::Update_GameObject(const _float& fTimeDelta)
{
	Install_Obj();
	if (GetAsyncKeyState('T')) // 횃불
	{
		
		Set_Drop();
	}
	if (GetAsyncKeyState('6')) // 횃불
	{
		Set_Cooking(false);
	}

	if (GetAsyncKeyState('7')) // 횃불
	{
		Set_Cooking(true);
	}



	if (GetAsyncKeyState('8')) // 횃불
	{
		Set_Hit();
		Set_Empty();
	}
	if (GetAsyncKeyState('9')) // 횃불
	{
		Set_Hit();
		Set_Full();
	}

	if (GetAsyncKeyState('0')) // 횃불
	{
		Set_Burnt();
	}

	if (!m_bIsFrameStop)
	{
		m_fFrame += m_fFrameEnd * fTimeDelta;
	}

	m_fTimeChek += fTimeDelta;
	if (m_fTimeChek >= m_MaxfTimeChek)
	{
		m_fTimeChek = 0.f;
	}



	Change_Frame_Event();

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CCookingPot::LateUpdate_GameObject()
{


	
	Check_FrameState();
	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CCookingPot::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	
	m_pCookingpotTexCom[m_eCookingpotCurState]->Set_Texture((_uint)m_fFrame);
	
	
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}



HRESULT CCookingPot::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;



	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	
	pComponent = m_pCookingpotTexCom[COOKINGPOT_IDLE_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Idle_empty"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Idle_empty", pComponent });


	pComponent = m_pCookingpotTexCom[COOKINGPOT_IDLE_FULL] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Idle_full"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Idle_full", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_COOKING_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_loop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_loop", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_BURNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Burnt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Burnt", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_PLACE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Place"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Place", pComponent });


	pComponent = m_pCookingpotTexCom[COOKINGPOT_HIT_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Hit_Empty"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Hit_Empty", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_DEFAULT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Default"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Default", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_HIT_COOKING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Hit", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.0f, vPos.z);

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CCookingPot::Check_FrameState()
{
	if (m_eCookingpotCurState != m_eCookingpotPrevState)
	{


		switch (m_eCookingpotCurState)
		{
		case CCookingPot::COOKINGPOT_IDLE_EMPTY:
			m_fFrameEnd = 0.0f;
			break;
		case CCookingPot::COOKINGPOT_IDLE_FULL:
			m_fFrameEnd = 0.0f;
			break;
		case CCookingPot::COOKINGPOT_COOKING_LOOP:
			m_fFrameEnd = 6.0f;
			break;
		case CCookingPot::COOKINGPOT_BURNT:
			m_fFrameEnd=0.0f;
			break;
		case CCookingPot::COOKINGPOT_PLACE:
			m_fFrameEnd = 8.0f;
			break;
		case CCookingPot::COOKINGPOT_HIT_EMPTY:
			m_fFrameEnd = 9.0f;
			break;
		case CCookingPot::COOKINGPOT_HIT_COOKING:
			m_fFrameEnd = 9.0f;
			break;
		case CCookingPot::COOKINGPOT_DEFAULT:
			m_fFrameEnd = 0.0f;
			break;
		default:
			break;
		}
		m_eCookingpotPrevState = m_eCookingpotCurState;
		m_fFrame = 0.0f;
	}
}

void CCookingPot::Change_Frame_Event()
{
	if (m_bIsDrop)
	{
		if (m_eCookingpotCurState == COOKINGPOT_DEFAULT)
		{
			m_eCookingpotCurState = COOKINGPOT_PLACE;
		}

		//솥이 타버리면 복구 불가
		if (m_bIsBurnt)
		{
			m_eCookingpotCurState = COOKINGPOT_BURNT;
			return;

		}

		//솥이 떨어지는 모션이 끝나면 프레임 고정
		if (m_eCookingpotCurState== COOKINGPOT_PLACE&&m_fFrame>m_fFrameEnd)
		{
			m_bIsFrameStop = true;
			m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;
		}
		
		//솥이 떨어지는 모션이 끝나고 그리고 요리가 시작 되었을 때 프레임 반복
		else if (m_eCookingpotCurState == COOKINGPOT_COOKING_LOOP&& m_bIsCooking)
		{
			m_bIsFrameStop= false;
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame=0.0f;
			}
		}

		//솥이 떨어지는 모션이 끝나고 요리가 시작되지 않았을 때 Empty로 돌아감
		else if(m_eCookingpotCurState == COOKINGPOT_COOKING_LOOP && !m_bIsCooking)
		{
			m_bIsFrameStop = true;
			m_eCookingpotCurState = COOKINGPOT_IDLE_EMPTY;
		}


		//요리를 다시 시작하려 할 때
		if (m_eCookingpotCurState == COOKINGPOT_IDLE_EMPTY && m_bIsCooking)
		{
			m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;
		}


		//솥에 요리가 비어있고, 요리 중일 때
		if (m_bIsHit&& m_bIsEmpty&& m_bIsCooking)
		{
		
			m_eCookingpotCurState = COOKINGPOT_HIT_EMPTY;
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame = 0.0f;
				m_bIsHit = false;
				m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;
			}

		}

		//솥에 요리가 차있고, 요리 중일 때
		if (m_bIsHit && m_bIsFull&& m_bIsCooking)
		{
			
			m_eCookingpotCurState= COOKINGPOT_HIT_COOKING;
			if (m_fFrame > m_fFrameEnd)
			{
				m_fFrame = 0.0f;
				m_bIsHit = false;
				m_eCookingpotCurState = COOKINGPOT_COOKING_LOOP;

			}

		}






	}


}

void CCookingPot::Install_Obj()
{
	if (!m_bInstall) return;

	auto& vecTerrain = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::TILE)[0];
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(scenemgr::Get_CurScene()->GetTerrainObject()->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	_vec3 vPos = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, vecTerrain->GetTransForm());

	vPos.y = 1.f;
	m_pTransForm->Set_Pos(vPos);

	if (Engine::GetMouseState(DIM_LB) == eKEY_STATE::TAP) // 설치 완료
	{
		m_bInstall = false;
		m_bIsDrop= true;
		auto& vecMouse = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::ENVIRONMENT, eOBJECT_GROUPTYPE::MOUSE)[0];
		CMouse* pMouse = dynamic_cast<CMouse*>(vecMouse);
		pMouse->Set_Install(false);

		CSlotMgr::GetInstance()->Remove_InvenItem(m_iSlotNum);
	}
}




CCookingPot* CCookingPot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bInstall)
{
	CCookingPot* pInstance = new CCookingPot(pGraphicDev, bInstall);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCookingPot::Free()
{
	CGameObject::Free();
}


