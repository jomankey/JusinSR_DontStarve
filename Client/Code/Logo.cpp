#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_System.h"
#include "Export_Utility.h"


#include "CBossScene.h"
#include "CRoadScene.h"
#include "CTestScene.h"

#include "Stage.h"
#include "Layer.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev,L"LOGO"), m_pLoading(nullptr)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	
	for (size_t i = 0; i < (int)eLAYER_TYPE::END; i++)
	{
		m_arrLayer[i] = CLayer::Create();
	}

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (KEY_TAP(DIK_Q))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev,L"STAGE");
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;
		}
		else if (KEY_TAP(DIK_W))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CRoadScene::Create(m_pGraphicDev, L"ROAD");
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;

		}
		else if (KEY_TAP(DIK_E))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CBossScene::Create(m_pGraphicDev, L"BOSS");
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;

		}
		else if (KEY_TAP(DIK_T))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CTestScene::Create(m_pGraphicDev, L"TEST");
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(scenemgr::Change_CurScene(pScene), E_FAIL);

			return 0;

		}		
	}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	Engine::Render_Font(L"Loading_Key1", m_pLoading->Get_String(), &_vec2(10.f, 300.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Prototype()
{
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_RvRcTex", CRvRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(proto::Ready_Proto(L"Proto_LogoTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/IU.jpg")), E_FAIL);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo* pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free()
{
	Safe_Release(m_pLoading);

	__super::Free();
}
