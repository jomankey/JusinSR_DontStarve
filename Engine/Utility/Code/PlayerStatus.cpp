#include "PlayerStatus.h"

CPlayerStatus::CPlayerStatus()
{
}

CPlayerStatus::CPlayerStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	:CObjStatus(pGraphicDev)
{
}

CPlayerStatus::CPlayerStatus(const CPlayerStatus& rhs)
	:CObjStatus(rhs)
{
}

CPlayerStatus::~CPlayerStatus()
{
}
