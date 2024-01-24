#include "MainTool.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "Engine_Function.h"
#include "framework.h"

#include "ToolTerrain.h"



CMainTool::CMainTool()
{
}

CMainTool::~CMainTool()
{
}

HRESULT CMainTool::Ready_MainTool()
{
    FAILED_CHECK_RETURN(SetUp_Setting(&m_pGraphicDev), E_FAIL);
    Setup_ImGui();

    FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile_Grass_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass1/grass_%d.png",80)), E_FAIL);
    FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Tile_Grass_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Terrain/Grass2/grass2_%d.png", 80)), E_FAIL);
    
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogin"), E_FAIL);
    FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
    FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);

    m_pTileTexture.push_back(dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tile_Grass_1")));
    m_pTileTexture.push_back(dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tile_Grass_2")));

    return S_OK;
}

void CMainTool::Update_MainTool()
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //매 업데이트마다 돌아감
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;

    ImGui::ShowDemoWindow(&show_demo_window);
    Window_Tile();

    //ImGui::Begin("test");                          // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &show_another_window);

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //    counter++;
    //ImGui::SameLine();
    //ImGui::Text("counter = %d", counter);

    //// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //ImGui::End();
    //m_pTerrain->Update_GameObject(0.f);
    
}

void CMainTool::LateUpdate_MainTool()
{
    //m_pTerrain->LateUpdate_GameObject();
}

void CMainTool::Render_MainTool()
{
    Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    //m_pTerrain->Render_GameObject();
    Engine::Render_End();
}

void CMainTool::Setup_ImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);
}

void CMainTool::Window_Tile()
{
    //타일툴 
    ImGui::Begin("Tile Tool");

    ImGui::Text("Choose the tile and pick it");

    const char* Items[] = {"grass", "grass2"};
    static int item_current_idx = 0;

    if (ImGui::BeginListBox("Tile List"))
    {
        for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
        {
            const bool bSelected = (item_current_idx == i);
            if (ImGui::Selectable(Items[i], bSelected))
                item_current_idx = i;
            

            if (bSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }


    for (int i = 0; i < 80; i++)
    {
        ImTextureID my_tex_id = (LPDIRECT3DTEXTURE9*)m_pTileTexture[item_current_idx]->Get_Texture(i);
        float my_tex_w = 128.f;
        float my_tex_h = 128.f;

        ImGui::PushID(i);
        ImVec2 size = ImVec2(32.f, 32.f); 
        ImVec2 uv0 = ImVec2(0.f, 0.f); 
        ImVec2 uv1 = ImVec2(32.f / my_tex_w, 32.f / my_tex_h); 
        ImVec4 bg_col = ImVec4(255.f, 255.f, 255.f, 1.f); // 색상값 
        ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.f); 
        ImGui::ImageButton("", (void*)my_tex_id, size, uv0, uv1, bg_col, tint_col);
         
        ImGui::PopID();
        if (i % 5 == 0 && i != 0)
            ImGui::NewLine();
        else
            ImGui::SameLine();
    }

    ImGui::End();
}

HRESULT CMainTool::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
    //Engine::CLayer* pLayer = Engine::CLayer::Create();
    //NULL_CHECK_RETURN(pLayer, E_FAIL);

    //Engine::CGameObject* pGameObject = nullptr;

    //m_pTerrain = CToolTerrain::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

    //m_mapLayer.insert({ pLayerTag, pLayer });


    return S_OK;
}

HRESULT CMainTool::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement)
{
    //Engine::CScene* pScene = nullptr;

    //pScene = CLogo::Create(pGraphicDev);
    //NULL_CHECK_RETURN(pScene, E_FAIL);

    //FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
    //(*ppManagement)->AddRef();

    //FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

    return S_OK;
}

HRESULT CMainTool::SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
    FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, TOOL_WINCX, TOOL_WINCY, &m_pDeviceClass), E_FAIL);
    m_pDeviceClass->AddRef();

    (*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
    (*ppGraphicDev)->AddRef();

    (*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

    //(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		  // Z버퍼에 깊이 값을 기록은 하지만 자동 정렬을 수행할지 말지 결정
    //(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 픽셀의 깊이 값을 저장할지 말지 결정

    FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕체", 30, 30, FW_HEAVY), E_FAIL);

    // Dinput
    //FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

    (*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    (*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

char* CMainTool::ConvertWCtoC(_tchar* str)
{
    char* pStr;
    int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    pStr = new char[strSize];
    WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

    return pStr;
}

CMainTool* CMainTool::Create()
{
    CMainTool* pInstance = new CMainTool;

    if (FAILED(pInstance->Ready_MainTool()))
    {
        //Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CMainTool::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Engine::Safe_Release(m_pDeviceClass);
    Engine::Safe_Release(m_pManagementClass);
    Engine::Safe_Release(m_pGraphicDev);
    //Engine::Safe_Release(m_pTerrain);

    Engine::Release_Utility();
    Engine::Release_System();
}
