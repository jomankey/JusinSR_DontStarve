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
	void Set_Attack(_float _fAttack);

public:
	OBJSTAT Get_Monster_Stat() { return m_Stat; }
public:
	virtual void Free() override;
protected:
	virtual void		Set_ObjStat() PURE;
	virtual void		State_Change() PURE;
	void				Player_Chase(const _float& fTimeDelta);
	_bool		IsTarget_Approach(float _fDistance);		//플레이어상대로 가까운지 검사하는 함수
	void		Look_Change();
	_vec3		Get_Player_Pos();
protected:
	_vec3		m_vPos;	//위치 저장용
	_vec3		m_vDir; //현재 이동중인 방향
	LOOKDIR		m_eCurLook;
	LOOKDIR		m_ePreLook;
	OBJSTAT		m_Stat;		//스탯
	_bool		m_Dirchange; //false 일때 오른쪽 보기
	_bool		m_Attacked;	//공격당했는지 아닌지 검사하는 변수
	

	//프레임
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	_float				m_fAcctime;		//시간 누적용 변수
	_int				m_fFrameChange = 0;		//프레임이 바뀌어야 하는 경우에 사용함.
	_bool				m_bFrameStop;		//프레임을 멈춰야 할 때 true로 바꿔줘
	
};

