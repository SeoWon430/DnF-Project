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
	// FMOD::Sound** -> Sound** : �Ҹ� ��ü
	// FMOD::System -> dx�� device �� ����

	System* system;		// ���� ��ġ�� ����
						// ���� ���� ��ġ(����ī��)�� ���κ��忡 ����

	Sound** sound;		// �ѱ� ���� ������
						// �޸𸮿� �÷��� �ϱ⿡ ���̴� ���� ����
					// ���� ��ü�� *�� ������, �迭���� �Ұ��̱⿡ **�� ��

	Channel** channel;	// ��� ���
						// Ư�� �Ҹ��� Ư�� ä�ο� ���, �ִ� ��� ��

	UINT count;		// ���� ��� �ִ� �Ҹ� ����

	map<string, Sound**> sounds;
	map<string, Sound**>::iterator iter;	// sounds �ݺ��� ��

	float volume;

};

/*
wav : ���ս�, ���α׷� ����� ����
mp3 : ����, ���α׷� ����� ����
midi : �Ǻ��� ���� ����, ����ī�忡 ���� �������� �޶���
	=> ����� ���κ��忡 ����ī�尡 �������� ���� ��� ������

����� �ַ� midi
�ܹ��� wav�� �ַ� ��

*/



