#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "CInvenBox.h"
#include <SlideBox.h>

class CInvenBoxMgr :
    public CBase
{
    DECLARE_SINGLETON(CInvenBoxMgr)

private:
    explicit CInvenBoxMgr();
    virtual ~CInvenBoxMgr();
    
public:
    void Set_Create_Menu(int _iNum, CItem* pItem);

    void Get_BoxPos(BOX_TYPE eType, int _iNum, _vec3* vPos) {
        vector< CInvenBox*> pBox = m_mapBox.find(eType)->second;
        vPos->x = pBox[_iNum]->Get_fX();
        vPos->y = pBox[_iNum]->Get_fY();
    }

    vector<CInvenBox*> Get_BoxList(BOX_TYPE eType) { return m_mapBox.find(eType)->second; }

public:
    HRESULT Add_InvenBoxList(LPDIRECT3DDEVICE9 pGraphicDev, BOX_TYPE eType, BOX_DIR eDir ,int _iNum);
    void Update_InvenBoxMgr(const _float& fTimeDelta, BOX_TYPE eType);
    void LateUpdate_InvenBoxMgr(BOX_TYPE eType);
    void Render_InvenBoxMgr(BOX_TYPE eType);
    
private:
    virtual void Free();

private:
    map <BOX_TYPE, vector<CInvenBox*>> m_mapBox;

};

