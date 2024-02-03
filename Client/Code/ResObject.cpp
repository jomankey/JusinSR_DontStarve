#include "ResObject.h"

CResObject::CResObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_eCurState(RES_DEAD),
	m_ePreState(RES_DEAD),
	m_bHit(false),
	m_fFrame(0),
	m_fFrameEnd(0),
	m_ePlayerLook(LOOK_END)
{
}

CResObject::CResObject(const CResObject& rhs)
	: CGameObject(rhs.m_pGraphicDev),
	m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState),
	m_bHit(false),
	m_fFrame(rhs.m_fFrame),
	m_fFrameEnd(rhs.m_fFrameEnd),
	m_ePlayerLook(rhs.m_ePlayerLook)
{
}

CResObject::~CResObject()
{
}

HRESULT CResObject::Ready_GameObject()
{
	return S_OK;
}

_int CResObject::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CResObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CResObject::Render_GameObject()
{
}

void CResObject::Free()
{
}