#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	HRESULT			Ready_Transform();
	virtual _int	Update_Component(const _float& fTimeDelta);

public:
	_matrix*		Get_WorldMatrix() { return &m_matWorld; }
	void				Set_Pos(const _vec3& vPos)
	{
		m_vInfo[INFO_POS] = vPos;
	}
	void				Set_Pos(_float fX, _float fY, _float fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}

	void		Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}

	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
	{
		m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
	}

	void		Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}

public:
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;
	_matrix		m_matWorld;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone();

private:
	virtual void		Free();
};

END

