#pragma once
#include "Base.h"
//#include "framework.h"
#include "Engine_Define.h"
#include "ToolTerrain.h"

BEGIN(Engine)
class CManagement;
class CGraphicDev;
class CTexture;
END

class CMainTool :
    public CBase
{
private:
    explicit CMainTool();
    virtual ~CMainTool();

public:
    HRESULT Ready_MainTool();
    void Update_MainTool();
    void LateUpdate_MainTool();
    void Render_MainTool();

public:
    void Setup_ImGui();

private:
    void Window_Tile();

private:
    HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement);
    HRESULT		SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev);

private:
    Engine::CGraphicDev* m_pDeviceClass;
    Engine::CManagement* m_pManagementClass;
    LPDIRECT3DDEVICE9			m_pGraphicDev;

    vector<Engine::CTexture*> m_pTileTexture;
    //CToolTerrain* m_pTerrain;

private:
    char* ConvertWCtoC(_tchar* str);

    HRESULT Ready_Layer_GameLogic(const _tchar* pLayerTag);

public:
    static CMainTool* Create();

private:
    virtual void Free();

};

