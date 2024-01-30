#pragma once
#include "VIBuffer.h"

namespace Engine {

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex() = delete;
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone();

private:
	virtual void	Free();
};

}