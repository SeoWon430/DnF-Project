#include "stdafx.h"
#include "Background.h"

Background::Background(wstring shaderFile)
{
	wstring textureFile = L"";

	textureFile = L"./Background/Cloud.png";
	cloud[0] = new Sprite(textureFile, shaderFile, 137, 128);
	cloud[1] = new Sprite(textureFile, shaderFile, 144, 0, 336, 128);

	textureFile = L"./Background/Bush.png";
	bush[0] = new Sprite(textureFile, shaderFile);
	textureFile = L"./Background/Bush2.png";
	bush[1] = new Sprite(textureFile, shaderFile);

	textureFile = L"./Background/Tile.png";
	tile[0] = new Sprite(textureFile, shaderFile);
	tile[1] = new Sprite(textureFile, shaderFile, 0, 66, 0, 0);

}

Background::~Background()
{
	SafeDelete(cloud[0]);
	SafeDelete(cloud[1]);
	SafeDelete(bush[0]);
	SafeDelete(bush[1]);
	SafeDelete(tile[0]);
	SafeDelete(tile[1]);
}

void Background::Update(Matrix & V, Matrix & P)
{
	cloud[0]->Update(V, P);
	cloud[1]->Update(V, P);

	bush[0]->Update(V, P);
	bush[1]->Update(V, P);

	tile[0]->Update(V, P);
	tile[1]->Update(V, P);
}

void Background::Render()
{


	cloud[0]->Position(40, 520);
	cloud[0]->Render();
	cloud[0]->Position(200, 480);
	cloud[0]->Render();

	cloud[1]->Position(500, 500);
	cloud[1]->Render();

	bush[1]->Position(-50, 180);
	bush[1]->Render();	   
	bush[1]->Position(300, 180);
	bush[1]->Render();	   
	bush[1]->Position(600, 180);
	bush[1]->Render();	   
						   
						   
	bush[0]->Position(100, 125.0f);
	bush[0]->Render();	   
	bush[0]->Position(650, 125.0f);
	bush[0]->Render();

	float tileSize = tile[0]->TextureSize().x;
	for (int i = 0; i < 7; i++) 
	{
		tile[0]->Position((float)i * tileSize, 45);
		tile[0]->Render();

		tile[1]->Position((float)i * tileSize, 0);
		tile[1]->Render();
	}
}
