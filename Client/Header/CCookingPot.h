#pragma once
#include "ResObject.h"
class CCookingPot :
    public CResObject
{



	enum COOKINGPOT_STATE {
		COOKINGPOT_IDLE_EMPTY,
		COOKINGPOT_IDLE_FULL,
		COOKINGPOT_COOKING_LOOP,
		COOKINGPOT_BURNT,
		COOKINGPOT_PLACE,
		COOKINGPOT_HIT_EMPTY,
		COOKINGPOT_HIT_COOKING,
		COOKINGPOT_END
	};


private:
	explicit CCookingPot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCookingPot(const CCookingPot& rhs);
	virtual ~CCookingPot();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;



private:
	virtual HRESULT			Add_Component() override;
	virtual void			Check_FrameState() override;
	virtual void			Ready_Stat() override {};
	virtual void			Change_Frame_Event() override {};





public:
	static CCookingPot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


private:
	CTexture* m_pCookingpotTexCom[COOKINGPOT_END];

	COOKINGPOT_STATE m_eCookingpotCurState;
	COOKINGPOT_STATE m_eCookingpotPrevState;

	BOOL m_bIsDrop = false;



};

