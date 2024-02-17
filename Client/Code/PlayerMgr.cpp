#include "PlayerMgr.h"
#include "stdafx.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CPlayerMgr)


void CPlayerMgr::Set_Player_Scale(_vec3 _vScale)
{
	m_pPlayer->GetTransForm()->Set_Scale(_vScale);
}

void CPlayerMgr::Initailize()
{
	m_pPlayer = nullptr;
}

CPlayerMgr::CPlayerMgr()
{
}

CPlayerMgr::~CPlayerMgr()
{
	Free();
}

void CPlayerMgr::Free()
{
}
