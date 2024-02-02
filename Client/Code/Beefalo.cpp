#include "stdafx.h"
#include "Beefalo.h"
//#include "Export_System.h"
#include "Export_Utility.h"

CBeefalo::CBeefalo(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos)
    , m_eCurState(WALK)
    , m_ePreState(STATE_END)
{
}

CBeefalo::CBeefalo(const CBeefalo& rhs)
    :CMonster(rhs)
{
}

CBeefalo::~CBeefalo()
{
}

HRESULT CBeefalo::Ready_GameObject()
{
    
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    Set_ObjStat();
    Look_Change();
    m_fFrameEnd = 10;

    return S_OK;
}

_int CBeefalo::Update_GameObject(const _float& fTimeDelta)
{
    m_fFrame += m_fFrameEnd * fTimeDelta;

    if (m_fFrameEnd < m_fFrame)
    {
        if (m_Stat.bDead)
            return 0x8000000;

        m_fFrame = 0.f;
    }

    //die
    if (m_Stat.fHP <= 0 && m_ePreState != DEAD)
    {
        m_eCurState = DEAD;
        m_Stat.bDead = true;

        m_fFrame = 0.f;
    }

    CGameObject::Update_GameObject(fTimeDelta);
    State_Change();
    if (m_eCurState != DEAD)
    {
        Player_Chase(fTimeDelta);
        Look_Change();// DEAD일때 진입 불가능
    }
    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CBeefalo::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

    m_pTransForm->BillBoard();
	
}

void CBeefalo::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	/* Set_Scale();*/

    if (m_eCurState == DEAD && m_ePreLook != LOOK_LEFT)
        m_ePreLook = LOOK_RIGHT;

    m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);


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

HRESULT CBeefalo::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM

    pComponent = m_pTextureCom[LOOK_DOWN][GRAZE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_graze"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_graze", pComponent });
   

    pComponent = m_pTextureCom[LOOK_DOWN][GRAZE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_graze"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_graze", pComponent });

    //Walk
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });
#pragma endregion TEXCOM
	

    pComponent = m_pTextureCom[LOOK_LEFT][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_dead", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_MultiMap[ID_STATIC].insert({ L"Proto_Beefalo_dead", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->m_vScale = { 2.f, 2.f, 2.f };

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
	return S_OK;
}

void CBeefalo::Height_OnTerrain()
{
	_vec3		vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex* pTerrainBufferCom = nullptr;
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

	m_pTransForm->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

void CBeefalo::Set_ObjStat()
{
    m_Stat.fHP = 100.f;
    m_Stat.fMxHP = 100.f;
    m_Stat.fSpeed = 1.f;
    m_Stat.bDead = false;
}

//void CBeefalo::Player_Chase(const _float& fTimeDelta)
//{
//    _vec3 PlayerPos;
//    PlayerPos = Get_Player_Pos();
//
//    m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);
//
// 
//    Look_Change();
//}

void CBeefalo::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case WALK:
            m_fFrameEnd = 20;
            break;
        case DEAD:
            m_fFrameEnd = 12;
            break;
        }

        m_ePreState = m_eCurState;

    }
}

CBeefalo* CBeefalo::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CBeefalo* pInstance = new CBeefalo(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Beefalo Create Failed");
        return nullptr;
    }

	return pInstance;
}

void CBeefalo::Free()
{
    CMonster::Free();
}
