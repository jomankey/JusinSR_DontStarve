#include "..\Include\stdafx.h"

#include "ExplainPanel.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "UIMgr.h"

CExplainPanel::CExplainPanel(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, wstring strItemKey)
    : CGameObject(pGraphicDev), 
    m_fX(vPos.x), 
    m_fY(vPos.y), 
    m_strItemKey(strItemKey),
    m_pBufferCom(nullptr),
    m_pTextureCom(nullptr),
    m_fSizeX(0.f),
    m_fSizeY(0.f)
{
    ZeroMemory(&m_tCreateInfo, sizeof(m_tCreateInfo));
}

CExplainPanel::CExplainPanel(const CExplainPanel& rhs)
    : CGameObject(rhs), 
    m_fX(rhs.m_fX), 
    m_fY(rhs.m_fY), 
    m_strItemKey(rhs.m_strItemKey),
    m_pBufferCom(rhs.m_pBufferCom),
    m_pTextureCom(rhs.m_pTextureCom),
    m_fSizeX(rhs.m_fSizeX),
    m_fSizeY(rhs.m_fSizeY)
{
}

CExplainPanel::~CExplainPanel()
{
}

HRESULT CExplainPanel::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_tCreateInfo = CUIMgr::GetInstance()->Get_CreateInfo(m_strItemKey);

    m_fSizeX = 80.f;
    m_fSizeY = 80.f;

    m_pTransForm->Set_Pos(_vec3((m_fX - WINCX * 0.5f), -m_fY + WINCY * 0.5f, 0.f));
    m_pTransForm->Set_Scale(_vec3(m_fSizeX, m_fSizeY, 0.f));

    D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

    return S_OK;
}

_int CExplainPanel::Update_GameObject(const _float& fTimeDelta)
{
    __super::Update_GameObject(fTimeDelta);

    return 0;
}

void CExplainPanel::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
}

void CExplainPanel::Render_GameObject()
{
    if (!m_bShow)
		return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    Engine::Render_Font(L"Font_Count", m_tCreateInfo.strName, &_vec2(m_fX - 10, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    Engine::Render_Font(L"Font_Count", m_tCreateInfo.strInfo, &_vec2(m_fX - 10, m_fY), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CExplainPanel* CExplainPanel::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, wstring strItemKey)
{
    CExplainPanel* pInstance = new CExplainPanel(pGraphicDev, vPos, strItemKey);
    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pInstance;
}

HRESULT CExplainPanel::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Explain_PopUp_Panel"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Explain_PopUp_Panel", pComponent });

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

void CExplainPanel::Free()
{
    __super::Free();
}
