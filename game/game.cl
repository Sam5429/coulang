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
	puts int,
	rand int

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
	UnloadSoundPtr int,
	# int GetMousePositionX()
	GetMousePositionX int,
	#int GetMousePositionY()
	GetMousePositionY int,
	#int IsMouseClicked()
	IsMouseClicked int,
	#float GetFrameTime()
	GetFrameTime float,
	#int IsKeyHold(float time)
	IsKeyHold int

load "libgameglue"
	# Missile* InitMissile(int x, int y, int puissance)
	InitMissile ptr,
	# void UpdatePosition_Missile(Missile* missile)
	UpdatePosition_Missile int,
	# int GetPositionX_Missile(Missile* missile)
	GetPositionX_Missile int,
	# int GetPositionY_Missile(Missile* missile)
	GetPositionY_Missile int,
	# int GetPuissance_Missile(Missile* missile)
	GetPuissance_Missile int, 
	# void DeinitMissile(Missile* missile)
	DeinitMissile int,
	# Asteroid* InitAsteroid(int x, int y)
	InitAsteroid ptr,
	# void UpdatePosition_Asteroid(Asteroid* asteroid, int speed)
	UpdatePosition_Asteroid int,
	# int GetPositionX(Asteroid* asteroid)
	GetPositionX_Asteroid int,
	# int GetPositionY(Asteroid* asteroid)
	GetPositionY_Asteroid int,
	# int GetHP_Asteroid(Asteroid* asteroid)
	GetHP_Asteroid int,
	# void DeinitAsteroid(Asteroid* asteroid)
	DeinitAsteroid int,
	# void TakeHit_Asteroid(Asteroid* asteroid, int degas)
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
val missile_bleu_texture ptr = 0
val missile_rouge_texture ptr = 0

val asteroid_width int = 0
val asteroid_height int = 0
val asteroid_texture_easy ptr = 0
val asteroid_texture_mid ptr = 0
val asteroid_texture_hard ptr = 0
val asteroid_speed int = 1

val health_icon_texture ptr = 0
val health_icon_widht int = 0
val health_icon_height int = 0
val health_icon_x int = 0
val health_icon_y int = 0

val end_texture ptr = 0

val but_restart_texture ptr = 0
val but_restart_width int = 0
val but_restart_heigt int = 0
val but_restart_x int = 200
val but_restart_y int = 400

val missiles_num int = 10
val missiles list = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

val asteroids_num int = 6
val asteroids list = [0, 0, 0, 0, 0, 0]

val piou_aigue_sound ptr = 0
val piou_grave_sound ptr = 0
val porouuuuu_sound ptr = 0
val prout_asteroid_sound ptr = 0

val score int = 0

val numbers_texture list = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

val heart_texture ptr = 0
val heart_texture_width int = 0
val heart_texture_height int = 0

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

fn spawn_missile(positionX int, positionY int, puissance int) int {
	val indx_free int = get_missiles_free_indx()

	if indx_free != (-1) {
		if puissance == 1 {
			PlaySoundPtr(piou_aigue_sound)
		} elif puissance == 3 {
			PlaySoundPtr(piou_grave_sound)
		}
		missiles[indx_free] = InitMissile(positionX, positionY, puissance)
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

# create a line of asteroid
fn generate_asteroid() int {
	val pos_x int = (rand() % (window_width- asteroid_width))
	while pos_x < window_width {
		spawn_asteroid(pos_x, 0)
		pos_x = (pos_x + (asteroid_width-2))
	}
}

fn updatePosition_Asteroid() int {
	val i int = 0
	while i<asteroids_num {
		if asteroids[i] != 0 {
			UpdatePosition_Asteroid(asteroids[i], asteroid_speed)
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
				if rand() % 100 < 30 {
					spawn_healt_icon(GetPositionX_Asteroid(asteroids[i]), GetPositionY_Asteroid(asteroids[i]))
				}
				DeinitAsteroid(asteroids[i])
				asteroids[i] = 0
			}
		}
		i = (i+1)
	}
}

fn spawn_healt_icon(positionX int, positionY int) int{
	if health_icon_x == 0 {
		if health_icon_y == 0 {
			health_icon_x = positionX
			health_icon_y = positionY
		}
	}
}

fn updatePosition_Items() int {
	if health_icon_x != 0 {
		if health_icon_y != 0 {
			health_icon_y = (health_icon_y + 1)
		}
	}
}

fn colliding(x1 int, y1 int, h1 int, w1 int, x2 int, y2 int, h2 int, w2 int) int {
	if (x1 < (x2 + w2)) {
		if (x2 < (x1 + w1)) {
			if (y1 < (y2 + h2)) {
				if (y2 < (y1 + h1)) {
					return 1
				}
			}
		}
	}
	return 0
}

fn tcheck_colision() int {
	val i int = 0
	while i<asteroids_num {
		if asteroids[i] != 0 {
			val asteroidPosX int = GetPositionX_Asteroid(asteroids[i])
			val asteroidPosY int = GetPositionY_Asteroid(asteroids[i])
			# colision avec le vaisseau
			if colliding(asteroidPosX, asteroidPosY, asteroid_height, asteroid_width, ship_x, ship_y, ship_height, ship_width) {
				PlaySoundPtr(prout_asteroid_sound)
				DeinitAsteroid(asteroids[i])
				asteroids[i] = 0
				ship_hp = (ship_hp - 1)

			}

			# colision avec les missiles
			val j int = 0
			while j<missiles_num {
				if missiles[j] != 0 {
					val missilePosX int = GetPositionX_Missile(missiles[j])
					val missilePosY int = GetPositionY_Missile(missiles[j])
					if colliding(missilePosX, missilePosY, missile_height, missile_width, asteroidPosX, asteroidPosY, asteroid_height, asteroid_width) {
						PlaySoundPtr(porouuuuu_sound)
						TakeHit_Asteroid(asteroids[i], GetPuissance_Missile(missiles[j]))
						DeinitMissile(missiles[j])
						missiles[j] = 0
						score = (score + 10)

                        val new_asteroid_speed int = score / 100

                        if new_asteroid_speed < 1 {
                            new_asteroid_speed = 1
                        } elif new_asteroid_speed > 10 {
                            new_asteroid_speed = 10
                        }

                        asteroid_speed = new_asteroid_speed
					}
				}
				j = (j+1)
			}
		}
		i = (i+1)
	}

	# colision with the items
	if colliding(health_icon_x, health_icon_y, health_icon_height, health_icon_widht, ship_x, ship_y, ship_height, ship_width) {
		if ship_hp < 3 {
			ship_hp = (ship_hp + 1)
		}
		health_icon_x = 0
		health_icon_y = 0
	} elif health_icon_y > window_height {
		health_icon_y = 0
		health_icon_x = 0
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
			if GetPuissance_Missile(missiles[i]) == 3 {
				DrawTexturePtr(missile_bleu_texture, missile_x, missile_y, 255, 255, 255, 255)
			} else {
				DrawTexturePtr(missile_rouge_texture, missile_x, missile_y, 255, 255, 255, 255)
			}
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

fn drawScore() int {
	val score_count int = score
	val number_count int = 0

	while score_count >= 0 {
		val current_digit int = score_count % 10
		val texture_ptr ptr = numbers_texture[current_digit]
		val posX int = window_width - ((number_count + 1) * TextureWidth(texture_ptr)) - 10
		val posY int = 10

		DrawTexturePtr(texture_ptr, posX, posY, 255, 255, 255, 255)

		score_count = (score_count / 10)
		number_count = (number_count + 1)

		if score_count == 0 {
			return 0
		}
	}

	return 0
}

fn drawHeart() int {
    val count int = 0
    val posX int = 0
    val posY int = 10

    while count < ship_hp {
        DrawTexturePtr(heart_texture, posX, posY, 255, 255, 255, 255)

        count = (count + 1)
        posX = (posX + heart_texture_width)
    }

    return 0
}

fn drawHealthIcon() int {
	if health_icon_x != 0 {
		if health_icon_y != 0 {
			DrawTexturePtr(health_icon_texture, health_icon_x, health_icon_y, 255, 255, 255, 255)
		}
	}
}

fn drawEnd() int {
	BeginDrawing()
		ClearBackgroundRGBA(0, 0, 0, 255)
		DrawTexturePtr(end_texture, 0, 0, 255, 255, 255, 255)
		DrawTexturePtr(but_restart_texture, but_restart_x, but_restart_y, 255, 255, 255)
		drawScore()
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
		spawn_missile((ship_x + (ship_width/2)), ship_y - 20, 1)
	}

	if IsKeyPressed2(KEY_UP) {
		spawn_missile((ship_x + (ship_width/2)), ship_y - 20, 3)
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
	updatePosition_Items()

	generate_asteroid()

	tcheck_colision()

	del_missiles()
	del_asteroid()

	BeginDrawing()
		drawShip()
		drawMissile()
		drawAsteroid()
		drawScore()
        drawHeart()
		drawHealthIcon()
	EndDrawing()

	return 0
}

fn resetupVar() int {
	val i int = 0
	while i<asteroids_num {
		if asteroids[i] != 0 {
			DeinitAsteroid(asteroids[i])
			asteroids[i] = 0
		}
		i = (i+1)
	}

	i = 0
	while i<missiles_num {
		if missiles[i] != 0 {
			DeinitMissile(missiles[i])
			missiles[i] = 0
		}
		i = (i+1)
	}

	ship_hp = 3
	ship_x = 0
	ship_y = (window_height - ship_height)

	score = 0
    asteroid_speed = 1
}


fn main() int {
	InitWindow(window_width, window_height, "Game")
	InitAudioDevice()

	SetTargetFPS(60)

	ship_texture = LoadTexturePtr("game/asset/vaisseau.png")
	ship_width = TextureWidth(ship_texture)
	ship_height = TextureHeight(ship_texture)#
	ship_y = (window_height - ship_height)

	missile_rouge_texture = LoadTexturePtr("game/asset/missile_rouge.png")
	missile_bleu_texture = LoadTexturePtr("game/asset/missile_bleu.png")
	missile_width = TextureWidth(missile_rouge_texture)
	missile_height = TextureHeight(missile_rouge_texture)

	asteroid_texture_easy = LoadTexturePtr("game/asset/asteroid_easy.png")
	asteroid_width = TextureWidth(asteroid_texture_easy)
	asteroid_height = TextureHeight(asteroid_texture_easy)

	asteroid_texture_mid = LoadTexturePtr("game/asset/asteroid_medium.png")
	asteroid_texture_hard = LoadTexturePtr("game/asset/asteroid_hard.png")

	health_icon_texture = LoadTexturePtr("game/asset/health.png")
	health_icon_widht = TextureWidth(health_icon_texture)
	health_icon_height = TextureHeight(health_icon_texture)

	end_texture = LoadTexturePtr("game/asset/fin.png")

	but_restart_texture = LoadTexturePtr("game/asset/restart.png")
	but_restart_width = TextureWidth(but_restart_texture)
	but_restart_heigt = TextureHeight(but_restart_texture)


	piou_aigue_sound = LoadSoundPtr("game/asset/piou_aigue_1.mp3")
	piou_grave_sound = LoadSoundPtr("game/asset/piou_grave.mp3")
	porouuuuu_sound = LoadSoundPtr("game/asset/porouuuuu_1.mp3")
	prout_asteroid_sound = LoadSoundPtr("game/asset/prout_asteroid.mp3")
	numbers_texture[0] = LoadTexturePtr("game/asset/numbers/0.png")
	numbers_texture[1] = LoadTexturePtr("game/asset/numbers/1.png")
	numbers_texture[2] = LoadTexturePtr("game/asset/numbers/2.png")
	numbers_texture[3] = LoadTexturePtr("game/asset/numbers/3.png")
	numbers_texture[4] = LoadTexturePtr("game/asset/numbers/4.png")
	numbers_texture[5] = LoadTexturePtr("game/asset/numbers/5.png")
	numbers_texture[6] = LoadTexturePtr("game/asset/numbers/6.png")
	numbers_texture[7] = LoadTexturePtr("game/asset/numbers/7.png")
				
	numbers_texture[8] = LoadTexturePtr("game/asset/numbers/8.png")
	numbers_texture[9] = LoadTexturePtr("game/asset/numbers/9.png")

    heart_texture = LoadTexturePtr("game/asset/heart.png")
    heart_texture_width = TextureWidth(heart_texture)
    heart_texture_height = TextureHeight(heart_texture)

	val isGameOver int = 0

	while !WindowShouldClose() {
		if isGameOver {
			drawEnd()
			val mousePosX int = GetMousePositionX()
			val mousePosY int = GetMousePositionY()
			if IsMouseClicked() {
				if colliding(mousePosX, mousePosY, 1, 1, but_restart_x, but_restart_y, but_restart_heigt, but_restart_width) {
					printf("lets do it again")
					resetupVar()
					isGameOver = 0
				}
			}
		} else {
			isGameOver = gameTurn()
		}
	}

	UnloadTexturePtr(ship_texture)
	UnloadTexturePtr(missile_rouge_texture)
	UnloadTexturePtr(missile_bleu_texture)
	UnloadTexturePtr(asteroid_texture_easy)
	UnloadTexturePtr(asteroid_texture_mid)
	UnloadTexturePtr(asteroid_texture_hard)
	UnloadSoundPtr(piou_aigue_sound)
	UnloadSoundPtr(piou_grave_sound)
	UnloadSoundPtr(porouuuuu_sound)
	UnloadSoundPtr(prout_asteroid_sound)
	UnloadTexturePtr(numbers_texture[0])
	UnloadTexturePtr(numbers_texture[1])
	UnloadTexturePtr(numbers_texture[2])
	UnloadTexturePtr(numbers_texture[3])
	UnloadTexturePtr(numbers_texture[4])
	UnloadTexturePtr(numbers_texture[5])
	UnloadTexturePtr(numbers_texture[6])
	UnloadTexturePtr(numbers_texture[7])
	UnloadTexturePtr(numbers_texture[8])
	UnloadTexturePtr(numbers_texture[9])
    UnloadTexturePtr(heart_texture)
	CloseWindow()

	return 0
}
