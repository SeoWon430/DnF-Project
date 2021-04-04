#pragma once

class Canvas
{
public:
	Canvas(Vector2 position, Vector2 scale);
	~Canvas();

	void Render();

	void SRV(ID3D11ShaderResourceView* srv);


	void Position(float x, float y);
	void Position(Vector2& pos);
	Vector2& Position() { return position; }

	void Scale(float x, float y);
	void Scale(Vector2& scal);
	Vector2& Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& rot);
	Vector3& Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& rot);
	Vector3& RotationDegree();

	Shader* GetShader() { return shader; }
	void Pass(UINT num) { pass = num; }

private:
	void CreateBuffer();
	void UpdateWorld();


private:
	struct Vertex
	{
		Vector3 position;
		Vector2 Uv;
	} vertices[4];

	// v, p는 따로 처리
	struct ViewpProjection	
	{
		Matrix View;
		Matrix Projection;
	} vp;

	Shader* shader;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Vector2 position;
	Vector2 scale;
	Vector3 rotation;

	UINT pass;

	ID3DX11EffectMatrixVariable* sWorld, *sView, *sProjection;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;

};