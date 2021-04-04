#include "stdafx.h"
#include "Systems/Device.h"
#include "Objects/Background.h"
#include "Objects/Player/Player.h"
#include "Objects/Player/Player_Thief.h"
#include "Objects/Player/PlayInterface.h"
#include "Scenes/Scene.h"
#include "Scenes/S00_SeriaRoom.h"
#include "Scenes/S01_DungeonSiroco.h"

// Device.cpp에서 호출

SceneValues* values;
vector<Scene*> scenes;
Map currentScene;

D2D1_RECT_F area;
Vector2 sceneArea;

void InitScene()
{
	currentScene = Map::SeriaRoom;

	PlayInterface::Create();
	ColliderFactory::Create();

	values = new SceneValues();
	values->MainCamera = new Freedom();
	values->player = new Player_Thief(L"그림리퍼");

	PlayInterface::Get()->SetCamera(values->MainCamera);

	D3DXMatrixIdentity(&values->MainCamera->View());
	D3DXMatrixIdentity(&values->Projection);

	scenes.push_back(new S01_DungeonSiroco(values));


	// Player 이동 제한
	sceneArea = scenes[(UINT)currentScene]->SceneArea() * 0.5f;
	Vector2 playerScale = values->player->ScaledSize() * 0.5f;
	area.left = -sceneArea.x + playerScale.x;
	area.top = +sceneArea.y - playerScale.y;
	area.right = +sceneArea.x - playerScale.x;
	area.bottom = -sceneArea.y + playerScale.y;
}

void DestroyScene() 
{

	PlayInterface::Delete();
	ColliderFactory::Delete();
	for (Scene* s : scenes)
		SafeDelete(s);

	SafeDelete(values->player);
	SafeDelete(values);
}



void Update()
{
	float x = Math::Clamp<float>(values->player->Position().x, area.left, area.right);
	float y = 0;
	if (values->player->IsGround() == true)
		y = Math::Clamp<float>(values->player->Position().y, area.bottom - 100.0f, area.top / 2.0f - 100.0f);
	else
		y = Math::Clamp<float>(values->player->Position().y, area.bottom - 100.0f, area.top);

	
	values->player->Position(x, y);

	// Camera이동 및 제한
	Vector2 cameraTarget = values->player->GroundPosition();
	float areaWidth = sceneArea.x - WIDTH * 0.5f;
	float areaHeight = sceneArea.y - HEIGHT * 0.5f;
	if (cameraTarget.x < -areaWidth) 
		cameraTarget.x = -areaWidth;
	else if (cameraTarget.x > areaWidth)
		cameraTarget.x = areaWidth;

	if (cameraTarget.y < -areaHeight)
		cameraTarget.y = -areaHeight;
	else if (cameraTarget.y > areaHeight)
		cameraTarget.y = areaHeight;

	values->MainCamera->Position(cameraTarget);

	// View(Camera)
	values->MainCamera->Update();

	// Projection
	D3DXMatrixOrthoOffCenterLH(&values->Projection, WIDTH*(-0.5f), WIDTH*0.5f, HEIGHT*(-0.5f), HEIGHT*0.5f, -1, 1);

	// Scene
	scenes[(UINT)currentScene]->Update();

}



void Render() 
{
	// 3D Render
	D3DXCOLOR bgColor = D3DXCOLOR(0.25f, 0.25f ,0.25f ,1);
	DeviceContext->ClearRenderTargetView(RTV, bgColor);
	{
		scenes[(UINT)currentScene]->Render();
	}
	ImGui::Render();


	// 2D Render
	DirectWrite::GetDC()->BeginDraw();
	{
		wstring str;
		RECT rect = { 0, 0, 500, 200 };
		str = L"FPS : " + to_wstring(ImGui::GetIO().Framerate);
		DirectWrite::RenderText(str, rect);
	}
	DirectWrite::GetDC()->EndDraw();

	SwapChain->Present(0, 0);
}