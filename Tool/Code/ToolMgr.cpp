#include "ToolMgr.h"
#include "framework.h"

#include "Export_Utility.h"

_vec3 CToolMgr::m_fDirectionDiffuseColor[3] = { {1.f, 1.f, 1.f} };
_vec3 CToolMgr::m_fDirectionAmbientColor[3] = { {1.f, 1.f, 1.f} };
_vec3 CToolMgr::m_fDirectionSpecularColor[3] = { {1.f, 1.f, 1.f} };

int CToolMgr::iItemCurrentMonsterIdx = 0;
int CToolMgr::iItemCurrentEtcIdx = 0;
int CToolMgr::iItemCurrentItemIdx = 0;

_bool CToolMgr::bObjectAdd = false;
_bool CToolMgr::bItemAdd = false;
_bool CToolMgr::bMonsterAdd = false;
_bool CToolMgr::bSaveData = false;
_bool CToolMgr::bLoadData = false;
_bool CToolMgr::bTerrainWireFrame = false;

_int CToolMgr::iPickingIndex = -1;
vector<_int> CToolMgr::vecPickingIdex;

_int CToolMgr::iTimeLight = 0; // 밤, 낮 조명 값 저장하기 
_int CToolMgr::iAUtoTime = 1;

CToolMgr::CToolMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), 
    item_current_idx(0)
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

    m_pTileTexture.push_back(dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Tile_Grass_1")));
    m_pTileTexture.push_back(dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Tile_Grass_2")));

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
    Window_Object();
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

    //ImGui::Text("Choose the tile and pick it");

    //const char* Items[] = { "grass", "grass2" };

    //if (ImGui::BeginListBox("Tile List"))
    //{
    //    for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
    //    {
    //        const bool bSelected = (item_current_idx == i);
    //        if (ImGui::Selectable(Items[i], bSelected))
    //            item_current_idx = i;

    //        if (bSelected)
    //            ImGui::SetItemDefaultFocus();
    //    }
    //    ImGui::EndListBox();
    //}

    //for (int i = 0; i < 80; i++)
    //{
    //    ImTextureID my_tex_id = (LPDIRECT3DTEXTURE9*)m_pTileTexture[item_current_idx]->Get_Texture(i);
    //    float my_tex_w = 128.f;
    //    float my_tex_h = 128.f;

    //    ImGui::PushID(i);
    //    ImVec2 size = ImVec2(32.f, 32.f);
    //    ImVec2 uv0 = ImVec2(0.f, 0.f);
    //    ImVec2 uv1 = ImVec2(32.f / my_tex_w, 32.f / my_tex_h);
    //    ImVec4 bg_col = ImVec4(255.f, 255.f, 255.f, 1.f); // 색상값 
    //    ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.f);
    //    ImGui::ImageButton("", (void*)my_tex_id, size, uv0, uv1, bg_col, tint_col);

    //    ImGui::PopID();
    //    if (i % 5 == 0 && i != 0)
    //        ImGui::NewLine();
    //    else
    //        ImGui::SameLine();
    //}

    ImGui::Checkbox("TerrainWireFrame", &bTerrainWireFrame);

    ImGui::NewLine();
    ImGui::Text("Picking VTX : (%d)", iPickingIndex);

    if (ImGui::BeginListBox("Unused List"))
    {
        for (int i = 0; i < vecPickingIdex.size(); ++i)
        {
            const bool bSelected = (item_current_idx == i);

            char buf[32];
            sprintf(buf, "%d", vecPickingIdex[i]);

            if (ImGui::Selectable(buf, bSelected))
                item_current_idx = i;

            if (bSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();

        }
    ImGui::End();
}

//조명 툴
void CToolMgr::Window_Light()
{
    ImGui::Begin("Light");

    ImGui::Text("Auto Time Set");
        ImGui::RadioButton("AutoTime", &iAUtoTime, 0); ImGui::SameLine();
    ImGui::RadioButton("SetLight", &iAUtoTime, 1);

    ImGui::NewLine();

    ImGui::RadioButton("Morning", &iTimeLight, 0); ImGui::SameLine();
    ImGui::RadioButton("Afternoon", &iTimeLight, 1); ImGui::SameLine();
    ImGui::RadioButton("Night", &iTimeLight, 2);

    ImGui::NewLine();
    ImGui::Text("Scene Direction");
    ImGui::ColorEdit3("DirectionDiffuse", m_fDirectionDiffuseColor[iTimeLight]);
    ImGui::ColorEdit3("DirectionAmbient", m_fDirectionAmbientColor[iTimeLight]);
    ImGui::ColorEdit3("DirectionSpecular", m_fDirectionSpecularColor[iTimeLight]);

    ImGui::End();
}

//오브젝트 배치 툴 
void CToolMgr::Window_Object()
{
    //지형지물
    //몬스터
    //플레이어 
    //아이템 
    ImGui::Begin("Object Tool");
    if (ImGui::CollapsingHeader("Monster", ImGuiTreeNodeFlags_None))
    {
        /*ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
        for (int i = 0; i < 5; i++)
            ImGui::Text("Some content %d", i);*/

        if (ImGui::BeginListBox("Monster List"))
        {
            const char* Items[] = { "Spider", "Pig", "Buffalo", "Boss"};

            for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
            {
                const bool bSelected = (CToolMgr::iItemCurrentMonsterIdx == i);
                if (ImGui::Selectable(Items[i], bSelected))
                {
                    CToolMgr::iItemCurrentMonsterIdx = i;
                    CToolMgr::bItemAdd = false;
                    CToolMgr::bObjectAdd = false;
                }
                    
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
    }
    if (ImGui::CollapsingHeader("Etc", ImGuiTreeNodeFlags_None))
    {
        if (ImGui::BeginListBox("Etc List"))
        {
            const char* Items[] = { "Tree", "Rock", "Grass", "Pig_House", "Berry"};
            
            for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
            {
                const bool bSelected = (CToolMgr::iItemCurrentEtcIdx == i);
                if (ImGui::Selectable(Items[i], bSelected))
                {
                    CToolMgr::iItemCurrentEtcIdx = i;
                    CToolMgr::bItemAdd = false;
                    CToolMgr::bMonsterAdd = false;
                }
                    
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        if (ImGui::SmallButton("ObjectAdd")) bObjectAdd = true;
    }

    if (ImGui::CollapsingHeader("Item", ImGuiTreeNodeFlags_None))
    {
        if (ImGui::BeginListBox("Item List"))
        {
            const char* Items[] = { 
                "Stick", 
                "Stone", 
                "Firestone", //부싯돌
                "Grass_Cut",
                };

            for (int i = 0; i < IM_ARRAYSIZE(Items); ++i)
            {
                const bool bSelected = (iItemCurrentItemIdx == i);
                if (ImGui::Selectable(Items[i], bSelected))
                {
                    iItemCurrentItemIdx = i;
                    CToolMgr::bMonsterAdd = false;
                    CToolMgr::bObjectAdd = false;
                }
                    
                if (bSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        if (ImGui::SmallButton("ItemAdd")) 
            bItemAdd = true;
    }

    if (ImGui::SmallButton("Save"))
        bSaveData = true;
    
    ImGui::SameLine();

    if (ImGui::SmallButton("Load"))
        bLoadData = true;

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
