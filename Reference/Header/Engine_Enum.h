#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	enum CHANNELID { SOUND_EFFECT, SOUND_BGM, MAXCHANNEL };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum PLAYERSTATE {
		IDLE, MOVE, BUILD, PICKUP, HIT, ATTACK, FALLDOWN, WAKEUP, EAT, STATE_END
	};
	enum LOOKDIR { LOOK_DOWN, LOOK_RIGHT, LOOK_LEFT, LOOK_UP, LOOK_END };
	
	enum class eSCENE_TYPE
	{
		LOGO,
		STAGE,
		END
	};

	enum class eLAYER_TYPE
	{
		ENVIRONMENT,
		GAME_LOGIC,
		FORE_GROUND,
		END,
	};
	enum class eOBJECT_GROUPTYPE
	{
		TILE,
		CAMERA,
		BACK_GROUND,
		ITEM,
		MONSTER,
		PLAYER,
		RESOURCE_OBJECT,
		OBJECT,
		UI,
		END = 32,
	};
	enum UI_STATE { UI_STATIC, UI_DYNAMIC, UI_NONE };

	enum  eITEMTOOL_TYPE {
		ITEM_TOOL,
		ITEM_LIGHT,
		ITEM_ALIVE,
		ITEM_EQUIP,
		ITEM_END
	};
}
#endif // Engine_Enum_h__
//일단 scene안에 레이어를 3개만들거야 배경레이어,
//게임플레이레이어,전경레이어 이렇게 그리고 각레이어들은 
//열거형으로만든 오브젝트 리스트만큼 배열을 가지고있을거야 
//UI,ITEM,MONSTER,PLAYER,TILE,BACKGROUND,EFFECT
//그리고 많이 접근해야하는건 매니저로 두던가
//Scene클래스에 멤버변수로 아예 가지게 만들어서
//GetCurScene호출해서 멤버변수를 받아오게 할거야