#pragma once
#include "ResObject.h"
class CTent :
    public CResObject
{



	enum CTENT_STATE { 
		CTENT_IDLE, 
		CTENT_HIT,
		CTENT_ENTER,
		CTENT_DESTROY,
		CTENT_BURNT,
		CTENT_PLACE,
		CTENT_END
	};

private:
	explicit CTent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTent(const CTent& rhs);
	virtual ~CTent();

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
	static CTent* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


private:
	CTexture* m_pTentTexCom[CTENT_END];

	CTENT_STATE m_eTentCurState;
	CTENT_STATE m_eTentPrevState;

	BOOL m_bIsDrop = false;


};

