#include "CObjectGrass.h"
#include "Export_System.h"
#include "Export_Utility.h"

CObjectGrass::CObjectGrass(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_eCurState(RESOBJECT_END)
	, m_ePreState(RESOBJECT_END)
{
}

CObjectGrass::CObjectGrass(const CObjectGrass& rhs)
	:CResObject(rhs.m_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_eCurState(rhs.m_eCurState)
	, m_ePreState(rhs.m_ePreState)
{
}

CObjectGrass::~CObjectGrass()
{
}

HRESULT CObjectGrass::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Ready_Stat();

	m_eCurState = RES_IDLE;
	m_fFrame = 0;
	m_fFrameEnd = 175;
	return S_OK;
}

_int CObjectGrass::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_eCurState == RES_DEAD)
			return 0;

		m_fFrame = 0.f;
	}
	
	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransForm->BillBoard();
	Check_FrameState();

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CObjectGrass::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	//_vec3	vPos;
	//m_pTransForm->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);
}

void CObjectGrass::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CObjectGrass::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom[RES_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Grass_idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Grass_idle", pComponent });

	pComponent = m_pTextureCom[RES_DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Obejct_Grass_dead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Obejct_Grass_dead", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	m_pTransForm->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos);

	return S_OK;
}

void CObjectGrass::Check_FrameState()
{
	if (m_ePreState == m_eCurState)
		return;

	if (m_eCurState == RES_IDLE)
		m_fFrameEnd = 175;
	if (m_eCurState == RES_DEAD)
		m_fFrameEnd = 40;

	m_ePreState = m_eCurState;
}

void CObjectGrass::Ready_Stat()
{
	m_Stat.fHP = 10;
	m_Stat.fMxHP = 10;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}

CObjectGrass* CObjectGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObjectGrass* pInstance = new CObjectGrass(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectGrass::Free()
{
	CGameObject::Free();
}
