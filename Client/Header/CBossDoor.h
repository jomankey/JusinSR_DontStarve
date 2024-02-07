#pragma once
#include "ResObject.h"
class CBossDoor :
    public CResObject
{
	enum BOSSDOOR_STATE { BOSSDOOR_OPEN, BOSSDOOR_CLOSE,BOSSDOOR_IDLE, BOSSDOOR_END };

private:
	explicit CBossDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossDoor(const CBossDoor& rhs);
	virtual ~CBossDoor();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

private:
	virtual HRESULT			Add_Component() override;
	virtual void Check_FrameState() override;
	virtual void Ready_Stat() override {};
	//virtual void Change_Frame_Event() override;





public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();



private:
	Engine::CTexture* m_pBossDoorTextureCom[BOSSDOOR_END];

	BOSSDOOR_STATE m_eBossDoorCurState;



};

