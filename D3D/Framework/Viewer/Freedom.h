#pragma once

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	void Update() override;

	void Speed(float moveSpeed, float rotationSpeed = 2.f);

private:
	float moveSpeed = 20.f;
	float rotationSpeed = 2.f;
};