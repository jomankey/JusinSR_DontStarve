#include "../Header/CItemFood.h"
#include "../Include/stdafx.h"

#include "Export_System.h"
#include "Export_Utility.h"

CItemFood::CItemFood(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key, _vec3 _vPos)
	:CItem(pGraphicDev,_key, _vPos)
{

}

CItemFood::~CItemFood()
{

}

_bool CItemFood::UseItem()
{
	return _bool();
}

_bool CItemFood::DropItem()
{
	return _bool();
}
