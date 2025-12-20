// MIT License
//
// Copyright (c) 2025 moi_c_sam, ArthurPV
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <coulang/macros.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  Position position;
} Missile;

typedef struct {
  Position position;
  int hp;
} Asteroid;

Missile *InitMissile(int x, int y) {
  Missile *missile = COULANG_ALLOC(sizeof(Missile));
  missile->position.x = x;
  missile->position.y = y;
  return missile;
}

void DeinitMissile(Missile *missile) { free(missile); }

void UpdatePosition_Missile(Missile *missile) { missile->position.y--; }

int GetPositionX_Missile(Missile *missile) { return missile->position.x; }
int GetPositionY_Missile(Missile *missile) { return missile->position.y; }

Asteroid *InitAsteroid(int x, int y, int hp) {
  Asteroid *asteroid = COULANG_ALLOC(sizeof(Asteroid));
  asteroid->position.x = x;
  asteroid->position.y = y;
  asteroid->hp = hp;
  return asteroid;
}

void DeinitAsteroid(Asteroid *asteroid) { free(asteroid); }

void UpdatePosition_Asteroid(Asteroid *asteroid, int speed) { asteroid->position.y += speed; }

int GetPositionX_Asteroid(Asteroid *asteroid) { return asteroid->position.x; }
int GetPositionY_Asteroid(Asteroid *asteroid) { return asteroid->position.y; }
int GetHP_Asteroid(Asteroid *asteroid) { return asteroid->hp; }
void TakeHit_Asteroid(Asteroid *asteroid) { asteroid->hp--; }
