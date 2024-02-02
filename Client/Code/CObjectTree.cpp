#include "CObjectTree.h"

#include "Export_System.h"
#include "Export_Utility.h"

CObjectTree::CObjectTree(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CObjectTree::CObjectTree(const CObjectTree& rhs)
	: CResObject(rhs.m_pGraphicDev)
{
}

CObjectTree::~CObjectTree()
{
}

HRESULT CObjectTree::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransForm->Set_Pos(_vec3(rand() % 20, 1.5f, rand() % 20));

	m_eCurState = RES_IDLE;
	m_fFrame = 0.f;
	m_fFrameEnd = 27.f;
	Ready_Stat();

	return S_OK;
}

_int CObjectTree::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_eCurState == RES_HIT_1) // 피격 모션이 끝난 후 IDLE로 돌아감
		{
			m_eCurState = RES_IDLE;
			m_bHit = false;
		}

		else if (m_eCurState == RES_DEAD || m_eCurState == RES_DEAD2)
			m_Stat.bDead = true;

		m_fFrame = 0.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);

	m_pTransForm->BillBoard();

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectTree::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	Change_Frame_Event();
	Check_FrameState();
	//_vec3	vPos;
	//m_pTransForm->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);
}

void CObjectTree::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CObjectTree::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom[RES_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Tree_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree_Idle", pComponent });

	pComponent = m_pTextureCom[RES_HIT_1] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Tree_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree_Hit", pComponent });

	pComponent = m_pTextureCom[RES_DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Tree_Right_Fall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree_Right_Fall", pComponent });

	pComponent = m_pTextureCom[RES_DEAD2] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Tree_Left_Fall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree_Left_Fall", pComponent });

	pComponent = m_pTextureCom[RES_FINAL] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Tree_Final"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Tree_Final", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.2f, vPos.z);

	return S_OK;
}

void CObjectTree::Change_Frame_Event()
{
	if (m_eCurState == RES_FINAL)
		return;

	//HIT 시 상태값
	if (m_bHit)
		m_eCurState = RES_HIT_1;

	if (m_Stat.fHP <= 0) // 체력이 0일때
	{
		if (m_ePlayerLook == LOOK_LEFT)
			m_eCurState = RES_DEAD2;
		else
			m_eCurState = RES_DEAD;
	}

	if (m_Stat.bDead)
		m_eCurState = RES_FINAL;
}

void CObjectTree::Check_FrameState()
{
	if (m_ePreState == m_eCurState)
		return;

	if (m_eCurState == RES_IDLE)
		m_fFrameEnd = 27;

	if (m_eCurState == RES_HIT_1)
		m_fFrameEnd = 6;

	if (m_eCurState == RES_DEAD) //위치 조정 필요
	{
		m_fFrameEnd = 13;
		m_pTransForm->Set_Scale(_vec3(3.5f, 3.5f, 3.5f));
	}
	if (m_eCurState == RES_DEAD2)
	{
		m_fFrameEnd = 13;
		m_pTransForm->Set_Scale(_vec3(2.f, 2.f, 2.f));
	}
	if (m_eCurState == RES_FINAL)
	{
		m_fFrameEnd = 0;
		m_pTransForm->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
	}

	m_ePreState = m_eCurState;
	m_fFrame = 0.f;
}

void CObjectTree::Ready_Stat()
{
	m_Stat.fHP = 6.f;
	m_Stat.fMxHP = 6.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}

CResObject* CObjectTree::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CObjectTree(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectTree::Free()
{
	CGameObject::Free();
}


