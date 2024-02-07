#include "stdafx.h"
#include "CObjectFireFlies.h"

#include "Export_System.h"
#include "Export_Utility.h"

CObjectFireFlies::CObjectFireFlies(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
	, m_bIsCheckAfterDay(false)
{
}

CObjectFireFlies::CObjectFireFlies(const CObjectFireFlies& rhs)
	: CResObject(rhs.m_pGraphicDev)
{
}

CObjectFireFlies::~CObjectFireFlies()
{
}

HRESULT CObjectFireFlies::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));

	m_eFireFliesCurState = FIREFLIES_PRE;
	m_eObject_id = FIRE_FLIES;
	m_fFrame = 0.f;
	m_fFrameEnd = 17.f;
	//Ready_Stat();

	return S_OK;
}

_int CObjectFireFlies::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsCheckAfterDay)
	{
		m_fFrame += m_fFrameEnd * fTimeDelta*0.5;
		if (m_fFrameEnd < m_fFrame)
		{
			m_fFrame = 0.f;
		}
	}
	else
	{
		//if (m_eFireFliesCurState == FIREFLIES_PST)
		//{
		//	m_fFrame += m_fFrameEnd * fTimeDelta;
		//	if (m_fFrame > m_fFrameEnd)
		//	{
		//		m_bNextAnim = true;
		//	}
		//
		//}

		//밤에서 낮으로 바뀌었을 때 소멸하는 애니메이션 한 번 출력후 바로 다음 상태로 넘어가기
		//기본 loop상태에서 바꿔야 하기 때문에 
		//m_fFrame += m_fFrameEnd * fTimeDelta;
		//if (m_eFireFliesCurState == FIREFLIES_PST && m_fFrameEnd < m_fFrame)
		//	m_bNextAnim = true;
		//
		//
		////무조건 낮일 때 해당 상태를 계속 유지 시켜야 함
		//if (m_bNextAnim)
		//{
			m_eFireFliesCurState = FIREFLIES_PRE;
			m_fFrame = 0.f;
		//}
	}
	
	if (GetAsyncKeyState('F')) //밤
	{
		m_bIsCheckAfterDay = true;
	}
	
	if (GetAsyncKeyState('G')) //낮
	{

		m_bIsCheckAfterDay = false;
	}

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectFireFlies::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	Change_Frame_Event();
	Check_FrameState();





	//if (m_bIsCheckAfterDay)	//밤이면 Check_Night()
	//{
	//	Change_Frame_Event();
	//	Check_FrameState();
	//}
	//else	//낮이면
	//{
	//	//밤에서 낮으로 바꼈을 때 바뀌는 동작을 수행하기 위한 조건문
	//	if (m_eFireFliesCurState == FIREFLIES_LOOP)
	//	{
	//
	//		m_eFireFliesCurState = FIREFLIES_PST;
	//		m_bIsCheckAfterDay= true;
	//		Check_FrameState();
	//	}
	//
	//	//밤에서 낮으로 바꼇을 때 바귀는 동작을 다 수행	했으면 다시 처음으로 돌아가기 위한 조건문
	//	if (m_eFireFliesCurState == FIREFLIES_PST&& m_fFrameEnd < m_fFrame)
	//	{
	//		m_eFireFliesCurState = FIREFLIES_PRE;
	//		m_fFrame = 0.f;
	//	}
	//	// 완전히 낮일 때 계속 안보이게 하기 위한 조건문
	//	else
	//	{
	//		m_eFireFliesCurState = FIREFLIES_PRE;
	//		m_fFrame = 0.f;
	//	}
	//	
	//}
	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CObjectFireFlies::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pFireFliesTextureCom[m_eFireFliesCurState]->Set_Texture((_uint)m_fFrame);
	//FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CObjectFireFlies::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pFireFliesTextureCom[FIREFLIES_PRE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FireFlies_Pre"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FireFlies_Pre", pComponent });

	pComponent = m_pFireFliesTextureCom[FIREFLIES_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FireFlies_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FireFlies_Idle", pComponent });

	pComponent = m_pFireFliesTextureCom[FIREFLIES_PST] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FireFlies_Pst"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FireFlies_Pst", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 2.3f, vPos.z);

	return S_OK;
}


void CObjectFireFlies::Change_Frame_Event()
{
	if (m_bIsCheckAfterDay)
	{
		if (m_eFireFliesCurState == FIREFLIES_PRE)
		{
			m_eFireFliesCurState = FIREFLIES_LOOP;
		}
	}

	//if (!m_bIsCheckAfterDay)
	//{
	//	m_eFireFliesCurState = FIREFLIES_PST;
	//}

}
BOOL CObjectFireFlies::Check_Night()
{
	if (light::Get_TimeIndex() == TIME_STATE::NIGHT)
		return true;
	else 
		return false;

}

void CObjectFireFlies::Check_FrameState()
{

	//현재 돌고 있는 프레임 체크용
	if (m_eFireFliesPreState == m_eFireFliesCurState)
		return;

	if (m_eFireFliesCurState == FIREFLIES_PRE)
		m_fFrameEnd = 17;

	if (m_eFireFliesCurState == FIREFLIES_LOOP)
		m_fFrameEnd = 34;

	if (m_eFireFliesCurState == FIREFLIES_PST)
	{
		//if(m_fFrameEnd < m_fFrame)
		//{
		//	m_bNextAnim = true;
		//}
		m_fFrameEnd = 18;
	}

	m_eFireFliesPreState = m_eFireFliesCurState;
	m_fFrame = 0.f;
}



CResObject* CObjectFireFlies::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CObjectFireFlies(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectFireFlies::Free()
{
	CGameObject::Free();
}


