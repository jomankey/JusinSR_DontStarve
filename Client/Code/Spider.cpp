#include "..\Include\stdafx.h"
#include "Spider.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "Scene.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos)
    :CMonster(pGraphicDev, _vPos), m_bModeChange(false), m_eCurstate(WALK), m_ePrestate(STATE_END)
{ 
}

CSpider::CSpider(const CSpider& rhs)
    :CMonster(rhs), m_bModeChange(rhs.m_bModeChange)
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
    m_fFrameChange = rand() % 3;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    Look_Change();
    return S_OK;
}

_int CSpider::Update_GameObject(const _float& fTimeDelta)
{
    m_fFrame += m_fFrameEnd * fTimeDelta;
    Die_Check();        //죽었는지 검사
    if (!m_Stat.bDead)      //죽지 않았을시 진입
    {
        if (IsTarget_Approach(5.f) == true)     //플레이어와의 거리가 5보다 작으면 진입
        {
            Attacking(fTimeDelta);
        }
        else
        {
            Patroll(fTimeDelta);
        }
    }
    else
    {
        //아이템 드랍 메소드 추가
    }
    State_Change();
    Set_Scale();
    CGameObject::Update_GameObject(fTimeDelta);

    renderer::Add_RenderGroup(RENDER_ALPHA, this);
    return 0;
}

void CSpider::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
    m_pTransForm->BillBoard();
}

void CSpider::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    

    m_pTextureCom[m_ePreLook][m_ePrestate]->Set_Texture((_uint)m_fFrame);

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
#pragma region TEXCOM

    //WALK
    pComponent = m_pTextureCom[LOOK_DOWN][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][WALK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_walk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_walk_side", pComponent });

    //ATTACK
    pComponent = m_pTextureCom[LOOK_DOWN][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_down"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_down", pComponent });

    pComponent = m_pTextureCom[LOOK_UP][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_up"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_up", pComponent });

    pComponent = m_pTextureCom[LOOK_LEFT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_side", pComponent });

    pComponent = m_pTextureCom[LOOK_RIGHT][ATTACK] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_atk_side"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_atk_side", pComponent });

    //DEAD
    pComponent = m_pTextureCom[LOOK_DOWN][DEAD] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Spider_dead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Spider_dead", pComponent });

#pragma endregion TEXCOM
    
    

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

//void CSpider::Player_Chase(const _float& fTimeDelta)
//{
//    _vec3 PlayerPos;
//    PlayerPos = Get_Player_Pos();
//    m_eCurLook = m_pTransForm->Chase_Target_Monster(&PlayerPos, m_Stat.fSpeed, fTimeDelta);
//}

void CSpider::State_Change()
{
    enum SPIDERSTATE { IDLE, WALK, ATTACK, SLEEP, DEAD, STATE_END };
    if (m_ePrestate != m_eCurstate)
    {
        switch (m_eCurstate)
        {
        case IDLE:
            break;
        case WALK:
            m_fFrameEnd = 6.f;
            break;
        case ATTACK:
            m_fFrameEnd = 11.f;
            break;
        case SLEEP:
            break;
        case DEAD:
            m_fFrameEnd = 9.f;
            break;
        }
        m_ePrestate = m_eCurstate;
        m_fFrame = 0.f;
    }
}

void CSpider::Set_ObjStat()
{
    m_Stat.fHP = 100.f;
    m_Stat.fMxHP = 100.f;
    m_Stat.fSpeed = 1.5f;

    m_Stat.fATK = 10.f;
    m_Stat.bDead = false;
}

void CSpider::Set_Scale()
{
    if(m_ePrestate == ATTACK)
        m_pTransForm->m_vScale = { 1.5f, 1.f, 1.5f };
    else if(m_ePrestate == WALK)
        m_pTransForm->m_vScale = { 1.f, 1.f, 1.f };
}

void CSpider::Attacking(const _float& fTimeDelta)
{
    if (!m_bModeChange)
    {
        m_bModeChange = true;
    }
    m_fAcctime += fTimeDelta;
    m_Stat.fSpeed = 5.5f;

    if (IsTarget_Approach(1) && m_eCurstate != ATTACK)
    {
        m_eCurstate = ATTACK;
       
    }
    else if (m_eCurstate == ATTACK)
    {
        if (m_fFrameEnd < m_fFrame)
        {
            if (!IsTarget_Approach(1))
            {
                m_eCurstate = WALK;
            }
        }
    }
    else if (m_eCurstate == WALK)
    {
        Player_Chase(fTimeDelta);
    }
 
    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;
    Look_Change();
}

void CSpider::Patroll(const _float& fTimeDelta)
{

    if (m_bModeChange)
    {
        m_bModeChange = false;
    }
    m_fAcctime +=  fTimeDelta;
    m_eCurstate = WALK;
    m_Stat.fSpeed = 1.5f;

 
    if (m_fFrameChange < m_fAcctime)
    {
        m_fAcctime = 0.f;
        m_fFrameChange = rand() % 3;
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
    else
    {
        m_pTransForm->Move_Pos(&m_vDir, m_Stat.fSpeed, fTimeDelta);
    }
    if (m_fFrameEnd < m_fFrame)
        m_fFrame = 0.f;


    Look_Change();
}

void CSpider::Die_Check()
{
    if (m_Stat.fHP <= 0 && m_ePrestate != DEAD)
    {
        m_eCurstate = DEAD;
        m_eCurLook = LOOK_DOWN;
        m_Stat.bDead = true;
        Look_Change();
        m_fFrame = 0.f;
    }
    else if (m_ePrestate == DEAD)
    {
        if (m_fFrameEnd < m_fFrame)
            m_fFrame = m_fFrameEnd;
    }
    else
        return;
        
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
