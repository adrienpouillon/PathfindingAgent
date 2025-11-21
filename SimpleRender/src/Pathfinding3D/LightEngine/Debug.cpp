#include "pch.h"

#include "Debug.h"

#include "GameManager.h"

#include <string>

Debug* Debug::Get()
{
	static Debug mInstance;

	return &mInstance;
}

void Debug::Draw(Window* pWindow)
{
	/*
	for (Sphere& sphere : mSpheres)
	{
		pWindow->Draw(sphere);
	}

	mSpheres.clear();

	for (Cube& cube : mCubes)
	{
		pWindow->Draw(cube);
	}

	mCubes.clear();

	for (Cylinder& line : mLines)
	{
		pWindow->Draw(line);
	}

	mLines.clear();
	*/

	for (Text* text : mpTexts)
	{
		pWindow->DrawText(*text);
	}
}

void Debug::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, const gce::Vector3f32& color)
{
	//Cylinder line;

	//Debug::Get()->mLines.push_back(line);
}

void Debug::DrawCube(float x, float y, float z, gce::Vector3f32 scale, const gce::Vector3f32& color)
{
	/*Cube cube;

	cube.SetScale(scale);
	cube.SetColor(color);
	cube.SetPosition(gce::Vector3f32(x, y, z));*/

	//Debug::Get()->mCubes.push_back(cube);
}

void Debug::DrawSphere(float x, float y, float z, float radius, const gce::Vector3f32& color)
{
	/*Sphere sphere;

	sphere.SetScale(gce::Vector3f32(radius * 2, radius * 2, radius * 2));
	sphere.SetColor(color);
	sphere.SetPosition(gce::Vector3f32(x, y, z));*/

	//Debug::Get()->mSpheres.push_back(sphere);
}

void Debug::DrawText(float x, float y, const std::wstring& text, float size, const gce::Color& color)
{
	DrawText(x, y, text, 0.f, 0.f, size, color);
}

void Debug::DrawText(float x, float y, const std::wstring& text, float ratioX, float ratioY, float size, const gce::Color& color)
{
	_ASSERT(ratioX >= 0.f && ratioX <= 1.f);
	_ASSERT(ratioY >= 0.f && ratioY <= 1.f);

	Text* txt = new Text(text);

	txt->SetText(text);
	txt->SetColor(color);
	txt->SetPosition(gce::Vector2f32(x, y));

	Debug::Get()->mpTexts.push_back(txt);
}
