#include "..\..\Header\Scene.h"
#include "Camera.h"


CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pCamera(nullptr)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent* CScene::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto& iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

void CScene::BeginOrtho()
{
	if (nullptr != m_pCamera)
		m_pCamera->BeginOrtho();
}

void CScene::EndOrtho()
{
	if (nullptr != m_pCamera)
		m_pCamera->EndOrtho();
}

CLayer* CScene::Get_Layer(const _tchar* pKey)
{
	CLayer* pSecond = m_mapLayer.find(pKey)->second;

	if (nullptr == pSecond)
		return nullptr;

	return pSecond;
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
	for (auto& iter : m_mapLayer)
		iter.second->Update_Layer(fTimeDelta);

	return 0;
}

void CScene::LateUpdate_Scene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();
}



void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
