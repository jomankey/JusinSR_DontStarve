#include "ObjStatus.h"

CObjStatus::CObjStatus()
{
	ZeroMemory(m_Stat, sizeof(m_Stat));
}

CObjStatus::CObjStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
	ZeroMemory(m_Stat, sizeof(m_Stat));
}

CObjStatus::CObjStatus(const CObjStatus& rhs)
	:CComponent(rhs), m_Stat(rhs.m_Stat)
{
}

CObjStatus::~CObjStatus()
{
}

CComponent* CObjStatus::Clone()
{
	return new CObjStatus(*this);
}
