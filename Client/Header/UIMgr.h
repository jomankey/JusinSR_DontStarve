#pragma once

#include "Engine_Define.h"
#include "Base.h"

class CItem;

class CUIMgr :
	public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	CREATEINFO Get_CreateInfo(wstring pKey);

public:
	_bool	AddItem(LPDIRECT3DDEVICE9 pGraphicDev ,wstring strItemKey);	//반환값이False면 아이템습득실패(아이템꽉참)
	CItem*	GetItemBox(_uint _uint);

	void Ready_CreateInfo();

private:
	virtual void Free();

private:
	CItem* m_pItemArr[15]; // 인벤토리

	// 생산법
	vector<CREATEINFO> m_vecCreateInfo;


};

