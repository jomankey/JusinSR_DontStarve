#include "stdafx.h"
#include "..\Header\MainApp.h"

#include "Logo.h"
#include "Stage.h"
#include "UIMgr.h"
#include "SlotMgr.h"

CMainApp::CMainApp() : m_pDeviceClass(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_Setting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Event(m_pGraphicDev, &m_pEventMgrClass), E_FAIL);
	//01:23
	//test
	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{

	Engine::Update_InputDev();
	Engine::Update_MouseState();
	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
	m_pManagementClass->LateUpdate_Scene();
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	Engine::Render_End();

	m_pEventMgrClass->EventUpdate();

}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CSceneMgr** ppManagement)
{
	Engine::CScene* pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	//매니지먼트 싱글톤의 주소 받아오기
	FAILED_CHECK_RETURN(scenemgr::Create_SceneMgr(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Change_Scene(pScene), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Event(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CEventMgr** ppEventMgr)
{
	//매니지먼트 싱글톤의 주소 받아오기
	FAILED_CHECK_RETURN(eventmgr::Create_EvnetMgr(pGraphicDev, ppEventMgr), E_FAIL);
	(*ppEventMgr)->AddRef();

	return S_OK;
}

HRESULT CMainApp::SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	//(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		  // Z버퍼에 깊이 값을 기록은 하지만 자동 정렬을 수행할지 말지 결정
	//(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 픽셀의 깊이 값을 저장할지 말지 결정
	
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Mapo배낭여행", L"Loading_Key1", 10, 22, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Mapo배낭여행", L"Loading_Key2", 10, 22, FW_HEAVY), E_FAIL);

	//owner Ui 
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Mapo배낭여행", L"Font_Count", 7, 7, FW_THIN), E_FAIL);

	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Mapo배낭여행",  L"Panel_Title", 20, 20, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Mapo배낭여행", L"Panel_Info",  12, 12, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Mapo배낭여행", L"Button_Make", 18, 18, FW_THIN), E_FAIL);

	//Mouse
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Mapo배낭여행", L"Mouse_Title", 21, 21, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Mapo배낭여행", L"Mouse_Sub", 19, 19, FW_HEAVY), E_FAIL);

	// Dinput
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	//PathMgr
	FAILED_CHECK_RETURN(Engine::Ready_PathMgr(), E_FAIL);

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
	
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);
	Engine::Safe_Release(m_pGraphicDev);
	CUIMgr::DestroyInstance();
	CSlotMgr::DestroyInstance();
	Engine::Release_System();
	Engine::Release_Utility();
}
