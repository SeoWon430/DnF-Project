#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
	:count(15), volume(1.0f)
{
	// System�� comInterfaceó�� �Լ�, ����ü�� ����
	System_Create(&system);
	system->init(count, FMOD_INIT_NORMAL, NULL);
				//�����, �⺻(PC��), �߰� ���� ��ġ

	sound = new Sound*[count];
	channel = new Channel*[count];

	ZeroMemory(sound, sizeof(Sound*)*count);
	ZeroMemory(channel, sizeof(Channel*)*count);
}

SoundManager::~SoundManager()
{
	// ������� ä���� ��� STOP
	if (channel != NULL)
	{
		for (UINT i = 0; i < count; i++)
			if (channel[i]) channel[i]->stop();
	}

	// ����� �Ҹ��� ��� ����
	if (sound != NULL) 
	{
		for (UINT i = 0; i < count; i++) 
			if(channel[i]) sound[i]->release();
	}

	// ���� �Ҵ� ����
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
							// ���� ���, ������(��������),  , ���� ���
							// sound�� ������� ���� ������ �����ؼ� ����
							//		sounds.size()�� ������ ������ �ִ� �ε���
	}
	else {
		system->createStream(soundFile.c_str(), FMOD_DEFAULT, NULL, &sound[sounds.size()]);
	}

	// map�� ����
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
							// FMOD_CHANNEL_FREE : &channel[count]�� ��������� �� ä�ο� ���
							// *iter->second : ����� ����
							// false : �ߴ� ����
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
