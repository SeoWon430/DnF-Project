#pragma once

class Background
{
public:
	Background(wstring shaderFile);
	~Background();

	void Update(Matrix& V, Matrix& P);
	void Render();

private:
	Sprite* cloud[2];
	Sprite* bush[2];
	Sprite* tile[2];

};