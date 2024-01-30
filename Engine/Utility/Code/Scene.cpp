#include "..\..\Header\Scene.h"
#include "Layer.h"	
#include "Camera.h"


CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strSceneName)
	: m_pGraphicDev(pGraphicDev)
	, m_strSceneName(_strSceneName)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

void CScene::BeginOrtho()
{
	m_pCamera->BeginOrtho();
}

void CScene::EndOrtho()
{
	m_pCamera->EndOrtho();
}

_matrix* CScene::Get_OrthoViewMatrix()
{
	return m_pCamera->Get_OrthoViewMatrix();
}

_matrix* CScene::Get_OrthoMatrix()
{
	return m_pCamera->Get_OrthoMatrix();
}

 
HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i]->UpdateLayer(fTimeDelta);
	}

	return 0;
}

void CScene::LateUpdate_Scene()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i]->LateUpdateLayer();
	}

}



void CScene::Free()
{
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		Safe_Release(m_arrLayer[i]);
	}

	Safe_Release(m_pGraphicDev);
}
