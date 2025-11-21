#pragma once
#include "pch.h"
#include <vector>
#include <string>
#include "lib/Geometries.h"

class GameManager;

namespace sf
{
	class Color;
	class RenderWindow;
}

class Debug
{
public:
	//std::vector<Cylinder> mLines;
	std::vector<Text*> mpTexts;
	//std::vector<Sphere> mSpheres;
	//std::vector<Cube> mCubes;
	void Draw(Window* pWindow);

public:
	static Debug* Get();

	static void DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, const gce::Vector3f32& color);
	static void DrawCube(float x, float y, float z, gce::Vector3f32 scale, const gce::Vector3f32& color);
	static void DrawSphere(float x, float y, float z, float radius, const gce::Vector3f32& color);
	static void DrawText(float x, float y, const std::wstring& text, float size, const gce::Color& color);
	static void DrawText(float x, float y, const std::wstring& text, float ratioX, float ratioY, float size, const gce::Color& color);

	friend GameManager;
};

