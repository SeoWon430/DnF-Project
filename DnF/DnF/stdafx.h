#pragma once

// Basic
#include <Windows.h>
#include <assert.h>


// STL
#include <vector>	
#include <queue>
#include <string>	
#include <bitset>
#include <map>	
#include <unordered_map>	
#include <functional>	
#include <mutex>
using namespace std;


// DX Lib
#include <d3d11.h>
#include <D3DX11.h>		
#include <D3DX10.h>
#include <D3DX10math.h>	

#include <d3dx11effect.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")	
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")


// ImGui : Runtime Debug API
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

// DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// FMOD : Sound API
#include "fmod/fmod.hpp"
#pragma comment(lib, "./fmod/fmodex_vc.lib")


// TypeDefine
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXMATRIX Matrix;
typedef D3DXCOLOR Color;


// Macro
#define SafeDelete(p){ if(p) { delete (p); (p) = NULL; } }
#define SafeDeleteArray(p) { if(p) { delete[] (p); (p) = NULL;} }
#define SafeRelease(p){ if(p) { (p)->Release(); (p) = NULL; } }


// Framework
#include "Systems/Keyboard.h"
#include "Systems/MouseManager.h"
#include "Systems/Time.h"
#include "Systems/DirectWrite.h"
#include "Systems/SoundManager.h"

#include "Viewer/Camera.h"
#include "Viewer/Freedom.h"
#include "Viewer/Following.h"

#include "Utilities/String.h"
#include "Utilities/Math.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Path.h"
#include "Utilities/Xml.h"
#include "Utilities/Queue.h"

#include "Renders/Shader.h"
#include "Renders/Sprite.h"
#include "Renders/Clip.h"
#include "Renders/Animation.h"
#include "Renders/RenderTarget.h"
#include "Renders/Canvas.h"

#include "Collider/Collider.h"
#include "Collider/BoxCollider.h"
#include "Collider/CylinderCollider.h"
#include "Collider/ColliderFactory.h"
#include "Objects/GameObject.h"
#include "Objects/Effect.h"
#include "Objects/Player/Skill/Action.h"
#include "Objects/Player/Player.h"
#include "Objects/Player/PlayInterface.h"
#include "Objects/Monster/Monster.h"


// ##########################################################
// ##########################################################
// ##########################################################

// Device
extern UINT WIDTH;	
extern UINT HEIGHT;
extern HWND Hwnd;
extern wstring Title;

// DX ComInterface
extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

// Input/Output
extern Keyboard* Key;
extern MouseManager* Mouse;
extern SoundManager* Audio;