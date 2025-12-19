# MIT License
#
# Copyright (c) 2025 moi_c_sam, ArthurPV
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

load "libc.so"
	printf int,
	puts int

load "librayglue"
	# void InitWindow(int width, int height, const char *title);
	InitWindow int,
	# bool WindowShouldClose(void);
	WindowShouldClose int,
	# void CloseWindow(void);
	CloseWindow int,
	# void BeginDrawing(void);
	BeginDrawing int,
	# void EndDrawing(void);
	EndDrawing int,
	# void ClearBackgroundRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ClearBackgroundRGBA int,
	# void DrawRectangleRGBA(int posX, int posY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	DrawRectangleRGBA int,
	# void SetTargetFPS(int fps);
	SetTargetFPS int,
    # bool IsKeyPressed2(int key);
	IsKeyPressed2 int,
	# Texture2D *LoadTexturePtr(const char *fileName);
	LoadTexturePtr ptr,
	# void UnloadTexturePtr(Texture2D *texture);
	UnloadTexturePtr int,
	# void DrawTexturePtr(Texture2D *texture, int posX, int posY, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	DrawTexturePtr int,
	# int TextureWidth(Texture2D *texture)
	TextureWidth int,
	# int TextureHeight(Texture2D *texture)
	TextureHeight int

load "libgameglue"
	# Missile* InitMissile(int x, int y)
	InitMissile ptr,
	# void UpdatePosition(Missile* missile)
	UpdatePosition int,
	# int GetPositionX(Missile* missile)
	GetPositionX int,
	# int GetPositionY(Missile* missile)
	GetPositionY int,
	# void DeinitMissile(Missile* missile)
	DeinitMissile int

val KEY_RIGHT int = 262
val KEY_LEFT int = 263
val KEY_DOWN int = 264
val KEY_UP int = 265

val window_width int = 700
val window_height int = 900
val ship_width int = 0
val ship_height int = 0
val ship_texture ptr = 0
val ship_x int = 0
val ship_y int = 0

val missile_width int = 0
val missile_height int = 0
val missile_texture ptr = 0

val missiles_num int = 10
val missiles list = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

# return a number between 0 and 9 if there is a index not use
# return -1 if not
fn get_missiles_free_indx() int {
	val i int = 0
	while i < missiles_num {
		val x ptr = 0
		if missiles[i] == x {
			return i
		}
		i = (i+1)
	}
	return -1
}

fn spawn_missile(positionX int, positionY int) int {
	val indx_free int = get_missiles_free_indx()

	if indx_free != (-1) {
		missiles[indx_free] = InitMissile(positionX, positionY)
		return 0
	}
	return -1
}

fn updatePosition() int {
	val i int = 0
	while i<missiles_num {
		if missiles[i] != 0 {
			UpdatePosition(missiles[i])
		}
		i = (i+1)
	}
	return 0
}

fn del_missiles() int {
	val i int = 0
	while i<missiles_num {
		# if there out of the screen
		if missiles[i] != 0 {
			if GetPositionY(missiles[i]) <= 0 {
				printf("je sup %d\n", i)
				DeinitMissile(missiles[i])
				missiles[i] = 0
			}
		}
		i = (i+1)
	}
}

fn drawShip() int {
	ClearBackgroundRGBA(0, 0, 0, 255)
	DrawTexturePtr(ship_texture, ship_x, ship_y, 255, 255, 255, 255)

	return 0
}

fn drawMissile() int {
	ClearBackgroundRGBA(0, 0, 0, 255)
	val i int = 0
	while i < missiles_num {
		if missiles[i] != 0 {
			val missile_x int = GetPositionX(missiles[i])
			val missile_y int = GetPositionY(missiles[i])
			DrawTexturePtr(missile_texture, missile_x, missile_y, 255, 255, 255, 255)
		}
		i = (i + 1)
	}
	return 0
}

fn handle_events() int {
    if IsKeyPressed2(KEY_RIGHT) {
		if ship_x < (window_width - ship_width) {
			ship_x = (ship_x + 10)
		} else {
			ship_x = (window_width - ship_width)
		}
    }

    if IsKeyPressed2(KEY_LEFT) {
		if ship_x > 0 {
			ship_x = (ship_x - 10)
		} else {
			ship_x = 0
		}
    }

	return 0
}


fn main() int {
	InitWindow(window_width, window_height, "Game")

	SetTargetFPS(60)

	ship_texture = LoadTexturePtr("game/asset/vaisseau.png")
	ship_width = TextureWidth(ship_texture)
	ship_height = TextureHeight(ship_texture)
	ship_y = (window_height - ship_height)

	missile_texture = LoadTexturePtr("game/asset/misil1.png")
	missile_width = TextureWidth(missile_texture)
	missile_height = TextureHeight(missile_texture)

	val x int = 0

	while !WindowShouldClose() {
		handle_events()

		updatePosition()
		spawn_missile(x, ship_y)

		BeginDrawing()
			drawShip()
			drawMissile()
		EndDrawing()

		del_missiles()
		x = (x + 30)
	}

	UnloadTexturePtr(ship_texture)
	CloseWindow()

	return 0
}
