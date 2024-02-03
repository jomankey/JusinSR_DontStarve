#include "BerryBush.h"

CBerryBush::CBerryBush(LPDIRECT3DDEVICE9 pGrahpicDev)
	: CResObject(pGrahpicDev),
	m_eBerryCurState(BERRY_END),
	m_eBerryPreState(BERRY_END)
{
}

CBerryBush::~CBerryBush()
{
}

HRESULT CBerryBush::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eBerryCurState = BERRY_IDLE;
	m_fFrame = 0.f;
	m_fFrameEnd = 0.f;
	Ready_Stat();

	return S_OK;
}

_int CBerryBush::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += m_fFrameEnd * fTimeDelta;

	if (m_fFrameEnd < m_fFrame)
	{
		if (m_eBerryCurState == BERRY_PICK || m_eBerryCurState == BERRY_EMPTY_PICK)
			m_eBerryCurState = BERRY_EMPTY;

		m_fFrame = 0.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);

	m_pTransForm->BillBoard();

	renderer::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBerryBush::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	Change_Frame_Event();
	Check_FrameState();
}

void CBerryBush::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pBerryTextureCom[m_eBerryCurState]->Set_Texture((int)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBerryBush::Change_Frame_Event()
{
	if (m_bHit && m_eBerryCurState == BERRY_IDLE)
		m_eBerryCurState = BERRY_PICK;
	else if (m_bHit && m_eBerryCurState == BERRY_EMPTY)
		m_eBerryCurState = BERRY_EMPTY_PICK;
	else if (m_Stat.fHP <= 0)
		m_eBerryCurState = BERRY_EMPTY;
}

void CBerryBush::Check_FrameState()
{
	if (m_eBerryPreState == m_eBerryCurState)
		return;

	if (m_eBerryCurState == BERRY_IDLE || m_eBerryCurState == BERRY_EMPTY)
		m_fFrameEnd = 0;
	if (m_eBerryCurState == BERRY_PICK)
		m_fFrameEnd = 3;
	if (m_eBerryCurState == BERRY_EMPTY_PICK)
		m_fFrameEnd = 5;

	m_eBerryPreState = m_eBerryCurState;
	m_fFrame = 0;
}

void CBerryBush::Ready_Stat()
{
	m_Stat.fHP = 1.f;
	m_Stat.fMxHP = 1.f;
	m_Stat.fSpeed = 1.f;
	m_Stat.bDead = false;
}

HRESULT CBerryBush::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_most_idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_most_idle", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_PICK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_most_pick"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_most_pick", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_empty_idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_empty_idle", pComponent });

	pComponent = m_pBerryTextureCom[BERRY_EMPTY_PICK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_empty_pick"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_empty_pick", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(1.5f, 1.5, 1.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.2f, vPos.z);

	return S_OK;
}

CResObject* CBerryBush::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResObject* pInstance = new CBerryBush(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBerryBush::Free()
{
	CGameObject::Free();
}

