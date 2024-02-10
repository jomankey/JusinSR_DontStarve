
#pragma once
#include "ResObject.h"



class CFire :
	public CResObject
{
	enum FIRE_STATE { FIRE_LEVEL_1=1, FIRE_LEVEL_2=2, FIRE_LEVEL_3=3, FIRE_LEVEL_4=4, FIRE_END };

private:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFire(const CFire& rhs);
	virtual ~CFire();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;



	void Set_Pos(_vec3 _Pos);
	void Set_FireLevel(_int iLevel);

	BOOL Get_IsOff() { return m_bIsOff; }
	void Set_IsOff(BOOL _bIsOff) { m_bIsOff = _bIsOff; }
private:
	virtual HRESULT			Add_Component() override;
	virtual void			Check_FrameState() override;
	virtual void			Ready_Stat() override {};
	virtual void			Change_Frame_Event() override {};




	void Level_Down();
public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


private:
	CTexture* m_pFireTexCom[FIRE_END];

	FIRE_STATE m_efireCurState;
	FIRE_STATE m_efirerPrevState;


	_float m_fTimeChek = 0.f;
	_float m_MaxfTimeChek = 10.f;

	BOOL m_bIsOff = false;
};

