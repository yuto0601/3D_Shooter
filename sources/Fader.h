#pragma once

#include "GameObject.h"
static float a;

void FaderInit();
bool FaderUpdate();
void FaderDraw();
void FaderExit();
void FaderFadeIn();
void FaderFadeOut();
// �t�F�[�h��Ԓ萔
enum STATE {
	NONE, // �t�F�[�h���Ă��Ȃ�
	FADEIN, // �t�F�[�h�E�C�����Ă���
	FADEOUT // �t�F�[�h�E�A�E�g���Ă���
};
int FaderGetState();



