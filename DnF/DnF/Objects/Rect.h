#pragma once





class Rect
{
public:
	Rect(wstring shaderFile);
	Rect(wstring shaderFile, Vector2 position, Vector2 scale, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));
	virtual ~Rect();

	virtual void Update(Matrix& V, Matrix &P);
	void Render();



	void Position(float x, float y);
	void Position(Vector2& pos);
	Vector2 Position();

	void Scale(float x, float y);
	void Scale(Vector2& scal);
	Vector2 Scale();

	void Color(float r, float g, float b, float a = 1.0f);
	void Color(D3DXCOLOR& color);
	D3DXCOLOR Color();


	Matrix World() { return world; }
	void World(Matrix& matrix) { world = matrix; }

private:
	void CreateBuffer(wstring shaderFile);
	void Initialize(wstring shaderFile);

	void UpdateWorld();

private:
	struct Vertex
	{
		Vector3 Position;
	};

	D3DXCOLOR color;
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	Matrix world;

	UINT pass;

	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	Collider* collider;
	
	ID3DX11EffectMatrixVariable* sWorld;
	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjection;
	ID3DX11EffectVectorVariable* sColor;


};