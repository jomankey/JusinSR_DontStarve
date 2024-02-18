#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	enum CHANNELID { 
		SOUND_EFFECT,
		SOUND_BGM,
		SOUND_MAIN_BGM,
		STEREO_EFFECT,
		STEREO_BGM,					//공간음향 적용중
		SOUND_EFFECT_CONTINUE_CH1, //공간음향 적용중
		SOUND_EFFECT_CONTINUE_CH2, //공간음향 적용중
		SOUND_EFFECT_CONTINUE_CH3, //공간음향 적용중
		SOUND_EFFECT_CONTINUE_CH4,
		SOUND_EFFECT_CONTINUE_CH5,
		SOUND_SPIDER,
		SOUND_BEEFALO,
		SOUND_PIG,
		SOUND_DEERCLOPS,
		SOUND_TALLBIRD,
		SOUND_TREE,
		SOUND_ROCK,
		SOUND_TELEPORT,
		SOUND_MOUSE,
		SOUND_PLAYER,
		SOUND_PLAYER_VOICE,
		SOUND_CREATE,
		SOUND_UI,
		SOUND_TORCH,
		MAXCHANNEL };

	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
	enum RESOBJID {
		ROCK,
		TREE,
		PIG_HOUSE,
		GRASS,
		BERRY_BUSH,
		FIRE_FLIES,
		TELEPORTER,
		BOSS_DOOR,
		BON_FIRE,
		TENT,
		COOKING_POT,
		RSOBJ_END
	};

	enum PLAYERSTATE {
		IDLE, MOVE, BUILD, PICKUP, HIT, ATTACK, FALLDOWN, WAKEUP, EAT, DIALOG,
		TORCH_IDLE, TORCH_RUN,
		SPEAR_ATTACK,
		PICKING_OBJECT,
		HAMMERING,
		AXE_CHOP_PRE,
		AXE_CHOP_LOOP,
		SLEEP,
		DEAD,
		REBIRTH,
		STATE_END
	};
	enum LOOKDIR { LOOK_DOWN, LOOK_RIGHT, LOOK_LEFT, LOOK_UP, LOOK_END };
	enum WEAPON { UNARMED, AXE, TORCH, HAMMER, PICK, SPEAR, WEAPON_END }; //플레이어 무기
	enum class eSCENE_TYPE
	{
		LOGO,
		STAGE,
		ROAD,
		BOSS,
		TEST,
		NONE,
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
		MOUSE,
		ITEM,
		MONSTER,
		PLAYER,
		RESOURCE_OBJECT,
		OBJECT,
		TRAP,
		UI,
		BOSS,
		EFFECT,
		PANEL,
		PARTICLE,
		END = 32,
	};
	enum UI_STATE { UI_STATIC, UI_DYNAMIC, UI_NONE };

	enum TIME_STATE { MORNING, AFTERNOON, NIGHT };


	//이건 사용안하는중 
	enum class eCHANNELID
	{
		SOUND_EFFECT,
		SOUND_BTN,
		SOUND_BGM,
		SOUND_ATTACK1,
		SOUND_ATTACK2,
		SOUND_ATTACK3,
		SOUND_HIT,
		SOUND_DEAD1,
		SOUND_LDY1,
		SOUND_LDY2,
		SOUND_LDY3,
		SOUND_SELECT,
		SOUND_ENEMY_DEAD,
		SOUND_ENEMY_HIT,
		SOUND_ENEMY_ATTACK,
		SOUND_EFFECT1,
		SOUND_EFFECT2,
		SOUND_EFFECT3,
		SOUND_MOUSE,
		MAXCHANNEL,
	};


	enum class eEVENT_TYPE
	{
		CREATE_OBJECT,		// CreateObject
		DELETE_OBJECT,		// DeleteObject
		SCENE_CHANGE,		// SceneChange
		NONE,
	};

	enum  eITEMTOOL_TYPE {
		ITEM_EQUIP,
		ITEM_ALIVE,
		ITEM_WEAPON,
		ITEM_PROCESSING,
		ITEM_END
	};

	enum UI_ITEM_TYPE { // 인벤/ 생산용인지
		UI_ITEM_INVEN,
		UI_ITEM_CREATE,
		UI_ITEM_CREATE_NEED,
		UI_ITEM_COOK,
		UI_ITEM_END
	};

	enum BOX_TYPE {
		INVEN,
		CREATE,
		COOK,
		BOX_END
	};

	enum BOX_DIR { // 나중에 지울것 
		WIDTH,
		HEIGHT,
		BOX_DIR_END
	};

	enum ARMOR_SLOT_TYPE {
		ARMOR = 15,
		HAND,
		HEAD,
		ARMOR_SLOT_END
	};



	enum class eTRAP_STATE
	{
		HIDE,	//숨어있는상태
		PLACE,	//생성중인상태
		ATK,	//공격중인상태
		GROW,	//나오고있는상태
		MOVE,	//움직이고있는상태
		IDLE,	//나온상태
		HIT,	//부딫힌상태
		DEAD,	//죽은상태
		NONE
	};

	enum class eROAD_CAMERA_STATE
	{
		RUN,
		TURN,
		BOSS,
		BOSS_TAUNT,
		MOVE_MPA,
		NONE,
	};
}
#endif // Engine_Enum_h__
