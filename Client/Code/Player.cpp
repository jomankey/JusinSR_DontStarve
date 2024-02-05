#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "CInven.h"
#include "Monster.h"

#include "Component.h"
#include "Layer.h"
#include "Scene.h"
#include "CItem.h"

//Manager
#include "CInventoryMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bAttack(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev,wstring _strName)
	: Engine::CGameObject(pGraphicDev,_strName), m_bAttack(false)
{
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
	, m_bAttack(rhs.m_bAttack)
	, m_Stat(rhs.m_Stat)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	/*m_pTransForm->m_vScale = { 1.2f, 1.f, 1.f };*/
	m_eCurState = IDLE;
	m_ePreState = STATE_END;
	m_ePlayerLookAt = LOOK_DOWN;

	m_eCurWeapon = UNARMED;
	m_ePreWeapon = WEAPON_END;

	m_Dirchange = false;
	m_fFrameEnd = 22;
	Set_Stat();
	return S_OK;
}
Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_ePreState == ATTACK)
			m_bAttack = false;
			
		m_fFrame = 0.f;
	}

	Key_Input(fTimeDelta);
	Check_State();
	Set_Scale();
	Fire_Light(); // 횃불 모션 시 켜짐 / 꺼짐 추가해야함
	CGameObject::Update_GameObject(fTimeDelta);

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	/*Engine::IsPermit_Call(L"Unarmed_IDLE", fTimeDelta);*/
	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3 vPos;
	BillBoard();
	m_pTransForm->Get_Info(INFO::INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTextureCom[m_ePlayerLookAt][m_ePreState]->Set_Texture((_uint)m_fFrame);

	FAILED_CHECK_RETURN(SetUp_Material(), );

	if (m_Dirchange)
	{
		m_pReverseCom->Render_Buffer();
	}
	else if (!m_Dirchange)
	{
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM

	//IDLE
	pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_idle_side", pComponent });


	//RUN(UNARMED)
	pComponent = m_pTextureCom[LOOK_DOWN][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][MOVE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Unarmed_run_side", pComponent });

	//Build
	pComponent = m_pTextureCom[LOOK_DOWN][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][BUILD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_build_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_build_side", pComponent });

	//Pickup
	pComponent = m_pTextureCom[LOOK_UP][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_up", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_down", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][PICKUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_pickup_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_pickup_side", pComponent });


	//Hit
	pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_hit_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_hit_side", pComponent });


	//Attack
	pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Unarmed_punch_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Unarmed_punch_side", pComponent });

	//Torch(IDLE)
	pComponent = m_pTextureCom[LOOK_DOWN][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][TORCH_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_idle_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_idle_side", pComponent });

	//Torch(Run)
	pComponent = m_pTextureCom[LOOK_DOWN][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_down", pComponent });

	pComponent = m_pTextureCom[LOOK_UP][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_up"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_up", pComponent });

	pComponent = m_pTextureCom[LOOK_RIGHT][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_side", pComponent });

	pComponent = m_pTextureCom[LOOK_LEFT][TORCH_RUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_Torch_run_side"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_Torch_run_side", pComponent });


	//Other
	pComponent = m_pTextureCom[LOOK_DOWN][FALLDOWN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_falldown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_falldown", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][WAKEUP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_wakeup"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_wakeup", pComponent });

	pComponent = m_pTextureCom[LOOK_DOWN][EAT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Player_eat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Player_eat", pComponent });
#pragma endregion TEXCOM


	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Pos(0.f, 1.f, 0.f);

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev, _strName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	_vec3		vDir, vRight, vCurPos;
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	m_pTransForm->Get_Info(INFO_LOOK, &vDir);
	m_pTransForm->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState('W'))
	{
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);

		if (m_eCurWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else if (m_eCurWeapon == UNARMED)
		{
			m_eCurState = MOVE;
		}
		m_ePlayerLookAt = LOOK_UP;
	}

	if (KEY_TAP(DIK_X))// KEY_TAP(누르는시점) , KEY_AWAY (키를떼는시점), KEY_NONE(키를안누른상태), KEY_HOLD(키를누르고있는상태)
	{
		//Find_NeerObject: 못찾았을경우 nullptr반환
		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::ITEM);

		if (nullptr != findObj)
			DeleteObject(findObj);
	}

	if (GetAsyncKeyState('Z'))
	{
		//Find_NeerObject: 못찾았을경우 nullptr반환

		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::RESOURCE_OBJECT);
		if (nullptr != findObj)
			DeleteObject(findObj);
	}

	if (GetAsyncKeyState('S'))
	{ //f
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vDir, -m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Move_Pos(&vDir, m_Stat.fSpeed, fTimeDelta);
		if (m_eCurWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else if(m_eCurWeapon == UNARMED)
		{
			m_eCurState = MOVE;
		}
		m_ePlayerLookAt = LOOK_DOWN;

	}

	if (GetAsyncKeyState('A'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vRight, -m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		vCurPos.x += 0.5f;
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);

		if (m_eCurWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else if (m_eCurWeapon == UNARMED)
		{
			m_eCurState = MOVE;
		}
		m_ePlayerLookAt = LOOK_LEFT;
		if (!m_Dirchange)
		{
			m_Dirchange = true;
		}
		/*m_pTransForm->Rotation(ROT_Y, D3DXToRadian(90.f * fTimeDelta));*/
	}

	if (GetAsyncKeyState('D'))
	{
		D3DXVec3Normalize(&vRight, &vRight);
		vDir.y = 0.f;
		m_pTransForm->Move_Pos(&vRight, m_Stat.fSpeed, fTimeDelta);
		m_pTransForm->Get_Info(INFO_POS, &vCurPos);
		vCurPos.x -= 0.5f;
		if (!m_pCalculatorCom->Check_PlayerMoveIndex(&vCurPos, pTerrainTex->Get_VecPos()))
			m_pTransForm->Set_Pos(vCurPos);

		if (m_eCurWeapon == TORCH)
		{
			m_eCurState = TORCH_RUN;
		}
		else if (m_eCurWeapon == UNARMED)
		{
			m_eCurState = MOVE;
		}
		m_ePlayerLookAt = LOOK_RIGHT;
		if (m_Dirchange)
		{
			m_Dirchange = false;
		}
		/*m_pTransForm->Rotation(ROT_Y, D3DXToRadian(-90.f * fTimeDelta));*/
	}

	if (!GetAsyncKeyState('W') &&
		!GetAsyncKeyState('A') &&
		!GetAsyncKeyState('S') &&
		!GetAsyncKeyState('D'))
	{

		if (m_eCurWeapon == TORCH)
		{
			m_eCurState = TORCH_IDLE;
		}
		else if (m_eCurWeapon == UNARMED)
		{
			m_eCurState = IDLE;
		}
	}

	if (GetAsyncKeyState('F')) // 공격
	{
		m_eCurState = ATTACK;
		CGameObject* findObj = Find_NeerObject(m_Stat.fAggroRange, eOBJECT_GROUPTYPE::MONSTER);
		if (findObj->IsDelete() != true && dynamic_cast<CMonster*>(findObj)->IsTarget_Approach(m_Stat.fATKRange))
		{
			dynamic_cast<CMonster*>(findObj)->Set_Attack(m_Stat.fATK);
		}
		//auto vecMonster = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);

		//_vec3* vMonsterAxis = new _vec3[3];
		//_vec3* vPlayerAxis = new _vec3[3];
		//_vec3 vPlayerPos, vMonsterPos, vPlayerScale, vMonsterScale;

		//m_pTransForm->Get_Info(INFO_POS, &vPlayerPos);
		//m_pTransForm->Get_Info(INFO_RIGHT, &vPlayerAxis[0]);
		//m_pTransForm->Get_Info(INFO_UP, &vPlayerAxis[1]);
		//m_pTransForm->Get_Info(INFO_LOOK, &vPlayerAxis[2]);

		//vPlayerScale = m_pTransForm->Get_Scale();

		//for (auto& monster : vecMonster)
		//{
		//	CTransform* pItemTransform = monster->GetTransForm();
		//	pItemTransform->Get_Info(INFO_POS, &vMonsterPos);
		//	pItemTransform->Get_Info(INFO_RIGHT, &vMonsterAxis[0]);
		//	pItemTransform->Get_Info(INFO_UP, &vMonsterAxis[1]);
		//	pItemTransform->Get_Info(INFO_LOOK, &vMonsterAxis[2]);
		//	vMonsterScale = pItemTransform->Get_Scale();

		//	if (!m_bAttack && Engine::Collision_Monster(vPlayerPos,
		//		vPlayerAxis, 
		//		vMonsterPos,
		//		vMonsterAxis, 
		//		vPlayerScale, 
		//		vMonsterScale))
		//	{
		//		// 몬스터와 공격 충돌 시
		//		// 몬스터 채력이 깎임. -> 몬스터 공격 한번만 되도록

		//		m_pTransForm->Set_Scale(_vec3{ 0.2f, 0.2f, 0.2f });
		//		dynamic_cast<CMonster*>(monster)->Set_Attack(10.f);
		//		m_bAttack = true;
		//		break;
		//	}
		//}

		//delete[] vMonsterAxis;
		//delete[] vPlayerAxis;
	}

	if (GetAsyncKeyState('G')) // 횃불
	{
		m_eCurWeapon = TORCH;
	}

	if (GetAsyncKeyState('V')) // 줍기
	{
		m_eCurState = PICKUP;

		auto pLayer = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM);
		_vec3 vPlayerPos, vPlayerScale, vItemPos, vItemScale;
		m_pTransForm->Get_Info(INFO_POS, &vPlayerPos);
		vPlayerScale = m_pTransForm->Get_Scale();

		for (auto& object : pLayer)
		{
			//_vec3 pPlayerPos, _vec3 pItemPos, _vec3 vPlayerScale, _vec3 vItemScale
			CTransform* pItemTransform = object->GetTransForm();
			pItemTransform->Get_Info(INFO_POS, &vItemPos);
			vItemScale = pItemTransform->Get_Scale();

			if (Engine::Collision_Item(vPlayerPos, vItemPos, vPlayerScale, vItemScale))
				m_pTransForm->Set_Scale(_vec3{ 0.2f, 0.2f, 0.2f });
		}
	}
	if (GetAsyncKeyState('B'))
	{
		m_eCurState = BUILD;
	}
	if (GetAsyncKeyState('N'))
	{
		m_eCurState = FALLDOWN;
		m_ePlayerLookAt = LOOK_DOWN;
	}
	if (GetAsyncKeyState('M'))
	{
		m_eCurState = WAKEUP;
		m_ePlayerLookAt = LOOK_DOWN;
	}
	if (GetAsyncKeyState('H'))
	{
		m_eCurState = EAT;
		m_ePlayerLookAt = LOOK_DOWN;
	}


	//if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	//{
	//	m_eCurState = MOVE;
	//	m_ePlayerLookAt = m_pTransForm->Patroll_LookChange(m_pCalculatorCom->Picking_PosVec(g_hWnd), m_Stat.fSpeed, fTimeDelta);
	//}

}

HRESULT CPlayer::SetUp_Material()
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;
	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

void CPlayer::Height_OnTerrain()
{
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();
	_vec3		vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

	m_pTransForm->Set_Pos(vPos.x, fHeight + 1.5f, vPos.z);
}

_vec3 CPlayer::Picking_OnTerrain()
{
	auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(pTerrain->GetTransForm());
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CPlayer::Check_State()
{
	if (m_ePreState != m_eCurState)
	{
		if (m_eCurState == IDLE)
		{
			m_fFrameEnd = 22;
			
		}
		else if (m_eCurState == HIT)
		{
			m_fFrameEnd = 7;
		}
		else if (m_eCurState == ATTACK)
		{
			m_fFrameEnd = 11;
		}
		else if (m_eCurState == FALLDOWN)
		{
			m_fFrameEnd = 8;
		}
		else if (m_eCurState == WAKEUP)
		{
			m_fFrameEnd = 32;
		}
		else if (m_eCurState == EAT)
		{
			m_fFrameEnd = 36;
		}
		else if (m_eCurState == MOVE)
		{
			m_fFrameEnd = 6;
		}
		else if (m_eCurState == TORCH_IDLE)
		{
			m_fFrameEnd = 22;
		}
		else if (m_eCurState == TORCH_RUN)
		{
			m_fFrameEnd = 6;
		}
		else
		{
			m_fFrameEnd = 6;
		}
		m_ePreState = m_eCurState;
		m_fFrame = 0.f;
	}
	else
		return;

}

void CPlayer::Set_Scale()
{
	if (m_eCurState == BUILD) //B
		m_pTransForm->m_vScale = { 0.85f, 0.7f, 0.85f };

	else if ((m_ePlayerLookAt == LOOK_LEFT || m_ePlayerLookAt == LOOK_RIGHT) && m_eCurState == PICKUP)
		m_pTransForm->m_vScale = { 1.0f, 0.1f, 1.0f };

	else if (m_eCurState == PICKUP)
		m_pTransForm->m_vScale = { 0.73f, 0.63f, 0.73f };

	else if ((m_ePlayerLookAt == LOOK_LEFT || m_ePlayerLookAt == LOOK_RIGHT) && m_eCurState == ATTACK)
		m_pTransForm->m_vScale = { 1.f, 0.3f, 1.f };

	else if (m_ePlayerLookAt == LOOK_UP && m_eCurState == ATTACK)
		m_pTransForm->m_vScale = { 0.8f, 0.3f, 0.8f };

	else if (m_eCurState == ATTACK)
		m_pTransForm->m_vScale = { 0.85f, 0.8f, 0.85f };

	else if (m_eCurState == HIT) //H
		m_pTransForm->m_vScale = { 0.9f, 1.f, 0.9f };

	else if (m_eCurState == FALLDOWN)
		m_pTransForm->m_vScale = { 1.f, 1.f,1.0f };

	else if (m_eCurState == WAKEUP)
		m_pTransForm->m_vScale = { 1.1f, 1.f, 1.1f };

	else if (m_eCurState == EAT) // H
		m_pTransForm->m_vScale = { 1.1f, 0.3f, 1.1f };

	else if (m_eCurState == MOVE && (m_ePlayerLookAt == LOOK_LEFT || m_ePlayerLookAt == LOOK_RIGHT))
		m_pTransForm->m_vScale = { 0.9f, 0.6f, 0.8f };

	else if (m_eCurState == MOVE)
		m_pTransForm->m_vScale = { 0.7f, 1.f, 0.7f };

	else
		m_pTransForm->m_vScale = { 0.7f, 0.5f, 0.7f };

}

void CPlayer::Set_Stat()
{
	m_Stat.fHP = 150.f;
	m_Stat.fMxHP = 150.f;
	m_Stat.fSpeed = 5.f;
	m_Stat.fATK = 10.f;
	m_Stat.fATKRange = 2.f;
	m_Stat.fAggroRange = 5.f;
	m_Stat.bDead = false;

}

HRESULT CPlayer::Ready_Light()
{
	//점광원
// 최초 생성 후 플레이어 횟불 사용 시에만 켜지도록 
	D3DLIGHT9 tPointLightInfo;
	ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo.Type = D3DLIGHT_POINT;

	tPointLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tPointLightInfo.Attenuation0 = 0.00000001f;
	tPointLightInfo.Range = 5.f;
	tPointLightInfo.Position = { 0.f, 0.f, 0.f };

	FAILED_CHECK_RETURN(light::Ready_Light(m_pGraphicDev, &tPointLightInfo, 1), E_FAIL);
	light::Get_Light(1)->Close_Light();

}

void CPlayer::Fire_Light()
{
	if (m_eCurWeapon != TORCH)
		return ;

	D3DLIGHT9* tPointLightInfo = light::Get_Light(1)->Get_Light();
	//ZeroMemory(&tPointLightInfo, sizeof(D3DLIGHT9));

	tPointLightInfo->Type = D3DLIGHT_POINT;

	tPointLightInfo->Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.6f);
	tPointLightInfo->Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.6f);
	tPointLightInfo->Attenuation0 = 0.000000001f;
	tPointLightInfo->Range = 5.f;

	_vec3 pPlayerPos;
	m_pTransForm->Get_Info(INFO_POS, &pPlayerPos); // player pos 값 설정
	tPointLightInfo->Position = pPlayerPos;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);
	light::Get_Light(1)->Update_Light();
}


//못찾았을경우 nullptr반환
CGameObject* CPlayer::Find_NeerObject(float _fRange, eOBJECT_GROUPTYPE _findTarget)
{
	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, _findTarget);

	_vec3 vPlayerPos, vTargetPos, vLook;
	_float fNeerDistance = FLT_MAX;
	_float fFindDistance = 0.f;
	CGameObject* pFindObject = nullptr;

	ZeroMemory(vPlayerPos, sizeof(_vec3));
	ZeroMemory(vTargetPos, sizeof(_vec3));
	ZeroMemory(vLook, sizeof(_vec3));

	m_pTransForm->Get_Info(INFO_POS, &vPlayerPos);//플레이어좌표 구하기
	vPlayerPos.y = 0.f;

	//반복문돌면서 모든 오브젝트그룹 순회하기
	for (auto& obj : vecObj)
	{
		if (obj->IsDelete())//해당오브젝트가 삭제될예정이면 무시
			continue;

		obj->GetTransForm()->Get_Info(INFO_POS, &vTargetPos);
		vTargetPos.y = 0.f;

		vLook = vTargetPos - vPlayerPos;

		fFindDistance = D3DXVec3Length(&vLook);
		if (_fRange < fFindDistance)//현재 찾은거리가 범위보다 클경우
			continue;

		if (fNeerDistance > fFindDistance)//현재 찾은 거리가 이전에찾았던 가장가까운거리보다 작을경우
		{
			pFindObject = obj;
			fNeerDistance = fFindDistance;
		}


	}

	return pFindObject;
}


void CPlayer::BillBoard()
{
	_matrix	matWorld, matView, matBill;

	m_pTransForm->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill._23 = matView._23;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransForm->Set_WorldMatrix(&(matBill * matWorld));
}



void CPlayer::Free()
{

	__super::Free();
}

//DeleteObject(삭제할게임오브젝트포인터 (GameObject*));
//CreateObject(레이어타입,오브젝트타입,생성한 오브젝트포인터);
