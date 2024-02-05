#pragma once
#include "CUI.h"

class CSlideUI;

class CItemTool;
class CSlideUI;
class CLightUI;
class CScienceUI;
class CClothUI;
class CWeaponUI;
class CAliveUI;
class CEquiment;


class CToolUI :public CUI
{
	//이 객체는 부모를 통해 생성되고 관리는 데이터메니져를 통해 관리된다.
	//데이터 매니져는 레이어에 있는 CUI를 찾아 Get을 통해 해당 객체의 데이터를 관리한다.
	//이 객체는 각 탭에 대한 데이터를 뿌려주는 역할을 한다.

protected:
	explicit CToolUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _UI_Name);
	explicit CToolUI(const CToolUI& rhs);
	virtual ~CToolUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	//각 자식들은 이 함수를 오버라이딩 해서 사용한다.
	virtual void Pop_SidePanel();

	//게임 데이터를 관리 하는 매니져를 통해 생성된 , CUI에서 
	void SetGameData(CItemTool* _ItemTool, eITEMTOOL_TYPE _ItemType);
	//virtual void UpdateItemTool(eITEMTOOL_TYPE _ItemToolState);

	const CAliveUI* GetAliveUI() { return m_pAliveUI; }
	const CEquiment* GetEquimentI() { return m_pEquimentUI; }
	const CLightUI* GetLightUI() { return m_pLightUI; }
	const CScienceUI* GetScienceUI() { return m_pScienceUI; }
	const CClothUI* GetClothUI() { return m_pClothUI; }
	const CWeaponUI* GetWeaponUI() { return m_pWeaponUI; }




	 static   CToolUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, const _tchar* _UI_Name);


	virtual void Free() override;
protected:

	//게임 데이터 상속
	//자식들은 각 데이터를 소유 하고 있다
	CItemTool* m_pToolData[ITEM_END];


	BOOL m_bSlideState;
private:
	static  CAliveUI* m_pAliveUI;
	static  CEquiment* m_pEquimentUI;
	static  CLightUI* m_pLightUI;
	static  CScienceUI* m_pScienceUI;
	static  CClothUI* m_pClothUI;
	static  CWeaponUI* m_pWeaponUI;

	static CSlideUI* m_pSlideUI[5];
};

