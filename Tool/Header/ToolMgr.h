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
    //����â
    void Window_Tile();
    void Window_Light();
    void Window_Object();

public:
    static _vec3 m_fDirectionDiffuseColor[3];
    static _vec3 m_fDirectionAmbientColor[3];
    static _vec3 m_fDirectionSpecularColor[3];

    LPDIRECT3DDEVICE9			m_pGraphicDev;

    vector<Engine::CTexture*> m_pTileTexture;
    int item_current_idx;
    static int iItemCurrentMonsterIdx, iItemCurrentEtcIdx, iItemCurrentItemIdx;
    static _bool bObjectAdd, bItemAdd, bMonsterAdd;

    static _bool bSaveData;
    static _bool bLoadData;

    static _bool bTerrainWireFrame;

    static _int iPickingIndex;
    static vector<_int> vecPickingIdex;

    static _int iTimeLight;
    static _int iAUtoTime;
    

public:
    static CToolMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

};

