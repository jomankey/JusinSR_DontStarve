#include "stdafx.h"
#include "CTeleporterWorm.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "CRoadScene.h"


CTeleporterWorm::CTeleporterWorm(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CTeleporterWorm::CTeleporterWorm(const CTeleporterWorm& rhs)
	: CResObject(rhs.m_pGraphicDev)
{
}

CTeleporterWorm::~CTeleporterWorm()
{
}

HRESULT CTeleporterWorm::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));


	m_eObject_id = TELEPORTER;
	m_eTelporterCurState = TELEPORTER_IDLE;
	m_fFrame = 0.f;
	m_fFrameEnd = 7.f;
	m_fDiffY = 0.f;
	//Ready_Stat();
	m_Stat.strObjName = L"웜홀";

	return S_OK;
}

_int CTeleporterWorm::Update_GameObject(const _float& fTimeDelta)
{

	m_fFrame += m_fFrameEnd * fTimeDelta;
	if (m_fFrameEnd < m_fFrame)
	{
		m_fFrame = 0.f;
	}
	ChangeScenePlayer(1.f);//플레이어가 1.f거리안에 있을때 C키 입력시 씬이동

	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CTeleporterWorm::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	//Change_Frame_Event();
	Check_FrameState();

	//IsPlayerInRadius();


	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

}

void CTeleporterWorm::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTeleporterTextureCom[m_eTelporterCurState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CTeleporterWorm::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTeleporterTextureCom[TELEPORTER_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Teleporter_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Teleporter_Idle", pComponent });

	pComponent = m_pTeleporterTextureCom[TELEPORTER_OPEN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Teleporter_Open"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Teleporter_Open", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 2.3f, vPos.z);

	return S_OK;
}


void CTeleporterWorm::Change_Frame_Event()
{

}

//충돌반경
void CTeleporterWorm::ChangeScenePlayer(_float _fDistance)
{

	_vec3 vPlayerPos;
	_vec3 vPos;
	_vec3 vDistance;

	float fDistance = 0.f;
	scenemgr::Get_CurScene()->GetPlayerObject()->GetTransForm()->Get_Info(INFO_POS, &vPlayerPos);
	GetTransForm()->Get_Info(INFO_POS, &vPos);

	vDistance = vPlayerPos - vPos;
	vDistance.y = 0.f;
	fDistance = D3DXVec3Length(&vDistance);
	if (fDistance <= _fDistance)
	{
		if (KEY_TAP(DIK_C))
		{
			ChangeScene(CRoadScene::Create(m_pGraphicDev, L"ROAD"));
		}
	}


}

void CTeleporterWorm::Check_FrameState()
{
	bool IsClose = get<0>(IsPlayerInRadius());
	//플레이어 pos 저장용 , 나중에 멤버변수로 가지고 있어도 됨
	_vec3 vPlayerPos = get<1>(IsPlayerInRadius());
	switch (m_eTelporterCurState)
	{
	case CTeleporterWorm::TELEPORTER_IDLE:
	{


		if (IsClose)
		{
			m_eTelporterCurState = TELEPORTER_OPEN;
			//예) 플레이어 위치로 이동
			//m_pTransForm->Set_Pos(vPlayerPos);
			m_fFrame = 0.f;
			break;
		}


		if (m_fFrameEnd < m_fFrame)
		{
			m_fFrame = 0.f;
		}
		m_fFrameEnd = 7.0f;
		break;
	}
	case CTeleporterWorm::TELEPORTER_OPEN:
	{
		if (!IsClose)
		{
			m_eTelporterCurState = TELEPORTER_IDLE;
			m_fFrame = 0.f;
		}
		if (m_fFrameEnd < m_fFrame)
		{
			m_fFrame = 0.f;
		}
		m_fFrameEnd = 6.0f;
		break;
	}
	default:
		m_eTelporterCurState = TELEPORTER_IDLE;
		m_fFrame = 0.0f;
		m_fFrameEnd = 5.0f;
		break;
	}

}







bool CTeleporterWorm::IsPlayerInRadius(_vec3& _PlayerPos, _vec3& _MyPos)
{
	_PlayerPos.y = 0.f;
	_MyPos.y = 0.f;
	if (D3DXVec3Length(&(_PlayerPos - _MyPos)) < 3.f)
	{
		return true;
	}
	else
	{
		return  false;
	}


}

tuple<_bool, _vec3> CTeleporterWorm::IsPlayerInRadius()
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



CResObject* CTeleporterWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CTeleporterWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CTeleporterWorm::Free()
{
	CGameObject::Free();
}


