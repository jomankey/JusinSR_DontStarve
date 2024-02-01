#pragma once

#include "Engine_Define.h"
#include "Base.h"

class CItem;

class CInventoryMgr :
	public CBase
{
	DECLARE_SINGLETON(CInventoryMgr)
public:
	_bool	AddItem(CItem* _pItem);	//반환값이False면 아이템습득실패(아이템꽉참)
	CItem*	GetItemBox(_uint _uint);

private:
	explicit CInventoryMgr();
	virtual ~CInventoryMgr();


private:
	virtual void Free() override;


private:
	CItem* m_pItemArr[15]; // 인벤토리


};

