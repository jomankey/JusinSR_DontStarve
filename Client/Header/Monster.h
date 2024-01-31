#pragma once
#include "Base.h"
#include "GameObject.h"
class CMonster :
    public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	void Set_Attack(_float _fAttack) { m_Stat.fHP -= _fAttack; }

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	virtual HRESULT			Add_Component();

public:
	virtual void Free() override;

protected:
	virtual void		Set_ObjStat() PURE;
	void		Look_Change();
	_vec3		Get_Player_Pos();
protected:
	_vec3 m_vPos;
	
	LOOKDIR m_eCurLook;
	LOOKDIR m_ePreLook;
	OBJSTAT m_Stat;
	_bool		m_Dirchange; //false 일때 오른쪽 보기
	_float	m_fAcctime;
};

