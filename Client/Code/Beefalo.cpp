#include "stdafx.h"
#include "Beefalo.h"
//#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
CBeefalo::CBeefalo(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos)
    , m_eCurState(WALK)
    , m_ePreState(STATE_END)
    , m_bHit(false)
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
    m_fFrameChange = rand() % 5;
    return S_OK;
}

_int CBeefalo::Update_GameObject(const _float& fTimeDelta)
{

    if (!m_bFrameStop)
        m_fFrame += m_fFrameEnd * fTimeDelta;
    else
    {
        m_fFrame = m_fFrameEnd;
    }
    _int iResult = Die_Check();

    if (!m_Stat.bDead)      //죽은 상태가 아닐때 진입
    {
        if (m_Attacked)     //공격받았을때 진입
        {
            Attacking(fTimeDelta);
        }
        else                 //공격 받지 않은 상태
        {
            Patroll(fTimeDelta);
        }
    }           //여기에 else 걸어서 사망 트리거 연결(아이템 드랍 테이블)
    CGameObject::Update_GameObject(fTimeDelta);
    State_Change();
    Look_Change();

    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    
    return iResult;
}

void CBeefalo::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
}

void CBeefalo::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    /* Set_Scale();*/

    m_pTextureCom[m_ePreLook][m_ePreState]->Set_Texture((_uint)m_fFrame);

    FAILED_CHECK_RETURN(SetUp_Material(), );
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
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CBeefalo::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

#pragma region TEXCOM
    //Graze
    pComponent = m_pTextureCom[LOOK_DOWN][GRAZE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_graze"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_graze", pComponent });

    //IDLE
    pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_idle", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_idle", pComponent });

    //Walk
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_walk_side", pComponent });

    //RUN
    pComponent = m_pTextureCom[LOOK_DOWN][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][MADRUN] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_run_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_run_side", pComponent });

    //ATTACK
    pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_atk_side", pComponent });

    //DEAD
    pComponent = m_pTextureCom[LOOK_LEFT][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_dead", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_dead", pComponent });

    //HIT
    pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_hit", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Beefalo_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Beefalo_hit", pComponent });


    //ERASE
    pComponent = m_pTextureCom[LOOK_DOWN][ERASE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Erase"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Erase", pComponent });

#pragma endregion TEXCOM

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->m_vScale = { 2.f, 2.f, 2.f };

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
    return S_OK;
}


void CBeefalo::Set_ObjStat()
{
    m_Stat.fHP = 100.f;
    m_Stat.fMxHP = 100.f;
    m_Stat.fSpeed = 1.f;
    m_Stat.fATK = 20.f;
    m_Stat.bDead = false;
    m_Stat.fATKRange = 1.3f;
}

void CBeefalo::State_Change()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case IDLE:
            m_fFrameEnd = 16;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_DOWN;
            }
            break;
        case GRAZE:
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 10;
            break;
        case WALK:
            m_fFrameEnd = 20;
            break;
        case ATTACK:
            m_fFrameEnd = 7;
            break;
        case MADRUN:
            m_fFrameEnd = 4;
            break;
        case HIT:
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_RIGHT;
            }
            m_fFrameEnd = 5;
            break;
        case DEAD:
            m_fFrameEnd = 12;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_RIGHT;
            }
            break;
        case ERASE:
            m_fFrameEnd = 5;
            m_eCurLook = LOOK_DOWN;
            break;
        }
        m_fFrame = 0.f;
        m_ePreState = m_eCurState;
    }
}

_int CBeefalo::Die_Check()
{
    if (m_Stat.fHP <= 0 && m_ePreState != DEAD && m_ePreState != ERASE)
    {
        m_eCurState = DEAD;
        m_Stat.bDead = true;
    }
    else if (m_ePreState == DEAD)
    {
        if (m_fFrameEnd <= m_fFrame)
        {
            m_eCurState = ERASE;
        }
    }
    else if (m_ePreState == ERASE)
    {
        if ((m_fFrameEnd - 1) <= m_fFrame)
        {
            m_bFrameStop = true;
            DeleteObject(this);
            return 0x80000000;
        }
    }

    return 0;
}

void CBeefalo::Attacking(const _float& fTimeDelta)
{
    m_Stat.fSpeed = 5.f;
    if (!m_bHit)
    {
        if (IsTarget_Approach(1.f) && m_ePreState != ATTACK)
        {
            m_eCurState = ATTACK;
        }
        else if (m_ePreState == ATTACK)
        {
            if ((m_fFrameEnd < m_fFrame) && !IsTarget_Approach(1.f))
            { 
                m_eCurState = MADRUN;
            }
            else if ((3 < m_fFrame) && IsTarget_Approach(m_Stat.fATKRange) && !m_bAttacking)
            {
                dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(m_Stat.fATK);
                m_bAttacking = true;
            }
        }
        else if (m_ePreState == MADRUN && !IsTarget_Approach(1.f))
        {
            Player_Chase(fTimeDelta);
        }
        else if (!IsTarget_Approach(1.f))
        {
            m_eCurState = MADRUN;
        }
    }
    else
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_bHit = false;
        }

    }

    if (!IsTarget_Approach(7.f))
    {
        m_Attacked = false;
    }


    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
        if(m_bAttacking)
            m_bAttacking = false;
    }
}

void CBeefalo::Patroll(const _float& fTimeDelta)
{
    m_fAcctime += fTimeDelta;
    m_Stat.fSpeed = 1.f;
    if (m_fFrameChange < m_fAcctime)        //엑셀타임 충족 트리거
    {
        m_fAcctime = 0.f;
        m_fFrameChange = rand() % 6;
        int RandomPattern = rand() % 3;         // 0 1 2 인덱스 중 랜덤 패턴 적용
        m_eCurState = (BEEFALOSTATE)RandomPattern;

        if (m_eCurState == WALK)            //걷기가 당첨되면 방향 변환
        {
            int randomValue = rand() % 360;
            int randomValue2 = rand() % 360;
            // 부호를 무작위로 선택 (-1 또는 1)
            int sign = (rand() % 2 == 0) ? 1 : -1;
            int sign2 = (rand() % 2 == 0) ? 1 : -1;

            // 랜덤값에 부호를 적용
            int result = randomValue * sign;
            int result2 = randomValue2 * sign2;
            m_vDir = { (float)result,0.f,(float)result2 };
            D3DXVec3Normalize(&m_vDir, &m_vDir);
        }
    }
    else if (m_ePreState == WALK)           //걷기일때 방향으로 이동.
    {
        /*const _vec3* _vDir, const _float& fSpeed, const _float& fTimeDelta*/
        m_eCurLook = m_pTransForm->Patroll_LookChange(&m_vDir, m_Stat.fSpeed, fTimeDelta);
    }

    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;

}

void CBeefalo::Set_Hit()
{
    m_eCurState = HIT;
    m_bHit = true;
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
    __super::Free();
}