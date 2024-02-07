#include "stdafx.h"
#include "CBossDoor.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBossDoor::CBossDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CBossDoor::CBossDoor(const CBossDoor& rhs)
	: CResObject(rhs.m_pGraphicDev)
{
}

CBossDoor::~CBossDoor()
{
}

HRESULT CBossDoor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));
	

	m_eObject_id = BOSS_DOOR;
	m_eBossDoorCurState = BOSSDOOR_OPEN;
	m_fFrame = 0.f;

	//Ready_Stat();
	
	return S_OK;
}

_int CBossDoor::Update_GameObject(const _float& fTimeDelta)
{

	m_fFrame += fTimeDelta;


	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBossDoor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();



	Check_FrameState();

	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CBossDoor::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pBossDoorTextureCom[m_eBossDoorCurState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CBossDoor::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pBossDoorTextureCom[BOSSDOOR_OPEN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Open"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Open", pComponent });
	
	
	pComponent = m_pBossDoorTextureCom[BOSSDOOR_CLOSE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Close"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Close", pComponent });

	pComponent = m_pBossDoorTextureCom[BOSSDOOR_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Idle", pComponent });



	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 2.3f, vPos.z);

	return S_OK;
}


//void CBossDoor::Change_Frame_Event()
//{
//
//}

void CBossDoor::Check_FrameState()
{
	switch (m_eBossDoorCurState)
	{
	case CBossDoor::BOSSDOOR_OPEN:
	{
		//만약 플레이어가 충돌하지 않은 상태이면 맨 처음 이미지로 고정
		//if ()
		//{
		//
		//}
		//else
		//{
		//	//플레이어가 충돌하면 열리는 이미지를 프레임 끝까지 띄운 후, Idle상태로 넘어가기
		//	if (m_fFrame >= m_pBossDoorTextureCom[BOSSDOOR_OPEN]->Get_MaxFrame())
		//	{
		//		m_eBossDoorCurState = BOSSDOOR_IDLE;
		//		m_fFrame = 0.f;
		//	}
		//}
		//break;
	}

	case CBossDoor::BOSSDOOR_CLOSE:
	{

	

		break;
	}
	case CBossDoor::BOSSDOOR_IDLE:
	{

		break;
	}
	default:
		break;
	}
	

}









CResObject* CBossDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CBossDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CBossDoor::Free()
{
	CGameObject::Free();
}


