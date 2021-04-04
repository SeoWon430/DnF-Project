#include "stdafx.h"
#include "Objects/Player/Player.h"
#include "Objects/Monster/Monster.h"
#include "Objects/Monster/Leveche.h"
#include "S01_DungeonSiroco.h"

S01_DungeonSiroco::S01_DungeonSiroco(SceneValues * values)
	:SceneDungeon(values)
{
	wstring shaderFile = L"Sprite.fx";

	//background = new Sprite(L"Seria_Room.png", shaderFile);
	background = new Sprite(L"map/TrueCoffin_01.png", shaderFile);
	background->Scale(1.5f, 1.5f);
	backgroundFar = new Sprite(L"map/TrueCoffin_02_Back.png", shaderFile);
	backgroundFar->Scale(1.25f, 1.25f);

	Audio->AddSound("BGM", "./_Sounds/terra.mid");
	Audio->AddSound("Gun", "./_Sounds/Gun.wav");
	//Audio->Play("BGM");

	this->player = values->player;

	monster = new Leveche();
	monster->Position(300, -200);
	this->player->Position(0, -200);
	monster->SetTarget(this->player);
}

S01_DungeonSiroco::~S01_DungeonSiroco()
{
	SafeDelete(background);
}

void S01_DungeonSiroco::Update()
{
	Matrix V = values->MainCamera->View();
	Matrix P = values->Projection;

	background->Update(V, P);

	Vector3 offset = Vector3(0, 400, 0);
	offset.x = (background->Position().x - player->Position().x) / 25.0f;
	backgroundFar->Position(background->Position() + offset);
	backgroundFar->Update(V, P);


	monster->Update(V, P);
	player->Update(V, P);

	player->SkillAttack(monster);
	monster->PatternAttack(player);
	PlayInterface::Get()->Update(V, P);
}

void S01_DungeonSiroco::Render()
{
	backgroundFar->Render();
	background->Render();
	if (player->Position().z > monster->Position().z)
	{
		player->Render();
		monster->Render();
	}
	else
	{
		monster->Render();
		player->Render();
	}
	PlayInterface::Get()->Render();
}


Vector2 S01_DungeonSiroco::SceneArea()
{
	return background->ScaledSize();
}
