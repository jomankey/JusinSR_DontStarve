#include "ItemBasic.h"
#include "Export_Utility.h"
#include "stdafx.h"
#include "SlotMgr.h"
#include <Terrain.h>

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev), m_bChangeRander(false)
{
}

CItemBasic::CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	: CItem(pGraphicDev, _strObjName)
{
}

CItemBasic::CItemBasic(const CItem& rhs)
	: CItem(rhs)
{
}

CItemBasic::~CItemBasic()
{
}

void CItemBasic::Pickup_Item(_vec3 vSlotPos)
{
	m_vPos = m_pTransForm->Get_Pos();
	m_pCalculatorCom->Change_OnObjectMatrix(g_hWnd, &m_vPos);
	m_bChangeRander = true;

	m_vSlotPos = vSlotPos;

	m_fX = m_vPos.x;
	m_fY = m_vPos.y;

	m_fSizeX = 25.f;
	m_fSizeY = 25.f;
	

	m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.0f));
	m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	//m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(180.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);
}

HRESULT CItemBasic::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_ObjState();
	m_vUp = { 0,1,0 };
	int randomValue = rand() % 360;
	int randomValue2 = rand() % 360;
	// 부호를 무작위로 선택 (-1 또는 1)
	int sign = (rand() % 2 == 0) ? 1 : -1;
	int sign2 = (rand() % 2 == 0) ? 1 : -1;

	// 랜덤값에 부호를 적용
	int result = randomValue * sign;
	int result2 = randomValue2 * sign2;
	m_vDir = { (float)result,0.f,(float)result2 };
	D3DXVec3Normalize(&m_vDir, &m_vDir);




    return S_OK;
}

_int CItemBasic::Update_GameObject(const _float& fTimeDelta)
{

	CGameObject::Update_GameObject(fTimeDelta);

	if (m_bChangeRander)
	{
		if (m_vSlotPos.y - m_fY <= 0.5f)
		{
			_vec3 vSlotPos = {};
			CSlotMgr::GetInstance()->AddItem(m_pGraphicDev, m_strObjName, &vSlotPos);
			DeleteObject(this);
			//return 0x80000000;
		}

		// 아이템 먹엇을때 슬롯쪽으로 이동
		m_fX += (m_vSlotPos.x - m_fX) * 5.f * fTimeDelta;
		m_fY += (m_vSlotPos.y - m_fY) * 5.f * fTimeDelta;

		m_pTransForm->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.1f));
		m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));
	}

	MousePicking();

	if (m_bIsCreateByObject)
	{
		DropMotion(fTimeDelta);
	}

	renderer::Add_RenderGroup(m_bChangeRander ? RENDER_UI : RENDER_ALPHA, this);

	return 0;
}

void CItemBasic::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	m_pTransForm->BillBoard();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);
}

void CItemBasic::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	if (m_bChangeRander)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (!m_bChangeRander) m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);
	if (!m_bChangeRander) FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	if (!m_bChangeRander) m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	scenemgr::Get_CurScene()->EndOrtho();
}

void CItemBasic::DropMotion(const _float& fTimeDelta)
{
<<<<<<< HEAD
	const float gravity = 9.8f;
	_vec3 vDirUp;
	_vec3 vDirRight;
	_vec3 vDirLook;
	_vec3 vOrigin;

	// 아이템의 초기 위치 및 방향 정보 가져오기
	_float fSpeed = 20.f;

	m_pTransForm->Get_Info(INFO_POS, &vOrigin);
	
	m_pTransForm->Get_Info(INFO_UP, &vDirUp);
	m_pTransForm->Get_Info(INFO_RIGHT, &vDirRight);
	m_pTransForm->Get_Info(INFO_LOOK, &vDirLook);

	if (!bHowFar)
	{
		m_pTransForm->Move_Pos(&vDirUp, fSpeed, fTimeDelta);
		if (vOrigin.y > 0.8f)   // 어디까지 올라갈 것인가
		{
			bHowFar = true;
		}
	}
	else // 아이템이 아래로 떨어지는 동안 중력의 영향을 받아 포물선 운동
	{
		if (vOrigin.y < 0.3f)   // 어디까지 내려갈 것인가
		{
			m_pTransForm->Set_Pos(vOrigin.x+iPos, vOrigin.y, vOrigin.z+iPos);
			m_bIsCreateByObject = false;
			return;
		}

		// 아이템의 속도에 중력 가속도를 더하여 포물선 운동 구현
		fItemSpeed += gravity * fTimeDelta;
		m_pTransForm->Move_Pos(&(-vDirUp * fItemSpeed), fSpeed, fTimeDelta);
	}

	// x축으로 이동
	//if(!m_bHowToDir)
	//	m_pTransForm->Move_Pos(&(vDirLook * m_iSign), fSpeed * 0.5f, fTimeDelta);
	//else{
	//	// z축으로 이동
	m_pTransForm->Move_Pos(&(vDirRight * m_iSign), fSpeed * 0.5f, fTimeDelta);//여기서는 속도가 거리임
	//}





	//_vec3	vDirUp;
	//_vec3	vDirRight;
	//_vec3	vDirLook;
	//_vec3   vOrigin;
	//

	//_float fSpeed = 10.f;


	//m_pTransForm->Get_Info(INFO_POS, &vOrigin);
	//m_pTransForm->Get_Info(INFO_UP, &vDirUp);
	//m_pTransForm->Get_Info(INFO_RIGHT, &vDirRight);
	//m_pTransForm->Get_Info(INFO_LOOK, &vDirLook);

	//

	//if (bHowFar)
	//{
	//	m_pTransForm->Move_Pos(&-vDirUp, fSpeed, fTimeDelta);
	//	if (vOrigin.y < 0.7f)	//어디까지 내려갈 것인가
	//	{
	//		m_bIsCreateByObject = false;
	//	}
	//}
	//else if(!bHowFar)
	//{
	//	m_pTransForm->Move_Pos(&vDirUp, fSpeed, fTimeDelta);
	//	if (vOrigin.y > 1.0f)	//어디까지 올라갈 것인가
	//	{
	//		bHowFar = true;
	//	}
	//}

	//m_pTransForm->Move_Pos(&(vDirRight* m_iSign), fSpeed*0.5,  fTimeDelta);



	////m_pTransForm->Move_Pos(&(vDirLook* m_iSign), fSpeed,  fTimeDelta);
	//


=======
	const float Gravity = 9.8f;
	_vec3 vUp;
	//m_pTransForm->Get_Info(INFO_UP, &vUp);
>>>>>>> d51dad7016a6a512f22882391b6a04cb4a92f719

	m_pTransForm->Move_Pos(&m_vDir, 0.7f, fTimeDelta);
	m_pTransForm->Move_Pos(&m_vUp, 10.f, fTimeDelta);
	

	m_vUp.y-= 2.f * fTimeDelta;

	if (m_pTransForm->Get_Pos().y < 0)
		m_bIsCreateByObject = false;
	
}


CItem* CItemBasic::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _szName)
{
	CItem* pInstance = new CItemBasic(pGraphicDev, _szName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItemBasic::Free()
{
	__super::Free();
}
