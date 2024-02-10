#include "..\..\Header\GameObject.h"

#include "ProtoMgr.h"
#include "Transform.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_mapComponent{}
	, m_fViewZ(0.f)
	, m_pTransForm(nullptr)
	, m_bDelete(false)
	, m_strObjName(L"NONE")
	, m_fDiffY(0.f)
	, m_iSlotNum(0)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName)
	: m_pGraphicDev(pGraphicDev)
	, m_mapComponent{}
	, m_fViewZ(0.f)
	, m_pTransForm(nullptr)
	, m_bDelete(false)
	, m_strObjName(_strName)
	, m_fDiffY(0.f)
	, m_iSlotNum(0)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(&m_Stat, sizeof(OBJSTAT));
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_fViewZ(0.f)
	, m_pTransForm(nullptr)
	, m_bDelete(false)
	, m_Stat(rhs.m_Stat)
	, m_iSlotNum(rhs.m_iSlotNum)
	, m_fDiffY(0.f)
{
	m_mapComponent[ID_DYNAMIC] = rhs.m_mapComponent[ID_DYNAMIC];

	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}


HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject()
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();


}


CComponent* CGameObject::Find_Component(COMPONENTID eID, const _tchar* pComponentTag)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

HRESULT Engine::CGameObject::SetUp_Material(D3DXCOLOR vColor)
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	//tMtrl.Diffuse = D3DXCOLOR(CToolMgr::m_fMtrlDiffuseColor[0], CToolMgr::m_fMtrlDiffuseColor[1], CToolMgr::m_fMtrlDiffuseColor[2], 1.f);
	//tMtrl.Ambient = D3DXCOLOR(CToolMgr::m_fMtrlAmbientColor[0], CToolMgr::m_fMtrlAmbientColor[1], CToolMgr::m_fMtrlAmbientColor[2], 1.f);
	//tMtrl.Specular = D3DXCOLOR(CToolMgr::m_fMtrlSpecularColor[0], CToolMgr::m_fMtrlSpecularColor[1], CToolMgr::m_fMtrlSpecularColor[2], 1.f);
	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tMtrl.Emissive = vColor;
	tMtrl.Power = 0.f;
	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

void Engine::CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_vec3 vPos = *pPos;
	vPos.y = 0.f;
	vPos.y += m_fDiffY;

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - vPos));
}


void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}