
#include "Engine_Define.h"
#include "CSoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{

}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);
	m_pChannelArr[(int)eCHANNELID::SOUND_ATTACK1]->setPitch(1.5f);
	m_pChannelArr[(int)eCHANNELID::SOUND_ATTACK2]->setPitch(1.5f);
	m_pChannelArr[(int)eCHANNELID::SOUND_ATTACK3]->setPitch(1.5f);
}
void CSoundMgr::Release()
{
	m_pSystem->release();
	m_pSystem->close();

}




void CSoundMgr::PlaySound(FMOD::Sound* pSoundKey, eCHANNELID eID, float fVolume)
{

	bool isPlaying;
	m_pChannelArr[(int)eID]->isPlaying(&isPlaying);
	if (!isPlaying)
	{
		m_pChannelArr[(int)eID]->setVolume(fVolume);
		m_pSystem->playSound(pSoundKey, NULL, 0, &m_pChannelArr[(int)eID]);

	}
	m_pSystem->update();
}

void CSoundMgr::PlayBGM(FMOD::Sound* pSoundKey, float fVolume)
{
	m_pChannelArr[(int)eCHANNELID::SOUND_BGM]->stop();
	m_pSystem->playSound(pSoundKey, NULL, 0, &m_pChannelArr[(int)eCHANNELID::SOUND_BGM]);
	m_pChannelArr[(int)eCHANNELID::SOUND_BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[(int)eCHANNELID::SOUND_BGM]->setVolume(fVolume);
	m_pSystem->update();
}

void CSoundMgr::StopSound(eCHANNELID eID)
{
	m_pChannelArr[(int)eID]->stop();
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < (int)eCHANNELID::MAXCHANNEL; ++i)
		m_pChannelArr[i]->stop();
}

void CSoundMgr::SetChannelVolume(eCHANNELID eID, float fVolume)
{
	m_pChannelArr[(int)eID]->setVolume(fVolume);
	m_pSystem->update();

}

void Engine::CSoundMgr::Free()
{
}
