#pragma once

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update();

	Matrix View() { return view; }
	
	void Position(float x, float y);
	void PositionX(float x);
	void PositionY(float y);
	void Position(Vector2& pos);
	Vector2 Position() { return position; }

protected:
	Vector2 position;

private:
	Matrix view;

};

