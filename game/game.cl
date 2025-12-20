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
	# int TextureWidth(Texture2D *texture)prout_asteroid.mp3
	TextureWidth int,
	# int TextureHeight(Texture2D *texture)
	TextureHeight int,
	# void InitAudioDevice(void);
	InitAudioDevice int,
	# Sound *LoadSoundPtr(const char *fileName);
	LoadSoundPtr ptr,
	# void PlaySoundPtr(Sound *sound);
	PlaySoundPtr int,
	# void UnloadSoundPtr(Sound *sound);
	UnloadSoundPtr int

load "libgameglue"
	# Missile* InitMissile(int x, int y)
	InitMissile ptr,
	# void UpdatePosition_Missile(Missile* missile)
	UpdatePosition_Missile int,
	# int GetPositionX_Missile(Missile* missile)
	GetPositionX_Missile int,
	# int GetPositionY_Missile(Missile* missile)
	GetPositionY_Missile int,
	# void DeinitMissile(Missile* missile)
	DeinitMissile int,
	# Asteroid* InitAsteroid(int x, int y)
	InitAsteroid ptr,
	# void UpdatePosition_Asteroid(Asteroid* asteroid)
	UpdatePosition_Asteroid int,
	# int GetPositionX(Asteroid* asteroid)
	GetPositionX_Asteroid int,
	# int GetPositionY(Asteroid* asteroid)
	GetPositionY_Asteroid int,
	# int GetHP_Asteroid(Asteroid* asteroid)
	GetHP_Asteroid int,
	# void DeinitAsteroid(Asteroid* asteroid)
	DeinitAsteroid int,
	# void TakeHit_Asteroid(Asteroid* asteroid)
	TakeHit_Asteroid int

val KEY_RIGHT int = 262
val KEY_LEFT int = 263
val KEY_DOWN int = 264
val KEY_UP int = 265
val KEY_SPACE int = 32

val window_width int = 700
val window_height int = 900
val ship_width int = 0
val ship_height int = 0
val ship_texture ptr = 0
val ship_x int = 0
val ship_y int = 0
val ship_hp int = 3

val missile_width int = 0
val missile_height int = 0
val missile_texture ptr = 0

val asteroid_width int = 0
val asteroid_height int = 0
val asteroid_texture_easy ptr = 0
val asteroid_texture_mid ptr = 0
val asteroid_texture_hard ptr = 0

val end_texture ptr = 0

val missiles_num int = 10
val missiles list = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

val asteroids_num int = 6
val asteroids list = [0, 0, 0, 0, 0, 0]

val piou_aigue_sound ptr = 0
val piou_grave_sound ptr = 0
val porouuuuu_sound ptr = 0
val prout_asteroid_sound ptr = 0

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

fn updatePosition_Missile() int {
	val i int = 0
	while i<missiles_num {
		if missiles[i] != 0 {
			UpdatePosition_Missile(missiles[i])
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
			if GetPositionY_Missile(missiles[i]) <= 0 {
				printf("je sup %d\n", i)
				DeinitMissile(missiles[i])
				missiles[i] = 0
			}
		}
		i = (i+1)
	}
}

fn get_asteroid_free_indx() int {
	val i int = 0
	while i < asteroids_num {
		if asteroids[i] == 0 {
			return i
		}
		i = (i+1)
	}
	return -1
}

fn spawn_asteroid(positionX int, positionY int) int {
	val indx_free int = get_asteroid_free_indx()

	if indx_free != (-1) {
		asteroids[indx_free] = InitAsteroid(positionX, positionY, 3)
		return 0
	}
	return -1
}

fn has_asteroid() int {
	val i int = 0
	while i<asteroids_num {
		if asteroids[i] != 0 {
			return 1
		}
		i = (i+1)	
	}
	return 0
}

# create a line of asteroid
fn generate_asteroid() int {
	if has_asteroid() {
		return 0
	}
	val pos_x int = 0
	while pos_x < window_width {
		spawn_asteroid(pos_x, 0)
		pos_x = (pos_x + (asteroid_width-2))
	}
}

fn updatePosition_Asteroid() int {
	val i int = 0
	while i<asteroids_num {
		if asteroids[i] != 0 {
			UpdatePosition_Asteroid(asteroids[i])
		}
		i = (i+1)
	}
	return 0
}

fn del_asteroid() int {
	val i int = 0
	while i<asteroids_num {
		# if there out of the screen
		if asteroids[i] != 0 {
			if GetPositionY_Asteroid(asteroids[i]) > window_height {
				DeinitAsteroid(asteroids[i])
				asteroids[i] = 0
			}
			elif GetHP_Asteroid(asteroids[i]) <= 0 {
				DeinitAsteroid(asteroids[i])
				asteroids[i] = 0
			}
		}
		i = (i+1)
	}
}

fn tcheck_colision() int {
	val i int = 0
	while i<asteroids_num {
		if asteroids[i] != 0 {
			val asteroidPosX int = GetPositionX_Asteroid(asteroids[i])
			val asteroidPosY int = GetPositionY_Asteroid(asteroids[i])
			# colision avec le vaisseau
			if (asteroidPosX < (ship_x + ship_width)) {
				if (ship_x < (asteroidPosX + asteroid_width)) {
					if (asteroidPosY < (ship_y + ship_height)) {
						if (ship_y < (asteroidPosY + asteroid_height)) {
							PlaySoundPtr(prout_asteroid_sound)
							DeinitAsteroid(asteroids[i])
							asteroids[i] = 0
							ship_hp = (ship_hp - 1)
						}
					}
				}
			}

			# colision avec les missiles
			val j int = 0
			while j<missiles_num {
				if missiles[j] != 0 {
					val missilePosX int = GetPositionX_Missile(missiles[j])
					val missilePosY int = GetPositionY_Missile(missiles[j])
					if (asteroidPosX < (missilePosX + missile_width)) {
						if (missilePosX < (asteroidPosX + asteroid_width)) {
							if (asteroidPosY < (missilePosY + missile_height)) {
								if (missilePosY < (asteroidPosY + asteroid_height)) {
									PlaySoundPtr(porouuuuu_sound)
									TakeHit_Asteroid(asteroids[i])
									DeinitMissile(missiles[j])
									missiles[j] = 0
								}
							}
						}
					}
				}
				j = (j+1)
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
			val missile_x int = GetPositionX_Missile(missiles[i])
			val missile_y int = GetPositionY_Missile(missiles[i])
			DrawTexturePtr(missile_texture, missile_x, missile_y, 255, 255, 255, 255)
		}
		i = (i + 1)
	}
	return 0
}

fn drawAsteroid() int {
	ClearBackgroundRGBA(0, 0, 0, 255)
	val i int = 0
	while i < asteroids_num {
		if asteroids[i] != 0 {
			val asteroid_x int = GetPositionX_Asteroid(asteroids[i])
			val asteroid_y int = GetPositionY_Asteroid(asteroids[i])
			val hp int = GetHP_Asteroid(asteroids[i])
			if(hp==3) {
				DrawTexturePtr(asteroid_texture_hard, asteroid_x, asteroid_y, 255, 255, 255, 255)
			} elif(hp==2) {
				DrawTexturePtr(asteroid_texture_mid, asteroid_x, asteroid_y, 255, 255, 255, 255)
			} elif(hp==1) {
				DrawTexturePtr(asteroid_texture_easy, asteroid_x, asteroid_y, 255, 255, 255, 255)
			}
			#DrawTexturePtr(asteroid_texture_hard, asteroid_x, asteroid_y, 255, 255, 255, 255)
		}
		i = (i + 1)
	}
	return 0
}

fn drawEnd() int {
	BeginDrawing()
		ClearBackgroundRGBA(0, 0, 0, 255)
		DrawTexturePtr(end_texture, 0, 0, 255, 255, 255, 255)
	EndDrawing()

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

    if IsKeyPressed2(KEY_SPACE) {
		spawn_missile((ship_x + (ship_width/2)), ship_y - 20)
		PlaySoundPtr(piou_aigue_sound)
	}

	return 0
}

fn gameTurn() int {
	if ship_hp <= 0 {
		return 1
	}

	handle_events()
	updatePosition_Missile()
	updatePosition_Asteroid()

	generate_asteroid()

	tcheck_colision()

	del_missiles()
	del_asteroid()

	BeginDrawing()
		drawShip()
		drawMissile()
		drawAsteroid()
	EndDrawing()

	return 0
}


fn main() int {
	InitWindow(window_width, window_height, "Game")
	InitAudioDevice()

	SetTargetFPS(60)

	ship_texture = LoadTexturePtr("game/asset/vaisseau.png")
	ship_width = TextureWidth(ship_texture)
	ship_height = TextureHeight(ship_texture)
	ship_y = (window_height - ship_height)

	missile_texture = LoadTexturePtr("game/asset/misil1.png")
	missile_width = TextureWidth(missile_texture)
	missile_height = TextureHeight(missile_texture)

	asteroid_texture_easy = LoadTexturePtr("game/asset/asteroid_easy.png")
	asteroid_width = TextureWidth(asteroid_texture_easy)
	asteroid_height = TextureHeight(asteroid_texture_easy)

	asteroid_texture_mid = LoadTexturePtr("game/asset/asteroid_medium.png")
	asteroid_texture_hard = LoadTexturePtr("game/asset/asteroid_hard.png")
	end_texture = LoadTexturePtr("game/asset/fin.png")

	piou_aigue_sound = LoadSoundPtr("game/asset/piou_aigue_1.mp3")
	piou_grave_sound = LoadSoundPtr("game/asset/piou_grave.mp3")
	porouuuuu_sound = LoadSoundPtr("game/asset/porouuuuu_1.mp3")
	prout_asteroid_sound = LoadSoundPtr("game/asset/prout_asteroid.mp3")

	val isGameOver int = 0

	while !WindowShouldClose() {

		if isGameOver {
			drawEnd()
		} else {
			isGameOver = gameTurn()
		}
	}

	UnloadTexturePtr(ship_texture)
	UnloadTexturePtr(missile_texture)
	UnloadTexturePtr(asteroid_texture_easy)
	UnloadTexturePtr(asteroid_texture_mid)
	UnloadTexturePtr(asteroid_texture_hard)
	UnloadSoundPtr(piou_aigue_sound)
	UnloadSoundPtr(piou_grave_sound)
	UnloadSoundPtr(porouuuuu_sound)
	UnloadSoundPtr(prout_asteroid_sound)
	CloseWindow()

	return 0
}
