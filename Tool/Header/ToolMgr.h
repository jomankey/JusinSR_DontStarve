#pragma once
#include "Base.h"

#include "Engine_Define.h"
#include "Export_System.h"

BEGIN(Engine)
class CTexture;

END
class CToolMgr :
    public CBase
{

private:
    explicit CToolMgr(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolMgr();

public:
    HRESULT Setup_Imgui();
    void Update_ToolMgr();
    void Render_ToolMgr();
    
private:
    //¿Ø¥÷√¢
    void Window_Tile();
    void Window_Light();

public:
    static float m_fMtrlDiffuseColor[3];
    static float m_fMtrlAmbientColor[3];
    static float m_fMtrlSpecularColor[3];

    static float m_fDirectionDiffuseColor[3];
    static float m_fDirectionAmbientColor[3];
    static float m_fDirectionSpecularColor[3];

    LPDIRECT3DDEVICE9			m_pGraphicDev;

    vector<Engine::CTexture*> m_pTileTexture;
    int item_current_idx;

public:
    static CToolMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

};

