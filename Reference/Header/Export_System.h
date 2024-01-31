#pragma once

#include "Engine_Define.h"
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "TimerMgr.h"
#include "FontMgr.h"
#include "InputDev.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CollisionMgr.h"

namespace Engine 
{

	// GraphicDev

	inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _ulong& dwSizeX, const _ulong& dwSizeY, CGraphicDev** ppGraphicDev);
	inline void			Render_Begin(D3DXCOLOR Color);
	inline void			Render_End(void);

	// TimerMgr

	inline _float		Get_TimeDelta(const _tchar* pTimerTag);
	inline void		Update_TimeDelta(const _tchar* pTimerTag);
	inline HRESULT		Ready_Timer(const _tchar* pTimerTag);

	// FrameMgr

	inline _bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	inline HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

	// FontMgr
	inline HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pFontType,
		const _tchar* pFontTag,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	inline void		Render_Font(const _tchar* pFontTag,
		const _tchar* pString,
		const _vec2* pPos,
		D3DXCOLOR Color);

	// InputDev

	inline _byte	Get_DIKeyState(_ubyte byKeyID);
	inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	inline HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	inline void		Update_InputDev(void);

	//Collision
	inline _bool Collision_Item(_vec3 pPlayerPos, _vec3 pItemPos, _vec3 vPlayerScale, _vec3 vItemScale);
	inline _bool Collision_Monster(
		_vec3 vPlayerPos,
		_vec3* vPlayerAxis,
		_vec3 vMonsterPos,
		_vec3* vMonsterAxis,
		_vec3 vPlayerScale,
		_vec3 vMonsterScale);


	// PathMgr
	inline HRESULT			Ready_PathMgr();
	inline wstring GetRelativePath(const wchar_t* filePath);
	inline const _tchar* GetResourcePath();


	//ResMgr
	inline void Save_TexturePath(wstring _fileName, wstring _key, TEXTUREID _textureTag, wstring _subPath, int num);

	// Destroy
	inline void			Release_System();

#include "Export_System.inl"

}