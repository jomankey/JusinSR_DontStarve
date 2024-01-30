#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CustomizeTex : public CVIBuffer
{
private:
	explicit CustomizeTex();
	explicit CustomizeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CustomizeTex(const CustomizeTex& rhs);
	virtual ~CustomizeTex();

public:
	virtual HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void		Render_Buffer();

public:
	static CustomizeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual CComponent*	Clone();

private:
	virtual void	Free();
};

END