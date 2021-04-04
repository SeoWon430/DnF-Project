#pragma once
/*	Sprite : �߶� ���� �̹���
	�� �̹����� �޸��� ����� �� �����Ӻ��� �ѹ��� �����
		=> ������ �߶� �ð��� ���� �����־� �ִϸ��̼� ���
		=> Sprite Animation
	#Spine : bone������ �̹���
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
	Sprite(wstring textureFile, wstring shaderFile, float endX, float endY, CenterPoint mode = CenterPoint::Center);	// �̹����� ���� ���� �ڸ���
	Sprite(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY, CenterPoint mode = CenterPoint::Center);	// �̹����� �߰��� �ڸ���
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

// srv�� ����
// srv : �̹��� �Ѱ��� ������
//	sprite�� �� �̹������� �߶� �κ������� ��� �ϱ⿡ ���� �ʿ�
class Sprites
{
private:
	static ID3D11ShaderResourceView* Load(wstring file, wstring shaderFile, Shader** shader);
	static void Remove(wstring file);



private:
	friend class Sprite;
	// friend �ٸ� Ŭ���������� private��� ����, ��Ӻ��� ���� ����
	//	=> Sprite���� Sprites�� private�� ��� ����

	struct SpriteDesc {
		UINT RefCount = 0;
		ID3D11ShaderResourceView* SRV = NULL;
	};

	static map<wstring, SpriteDesc> spriteMap;
	static map<wstring, Shader*> shaderMap;


};