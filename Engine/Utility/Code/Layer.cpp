#include "..\..\Header\Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

HRESULT CLayer::AddGameObject(eOBJECT_GROUPTYPE _eObjType, CGameObject* pGameObject)
{
	//nullptr이면 오류;
	if (nullptr == pGameObject)
		return E_FAIL;
	m_vecObject[(int)_eObjType].push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::ReadyLayer()
{
	return S_OK;
}

_int CLayer::UpdateLayer(const _float& fTimeDelta)
{
	int		iResult = 0;

	for (size_t i = 0; i < (int)eOBJECT_GROUPTYPE::END; i++)
	{
		for (auto iter : m_vecObject[i])
		{
			iter;
			iResult = iter->Update_GameObject(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
	}

	return iResult;
}

void CLayer::LateUpdateLayer()
{

	for (size_t i = 0; i < (int)eOBJECT_GROUPTYPE::END; i++)
	{
		for (auto& iter : m_vecObject[i])
		{
			iter->LateUpdate_GameObject();
		}
	}
}


CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->ReadyLayer()))
	{
		Safe_Release(pLayer);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for (size_t i = 0; i < (int)eOBJECT_GROUPTYPE::END; i++)
	{
		for (auto& iter : m_vecObject[i])
		{
			Safe_Release(iter);
		}
		m_vecObject[i].clear();
	}
}
