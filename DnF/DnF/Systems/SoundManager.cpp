#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
	:count(15), volume(1.0f)
{
	// System도 comInterface처럼 함수, 구조체로 만듦
	System_Create(&system);
	system->init(count, FMOD_INIT_NORMAL, NULL);
				//재생수, 기본(PC용), 추가 사운드 장치

	sound = new Sound*[count];
	channel = new Channel*[count];

	ZeroMemory(sound, sizeof(Sound*)*count);
	ZeroMemory(channel, sizeof(Channel*)*count);
}

SoundManager::~SoundManager()
{
	// 재생중인 채널을 모두 STOP
	if (channel != NULL)
	{
		for (UINT i = 0; i < count; i++)
			if (channel[i]) channel[i]->stop();
	}

	// 저장된 소리들 모두 제거
	if (sound != NULL) 
	{
		for (UINT i = 0; i < count; i++) 
			if(channel[i]) sound[i]->release();
	}

	// 동적 할당 해제
	SafeDeleteArray(sound);
	SafeDeleteArray(channel);


	if (system != NULL) 
	{
		system->release();
		system->close();
	}

	sounds.clear();
}

void SoundManager::AddSound(string name, string soundFile, bool isLoop)
{

	if (isLoop == true) 
	{
		system->createStream(soundFile.c_str(), FMOD_LOOP_NORMAL, NULL, &sound[sounds.size()]);
							// 파일 경로, 재생모드(루프인지),  , 저장 장소
							// sound에 순서대로 사운드 파일을 생성해서 넣음
							//		sounds.size()는 마지막 순서에 넣는 인덱스
	}
	else {
		system->createStream(soundFile.c_str(), FMOD_DEFAULT, NULL, &sound[sounds.size()]);
	}

	// map에 저장
	sounds[name] = &sound[sounds.size()];
}

void SoundManager::Play(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++) 
	{
		if (name == iter->first) 
		{
			system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &channel[count]);
							// FMOD_CHANNEL_FREE : &channel[count]이 비어있으면 이 채널에 재생
							// *iter->second : 재생할 사운드
							// false : 중단 여부
			channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::Stop(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPlaying(string name)
{
	bool result = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->isPlaying(&result);
			break;
		}
	}

	return result;
}

bool SoundManager::IsPaused(string name)
{
	bool result = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&result);
			break;
		}
	}

	return result;
}

void SoundManager::Volume(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setVolume(volume);
			break;
		}
	}
}

void SoundManager::Update()
{
	system->update();
}
