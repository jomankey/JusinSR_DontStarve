#include "..\Include\stdafx.h"
#include "Beefalo.h"
#include "Export_System.h"
#include "Export_Utility.h"
CBeefalo::CBeefalo(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CBeefalo::CBeefalo(const CBeefalo& rhs)
    : Engine::CGameObject(rhs)
{
}

CBeefalo::~CBeefalo()
{
}

HRESULT CBeefalo::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(_float(rand() % 20), 0.f, _float(rand() % 20));

    m_pTransformCom->m_vScale = { 3.f, 2.f, 1.f };
    m_fFrameEnd = 10;
    return S_OK;
}

_int CBeefalo::Update_GameObject(const _float& fTimeDelta)
{
    m_fFrame += m_fFrameEnd * fTimeDelta;

    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;

    CGameObject::Update_GameObject(fTimeDelta);
    
    Engine::Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CBeefalo::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
    _vec3	vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    __super::Compute_ViewZ(&vPos);
    BillBoard();
    Height_OnTerrain();
}

void CBeefalo::Render_GameObject()
{
    
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   /* Set_Scale();*/

    m_pTextureCom->Set_Texture((_uint)m_fFrame);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBeefalo::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Beefalo_graze"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_graze", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
    return S_OK;
}

void CBeefalo::Height_OnTerrain()
{
    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"Terrain", L"Proto_TerrainTex"));
    NULL_CHECK(pTerrainBufferCom);

    _float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

    m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

void CBeefalo::BillBoard()
{
    _matrix	matWorld, matView, matBill;

    m_pTransformCom->Get_WorldMatrix(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, NULL, &matBill);

    m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
}

CBeefalo* CBeefalo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBeefalo* pInstance = new CBeefalo(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBeefalo::Free()
{
    __super::Free();
}
