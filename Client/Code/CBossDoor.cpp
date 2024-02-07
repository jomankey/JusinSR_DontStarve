#include "stdafx.h"
#include "CBossDoor.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBossDoor::CBossDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
	, m_pAnimCom(nullptr)
	, m_eBossDoorCurState(BOSSDOOR_END)
{
}

CBossDoor::CBossDoor(const CBossDoor& rhs)
	: CResObject(rhs.m_pGraphicDev)
	, m_eBossDoorCurState(rhs.m_eBossDoorCurState)
	, m_pAnimCom(nullptr)
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
	m_eBossDoorCurState = BOSSDOOR_END;
	//Ready_Stat();

	return S_OK;
}

_int CBossDoor::Update_GameObject(const _float& fTimeDelta)
{

	bool IsPlayerNear = get<0>(IsPlayerInRadius());
	if (IsPlayerNear)
	{
		if (m_eBossDoorPrevState != BOSSDOOR_OPEN)
		{
			m_eBossDoorCurState = BOSSDOOR_OPEN;
		}
	}
	else
	{
		m_eBossDoorCurState = BOSSDOOR_CLOSE;

	}

	if (m_pAnimCom->GetAnimFinish(L"OPEN"))
	{
		m_eBossDoorCurState = BOSSDOOR_IDLE;
	}

	Check_FrameState();

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CBossDoor::LateUpdate_GameObject()
{




	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CBossDoor::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pAnimCom->SetAnimTexture();
	//m_pBossDoorTextureCom[m_eBossDoorCurState]->Set_Texture((_uint)m_fFrame);
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

	//pComponent = m_pBossDoorTextureCom[BOSSDOOR_OPEN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Open"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Open", pComponent });
	//
	//
	//pComponent = m_pBossDoorTextureCom[BOSSDOOR_CLOSE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Close"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Close", pComponent });
	//
	//pComponent = m_pBossDoorTextureCom[BOSSDOOR_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_BossDoor_Idle"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_Object_BossDoor_Idle", pComponent });

	pComponent = m_pAnimCom = dynamic_cast<CAnimator*>(proto::Clone_Proto(L"Proto_Anim"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Anim", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	m_pAnimCom->AddAnimation(L"OPEN", proto::Clone_ProtoAnim(L"Proto_Object_BossDoor_Open"));
	m_pAnimCom->AddAnimation(L"CLOSE", proto::Clone_ProtoAnim(L"Proto_Object_BossDoor_Close"));
	m_pAnimCom->AddAnimation(L"IDLE", proto::Clone_ProtoAnim(L"Proto_Object_BossDoor_Idle"));
	m_pAnimCom->SetCurAnimation(L"CLOSE");
	m_pAnimCom->SetCurAnimationFrame(L"CLOSE", m_pAnimCom->GetAnimMaxFrame(L"CLOSE"));
	m_pAnimCom->SetLoopAnimator(false);

	//m_pAnimCom->SetLoopAnimator(true);



	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 2.3f, vPos.z);

	return S_OK;
}

void CBossDoor::Check_FrameState()
{
	switch (m_eBossDoorCurState)
	{
	case CBossDoor::BOSSDOOR_OPEN:
	{
		if (m_eBossDoorPrevState == BOSSDOOR_OPEN)
			break;

		m_pAnimCom->SetCurAnimation(L"OPEN");
		m_pAnimCom->SetLoopAnimator(false);
		break;
	}

	case CBossDoor::BOSSDOOR_CLOSE:
	{
		if (m_eBossDoorPrevState == BOSSDOOR_CLOSE)
			break;

		m_pAnimCom->SetCurAnimation(L"CLOSE");
		m_pAnimCom->SetLoopAnimator(false);

		break;
	}
	case CBossDoor::BOSSDOOR_IDLE:
	{
		if (m_eBossDoorPrevState == BOSSDOOR_IDLE)
			break;

		m_pAnimCom->SetCurAnimation(L"IDLE");
		m_pAnimCom->SetLoopAnimator(true);

		break;
	}
	case CBossDoor::BOSSDOOR_END:
	{



		m_pAnimCom->SetCurAnimationFrame(L"OPEN", 0);
		break;
	}
	default:
		break;
	}

	m_eBossDoorPrevState = m_eBossDoorCurState;
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

tuple<_bool, _vec3> CBossDoor::IsPlayerInRadius()
{
	bool IsClose = false;
	_vec3 vPlayerPos;
	_vec3 vMyPos;
	decltype(auto) vPlayerTrans = scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm();
	vPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransForm->Get_Info(INFO_POS, &vMyPos);
	vPlayerPos.y = 0.f;
	vMyPos.y = 0.f;

	_vec3 vDir = vPlayerPos - vMyPos;
	float _test = D3DXVec3Length(&vDir);

	if (D3DXVec3Length(&(vPlayerPos - vMyPos)) < 3.0f)
	{
		//m_eTelporterCurState = TELEPORTER_OPEN;
		IsClose = true;
	}
	else
	{
		//m_eTelporterCurState = TELEPORTER_IDLE;
		IsClose = false;
	}
	return make_tuple(IsClose, vPlayerPos);
}


