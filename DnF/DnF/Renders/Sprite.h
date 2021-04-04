#pragma once
/*	Sprite : 잘라 쓰는 이미지
	한 이미지에 달리는 모션이 각 프레임별로 한번에 저장됨
		=> 각각을 잘라 시간에 따라 보여주어 애니메이션 재생
		=> Sprite Animation
	#Spine : bone구조의 이미지
*/

enum class CenterPoint {
	Center,
	LeftUp,
	CenterDown
};

class Sprite
{
public:
	Sprite(wstring textureFile, wstring shaderFile, CenterPoint mode = CenterPoint::Center);
	Sprite(wstring textureFile, wstring shaderFile, float endX, float endY, CenterPoint mode = CenterPoint::Center);	// 이미지의 왼쪽 위를 자를때
	Sprite(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY, CenterPoint mode = CenterPoint::Center);	// 이미지의 중간을 자를때
	~Sprite();

	void Initialize(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY, CenterPoint mode);

	void Update(Matrix& V, Matrix& P);
	void Render();
	void UpdateWorld();


	void Position(float x, float y);
	void Position(Vector2& position);
	void Position(Vector3& position);
	Vector3 Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& scale);
	void Scale(Vector3& scale);
	Vector3 Scale() { return scale; }
	Vector2 ScaledSize();
	Vector3 ScaledSize3D();

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& rotation);
	Vector3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& rotation);
	Vector3 RotationDegree();

	Vector2 TextureSize() { return textureSize; }
	Matrix World() { return world; }

	void Pass(UINT val) { pass = val; }
	Shader* GetShader() { return shader; }

private:
	struct Vertex
	{
		Vector3 position;
		Vector2 uv;
	};

	Shader* shader;
	UINT pass;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	Matrix world;
	ID3DX11EffectMatrixVariable* sWorld;
	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjection;

	Vector2 textureSize;

	wstring textureFile;
	ID3D11ShaderResourceView* srv;

};




//#############################################################################

// srv를 관리
// srv : 이미지 한개를 가져옴
//	sprite는 위 이미지에서 잘라서 부분적으로 써야 하기에 관리 필요
class Sprites
{
private:
	static ID3D11ShaderResourceView* Load(wstring file, wstring shaderFile, Shader** shader);
	static void Remove(wstring file);



private:
	friend class Sprite;
	// friend 다른 클래스에서도 private사용 가능, 상속보다 강한 개념
	//	=> Sprite에서 Sprites의 private를 사용 가능

	struct SpriteDesc {
		UINT RefCount = 0;
		ID3D11ShaderResourceView* SRV = NULL;
	};

	static map<wstring, SpriteDesc> spriteMap;
	static map<wstring, Shader*> shaderMap;


};