#include "ToolMgr.h"
#include "framework.h"

#include "Export_Utility.h"

//TODO:: 타일 클래스 따로 뺴기
_float CToolMgr::m_fMtrlDiffuseColor[3] = { 0.f, 0.f, 0.f };
_float CToolMgr::m_fMtrlAmbientColor[3] = { 0.f, 0.f, 0.f };
_float CToolMgr::m_fMtrlSpecularColor[3] = { 0.f, 0.f, 0.f };

_float CToolMgr::m_fDirectionDiffuseColor[3] = { 0.f, 0.f, 0.f };
_float CToolMgr::m_fDirectionAmbientColor[3] = { 0.f, 0.f, 0.f };
_float CToolMgr::m_fDirectionSpecularColor[3] = { 0.f, 0.f, 0.f };

CToolMgr::CToolMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), item_current_idx(0)
{
}

CToolMgr::~CToolMgr()
{
}

HRESULT CToolMgr::Setup_Imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

    m_pTileTexture.push_back(dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tile_Grass_1")));
    m_pTileTexture.push_back(dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tile_Grass_2")));

	return S_OK;
}

void CToolMgr::Update_ToolMgr()
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

    //ImGui::ShowDemoWindow(&show_demo_window);
    Window_Tile();
    Window_Light();
}

void CToolMgr::Render_ToolMgr()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//타일툴
void CToolMgr::Window_Tile()
{
    ImGui::Begin("Tile Tool");

    ImGui::Text("Choose the tile and pick it");

    const char* Items[] = { "grass", "grass2" };


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

//조명 툴
void CToolMgr::Window_Light()
{
    ImGui::Begin("Light");

    ImGui::Text("Mtrl");
    ImGui::ColorEdit3("MtrlDiffuse", m_fMtrlDiffuseColor);
    ImGui::ColorEdit3("MtrlAmbient", m_fMtrlAmbientColor);
    ImGui::ColorEdit3("MtrlSpecular", m_fMtrlSpecularColor);

    ImGui::NewLine();
    ImGui::Text("Direction");
    ImGui::ColorEdit3("DirectionDiffuse", m_fDirectionDiffuseColor);
    ImGui::ColorEdit3("DirectionAmbient", m_fDirectionAmbientColor);
    ImGui::ColorEdit3("DirectionSpecular", m_fDirectionSpecularColor);

    ImGui::End();
}

CToolMgr* CToolMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CToolMgr* pInstance = new CToolMgr(pGraphicDev);

    if (FAILED(pInstance->Setup_Imgui()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }

	return pInstance;
}

void CToolMgr::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    //TODO::delete 해주기
    for (int i = 0; i < m_pTileTexture.size(); ++i)
        Engine::Safe_Release(m_pTileTexture[i]);
}
