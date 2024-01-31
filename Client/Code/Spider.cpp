#include "..\Include\stdafx.h"
#include "Spider.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "Scene.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos)
{
}

CSpider::CSpider(const CSpider& rhs)
    :CMonster(rhs)
{
}

CSpider::~CSpider()
{
}

HRESULT CSpider::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    Set_ObjStat();
    /*m_pTransForm->m_vScale = { 1.f, 1.f, 1.f };*/
    m_fFrameEnd = 6;

    return S_OK;
}

_int CSpider::Update_GameObject(const _float& fTimeDelta)
{
    m_fFrame += m_fFrameEnd * fTimeDelta;

    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;
    Player_Chase(fTimeDelta);
    
    CGameObject::Update_GameObject(fTimeDelta);

    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CSpider::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    //_vec3	vPos;
    //m_pTransForm->Get_Info(INFO_POS, &vPos);

    //__super::Compute_ViewZ(&vPos);
   
    /*Height_OnTerrain();*/
}

void CSpider::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    /* Set_Scale();*/

    m_pTextureCom[m_ePreLook]->Set_Texture((_uint)m_fFrame);

    if (m_Dirchange)
    {
        m_pReverseCom->Render_Buffer();
    }
    else if (!m_Dirchange)
    {
        m_pBufferCom->Render_Buffer();
    }

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSpider::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

    //Texturecom
    pComponent = m_pTextureCom[LOOK_DOWN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_side", pComponent });

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 0.7f, 0.5f, 0.7f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
    return S_OK;
}

void CSpider::Height_OnTerrain()
{
    _vec3		vPos;
    auto pTerrain = scenemgr::Get_CurScene()->GetTerrainObject();

    m_pTransForm->Get_Info(INFO_POS, &vPos);

    Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(pTerrain->Find_Component(ID_STATIC, L"Proto_TerrainTex"));
    NULL_CHECK(pTerrainBufferCom);

    _float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

    m_pTransForm->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

void CSpider::Player_Chase(const _float& fTimeDelta)
{
    _vec3 PlayerPos;
    PlayerPos = Get_Player_Pos();

    m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);

    
    Look_Change();


}

void CSpider::Set_ObjStat()
{
    m_Stat.fHP = 100.f;
    m_Stat.fMxHP = 100.f;
    m_Stat.fSpeed = 1.f;
}

CSpider* CSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CSpider* pInstance = new CSpider(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Beefalo Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CSpider::Free()
{
	CMonster::Free();
}
