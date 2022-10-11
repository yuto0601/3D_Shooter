#pragma once

#include "GameObject.h"
static float a;

void FaderInit();
bool FaderUpdate();
void FaderDraw();
void FaderExit();
void FaderFadeIn();
void FaderFadeOut();
// フェード状態定数
enum STATE {
	NONE, // フェードしていない
	FADEIN, // フェード・インしている
	FADEOUT // フェード・アウトしている
};
int FaderGetState();



