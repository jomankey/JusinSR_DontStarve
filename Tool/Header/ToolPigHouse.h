#pragma once
#include "Export_Utility.h"

class CToolPigHouse :
    public CGameObject
{
private:
    explicit CToolPigHouse();
    virtual ~CToolPigHouse();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	static CToolPigHouse* Create();


};

