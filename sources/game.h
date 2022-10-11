#pragma once
#include <cinttypes>
#include "gameobject.h"
void GameInit();
void GameInput(uint64_t dt);
void GameUpdate(uint64_t dt);
void GameRender(uint64_t dt);
void GameDispose();
