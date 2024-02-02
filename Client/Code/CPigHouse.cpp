#include "stdafx.h"
#include "CPigHouse.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "ResObject.h"

CPigHouse::CPigHouse(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResObject(pGraphicDev), m_eHouseState(STANDARD)
{
}

CPigHouse::CPigHouse(const CPigHouse& rhs)
	:CResObject(rhs.m_pGraphicDev), m_eHouseState(rhs.m_eHouseState)
{
}

CPigHouse::~CPigHouse()
{
}

HRESULT CPigHouse::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));

	m_eCurState = RES_IDLE;
	m_fFrame = 0.f;
	Ready_Stat();

	return S_OK;
}

_int CPigHouse::Update_GameObject(const _float& fTimeDelta)
{

	if (m_eCurState != RES_IDLE && m_eCurState != RES_DEAD)
		m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_eCurState == RES_HIT_1 || m_eCurState == RES_HIT_2) // 피격 모션이 끝난 후 IDLE로 돌아감
		{
			m_eCurState = RES_IDLE;
		}
		m_fFrame = 0.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);


	m_pTransForm->BillBoard();

	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CPigHouse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	Change_Frame_Event(); // 피격 혹은 특정 이벤트 발생시 집의 형태 변환
	Check_FrameState();

	//_vec3	vPos;
	//m_pTransForm->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);
}

void CPigHouse::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture((int)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPigHouse::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom[RES_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_IDLE"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_PigHouse_IDLE", pComponent });

	pComponent = m_pTextureCom[RES_HIT_1] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_hit_std"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_hit_std", pComponent });

	pComponent = m_pTextureCom[RES_HIT_2] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_hit_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_hit_run_down", pComponent });

	pComponent = m_pTextureCom[RES_DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PigHouse_dead", pComponent });


	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
<<<<<<< HEAD
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.f, 2.f, 2.f));
=======
	m_MultiMap[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.f, 1.5f, 1.5f));
>>>>>>> 8a923e83ae43d5dfaa506da26a4f67fbf042a8d9
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.2f, vPos.z);

	return S_OK;
}

void CPigHouse::Change_Frame_Event()
{
	//if () // 충돌 혹은 피격 혹은 아침저녁 바뀔때 메소드
	//{
	//	m_eHouseState = ??		//스테이트 바꾸기
	//	
	//}

	//시간에 따른 텍스쳐 프레임값 변경 
	TIME_STATE  eTimeState = light::Get_TimeIndex();
	//IDLE일때의 상태값
	if (m_eCurState == RES_IDLE && eTimeState != MORNING && m_Stat.fHP == m_Stat.fMxHP)
		m_fFrame = 1.f; // 점조명 추가필요
	if (m_eCurState == RES_IDLE && m_Stat.fHP <= 4)
		m_fFrame = 2.f;

	//HIT 시 상태값
	if (m_bHit && m_Stat.fHP <= 4)
		m_eCurState = RES_HIT_2;

	else if (m_bHit && m_Stat.fHP >= 5)
		m_eCurState = RES_HIT_1;

	else if (!m_bHit)
		m_eCurState = RES_IDLE;

	if (m_Stat.fHP <= 0) // 체력이 0일때
		m_eCurState = RES_DEAD;
}


void CPigHouse::Check_FrameState()
{
	if (m_ePreState == m_eCurState)
		return;

	if (m_eCurState == RES_IDLE)
		m_fFrameEnd = 0;
	if (m_eCurState == RES_HIT_1)
		m_fFrameEnd = 5;
	if (m_eCurState == RES_HIT_2)
	{
		m_fFrameEnd = 5;
		m_pTransForm->Set_Scale(_vec3(1.9f, 1.9f, 1.9f));
	}
	if (m_eCurState == RES_DEAD)
	{
		m_fFrameEnd = 0;
		m_pTransForm->Set_Scale(_vec3(0.8f, 0.8f, 0.8f));
	}

	m_ePreState = m_eCurState;
	m_fFrame = 0.f;
}

void CPigHouse::Ready_Stat()
{
	m_Stat.fHP = 10.f;
	m_Stat.fMxHP = 10.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}


CResObject* CPigHouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CPigHouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CPigHouse::Free()
{
	CGameObject::Free();
}

