#pragma once
#include <Windows.h>
#include <unordered_map>

#include <d3dx10.h>
#include "Textures.h"


using namespace std;

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	void Draw(float x, float y);
};

typedef CSprite* LPSPRITE;
/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(int id);
	void Clear();

	static CSprites* GetInstance();
};

