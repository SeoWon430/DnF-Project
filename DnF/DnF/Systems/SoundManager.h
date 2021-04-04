#pragma once
using namespace FMOD;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void AddSound(string name, string soundFile, bool isLoop = false);
	void Play(string name, float volume = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resume(string name);

	bool IsPlaying(string name);
	bool IsPaused(string name);

	void Volume(string name, float volume);
	float Volume() { return volume; }

	void Update();

private:
	// FMOD::Sound** -> Sound** : 소리 객체
	// FMOD::System -> dx의 device 의 개념

	System* system;		// 사운드 장치에 접근
						// 현재 사운드 장치(사운드카드)는 메인보드에 있음

	Sound** sound;		// 넘길 사운드 데이터
						// 메모리에 올려야 하기에 쓰이는 버퍼 역할
					// 보통 객체는 *로 하지만, 배열까지 할것이기에 **로 함

	Channel** channel;	// 재생 통로
						// 특정 소리는 특정 채널에 재생, 최대 재생 수

	UINT count;		// 동시 재생 최대 소리 갯수

	map<string, Sound**> sounds;
	map<string, Sound**>::iterator iter;	// sounds 반복문 용

	float volume;

};

/*
wav : 무손실, 프로그램 재생은 빠름
mp3 : 압출, 프로그램 재생은 느림
midi : 악보만 가진 파일, 사운드카드에 따라 재생방식이 달라짐
	=> 현재는 메인보드에 사운드카드가 공통으로 들어가서 상관 없어짐

배경은 주로 midi
단발은 wav를 주로 씀

*/



