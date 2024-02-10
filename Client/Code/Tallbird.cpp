#include "stdafx.h"
#include "Tallbird.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "Scene.h"
#include <ItemBasic.h>

CTallbird::CTallbird(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos), m_eCurstate(SLEEP), m_ePrestate(STATE_END)
{ 
}

CTallbird::CTallbird(const CTallbird& rhs)
    :CMonster(rhs)
{
}

CTallbird::~CTallbird()
{
}

HRESULT CTallbird::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransForm->Set_Pos(m_vPos);
    Set_ObjStat();
    /*m_pTransForm->m_vScale = { 1.f, 1.f, 1.f };*/
    m_bStatChange[0] = false;
    m_bStatChange[1] = false;
    m_fFrameEnd = 0;
    m_bFrameStop = true;
    m_fFrameChange = rand() % 3;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    Look_Change();
    return S_OK;
}

_int CTallbird::Update_GameObject(const _float& fTimeDelta)
{
  
    if (!m_bFrameStop)
    {
        m_fFrame += m_fFrameEnd * fTimeDelta;
    }

    if (IsTarget_Approach(m_Stat.fAggroRange) && !m_bStatChange[0])  //1회 최초진입
    {
        m_bStatChange[0] = true;
        m_bFrameStop = false;
    }

    _int iResult = Die_Check();    //죽었는지 검사
    if (!m_Stat.bDead)      //죽지 않았을시 진입
    {
        if (m_bStatChange[0])
        {           
            if (m_bStatChange[1])
            {
                Second_Phase(fTimeDelta);
            }
            else
            {
                First_Phase(fTimeDelta);
            }
        }
    }
   
    
    State_Change();
    Look_Change();
    Set_Scale();
    CGameObject::Update_GameObject(fTimeDelta);

    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return iResult;
}

void CTallbird::LateUpdate_GameObject()
{
    CGameObject::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
    _vec3	vPos;
    m_pTransForm->Get_Info(INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);

}

void CTallbird::Render_GameObject()
{
    //Test
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pTextureCom[m_ePreLook][m_ePrestate]->Set_Texture((_uint)m_fFrame);

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

HRESULT CTallbird::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pReverseCom = dynamic_cast<CRvRcTex*>(proto::Clone_Proto(L"Proto_RvRcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RvRcTex", pComponent });

    //Texturecom
#pragma region TEXCOM
    //IDLE
    pComponent = m_pTextureCom[LOOK_DOWN][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_idle", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_idle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_idle", pComponent });




    //WALK
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkdown"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkdown", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkup"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkup", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkiside"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkiside", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_walkiside"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_walkiside", pComponent });

    //ATTACK
    pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_atk_side", pComponent });


    
    //HIT
    pComponent = m_pTextureCom[LOOK_DOWN][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_hit", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_hit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_hit", pComponent });

    //Sleep
    pComponent = m_pTextureCom[LOOK_DOWN][SLEEP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_sleep"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_sleep", pComponent });

    //Wakeup
    pComponent = m_pTextureCom[LOOK_DOWN][WAKE_UP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_wakeup"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_wakeup", pComponent });

    //taunt
    pComponent = m_pTextureCom[LOOK_DOWN][TAUNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_taunt"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_taunt", pComponent });

    //DEAD
    pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_tallbird_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_tallbird_dead", pComponent });

    //ERASE
    pComponent = m_pTextureCom[LOOK_DOWN][ERASE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Erase"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Erase", pComponent });
#pragma endregion TEXCOM
    
    

    pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransForm->Set_Scale({ 2.f, 5.f, 2.f });

    pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
    return S_OK;
}



void CTallbird::State_Change()
{
    if (m_ePrestate != m_eCurstate)
    {
        switch (m_eCurstate)
        {
        case IDLE:
            m_fFrameEnd = 14.f;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_DOWN;
            }
            break;
        case WALK:
            m_fFrameEnd = 8.f;
            break;
        case ATTACK:
            m_fFrameEnd = 12.f;
            break;
        case SLEEP:
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 0;
            break;
        case WAKE_UP:
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 16;
            break;
        case TAUNT:
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 22.f;
            break;
        case HIT:
            m_fFrameEnd = 6;
            if (m_eCurLook != LOOK_LEFT)
            {
                m_eCurLook = LOOK_DOWN;
            }
            break;
        case DEAD:
            m_eCurLook = LOOK_DOWN;
            m_fFrameEnd = 10.f;
            break;
        case ERASE:
            m_fFrameEnd = 5;
            m_eCurLook = LOOK_DOWN;
            break;
        }
        m_ePrestate = m_eCurstate;
        m_fFrame = 0.f;
    }
}

void CTallbird::Set_ObjStat()
{
    m_Stat.strObjName = L"키다리새";
    m_Stat.fHP = 150.f;
    m_Stat.fMxHP = 150.f;
    m_Stat.fSpeed = 2.5f;

    m_Stat.fATK = 25.f;
    m_Stat.bDead = false;
    m_Stat.fATKRange = 2.f;
    m_Stat.fAggroRange = 3.f;
}

void CTallbird::Set_Scale()
{
  
}

void CTallbird::First_Phase(const _float& fTimeDelta)
{
    if (m_ePrestate == SLEEP)
    {
        m_eCurstate = WAKE_UP;
    }
    else if (m_ePrestate == WAKE_UP)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurstate = TAUNT;
        }
    }
    else if (m_ePrestate == TAUNT)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurstate = IDLE;
        }
    }
    else if (m_ePrestate == IDLE)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            m_eCurstate = WALK;
            m_bStatChange[1] = true;
        }
    }

    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
    }

}

void CTallbird::Second_Phase(const _float& fTimeDelta)
{

    if (!m_bHit)
    {
        if (IsTarget_Approach(m_Stat.fATKRange + 0.5f))
        {
            m_eCurstate = ATTACK;
        }
        else
        {
            m_eCurstate = WALK;
        }



        if (m_ePrestate == ATTACK)
        {
            if (m_fFrame > 5.f && !m_bAttacking && IsTarget_Approach(m_Stat.fATKRange))
            {
                dynamic_cast<CPlayer*>(Get_Player_Pointer())->Set_Attack(m_Stat.fATK);
                m_bAttacking = true;
            }
            else if (m_fFrameEnd < m_fFrame && !IsTarget_Approach(m_Stat.fATKRange + 0.5f))
            {
                m_eCurstate = WALK;
            }
        }
        else if (m_ePrestate == WALK)
        {
            Player_Chase(fTimeDelta);
            Collision_EachOther(fTimeDelta);
        }

    }


    if (m_fFrameEnd < m_fFrame)
    {
        m_fFrame = 0.f;
        if(m_bAttacking)
            m_bAttacking = false;
        if (m_bHit)
            m_bHit = false;
    }
    
}


_int CTallbird::Die_Check()
{
    if (m_Stat.fHP <= 0 && m_ePrestate != DEAD && m_ePrestate != ERASE )
    {
        m_eCurstate = DEAD;
        m_eCurLook = LOOK_DOWN;
        m_Stat.bDead = true;
        m_fFrame = 0.f;
    }
    else if (m_ePrestate == DEAD)
    {
        if (m_fFrameEnd < m_fFrame)
        {

            srand(static_cast<unsigned int>(time(nullptr)));
            int iItemCount = rand() % 1 + 3;	//아이템 갯수용
            for (int i = 0; i < iItemCount; ++i)
            {
                int signX = (rand() % 2 == 0) ? -1 : 1;
                int signZ = (rand() % 2 == 0) ? -1 : 1;
                int iItemPosX = rand() % 3 * signX;
                int iItemPosZ = rand() % 3 * signZ;
                _vec3 vPos;
                m_pTransForm->Get_Info(INFO_POS, &vPos);
                vPos.x += iItemPosX;
                vPos.y = 0.8f;
                vPos.z += iItemPosZ;
                CGameObject* pGameObj = CItemBasic::Create(m_pGraphicDev, L"Meat_Monster");
                dynamic_cast<CItemBasic*>(pGameObj)->SetCreateByObject();
                pGameObj->GetTransForm()->Set_Pos(vPos);
                scenemgr::Get_CurScene()->AddGameObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::ITEM, pGameObj);

            }



            m_eCurstate = ERASE;
        }
    }
    else if (m_ePrestate == ERASE)
    {
        if (m_fFrameEnd-1 < m_fFrame)
        {
            m_bFrameStop = true;
            DeleteObject(this);
            return 0x80000000;
        }
    }
    
    return 0;
        
}

void CTallbird::Set_Hit()
{
    m_eCurstate = HIT;
    m_bHit = true;
}



CTallbird* CTallbird::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
{
    CTallbird* pInstance = new CTallbird(pGraphicDev, _vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Tallbird Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CTallbird::Free()
{
    __super::Free();
}
