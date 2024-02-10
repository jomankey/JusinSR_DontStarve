#include "stdafx.h"
#include "..\Header\Effect.h"

#include "Export_System.h"
#include "Export_Utility.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos)
	: Engine::CGameObject(pGraphicDev)
	, m_vPos(vPos)
	, m_fFrameEnd(0.f)
	, m_fAcctime(0.f)
	, m_bFrameStop(false)
	, m_Dirchange(false)
	, m_eCurLook(LOOK_DOWN)
	, m_ePreLook(LOOK_END)
{
}

CEffect::CEffect(const CEffect& rhs)
	: Engine::CGameObject(rhs)
	, m_vPos(rhs.m_vPos)
	, m_fFrameEnd(rhs.m_fFrameEnd)
	, m_fAcctime(rhs.m_fAcctime)
	, m_bFrameStop(rhs.m_bFrameStop)
	, m_Dirchange(rhs.m_Dirchange)
	, m_eCurLook(rhs.m_eCurLook)
	, m_ePreLook(rhs.m_ePreLook)
{

}

CEffect::~CEffect()
{
}

void CEffect::Look_Change()
{
	if (m_ePreLook != m_eCurLook)
	{
		if (m_eCurLook == LOOK_LEFT)
		{
			m_Dirchange = true;
		}
		else
		{
			m_Dirchange = false;
		}

		m_ePreLook = m_eCurLook;
	}
}

_bool CEffect::Collision_Transform(CTransform* _Src, CTransform* _Dst)
{
	_vec3 vSrc, vSrcScale, vDst, vDstScale;

	_Src->Get_Info(INFO_POS, &vSrc);
	vSrcScale = _Src->Get_Scale();

	_Dst->Get_Info(INFO_POS, &vDst);
	vDstScale = _Dst->Get_Scale();

	_float iDistanceX = fabs(vSrc.x - vDst.x);
	_float fRadCX = vSrcScale.x * 0.5f + vDstScale.x * 0.5f;

	_float fDistanceZ = fabs(vSrc.z - vDst.z);
	_float fRadCY = vSrcScale.y * 0.5f + vDstScale.y * 0.5f;

	if (fDistanceZ > fRadCY || iDistanceX > fRadCX)
	{
		return false;
	}
	else
	{
		return true;
	}
}




void CEffect::Free()
{	
	__super::Free();
}




