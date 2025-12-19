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

load "build/librayglue.so"
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
	IsKeyPressed2 int

val KEY_RIGHT int = 262
val KEY_LEFT int = 263
val KEY_DOWN int = 264
val KEY_UP int = 265

val window_width int = 700
val window_height int = 700
val plane_width int = 100
val plane_height int = 100
val plane_x int = 0
val plane_y int = window_height - plane_height

fn draw() int {
	ClearBackgroundRGBA(0, 0, 0, 255)
	DrawRectangleRGBA(plane_x, plane_y, plane_width, plane_height, 230, 41, 55, 255)

	return 0
}

fn handle_events() int {
    if IsKeyPressed2(KEY_RIGHT) {
		if plane_x < (window_width - plane_width) {
			plane_x = (plane_x + 10)
		} else {
			plane_x = (window_width - plane_width)
		}
    }

    if IsKeyPressed2(KEY_LEFT) {
		if plane_x > 0 {
			plane_x = (plane_x - 10)
		} else {
			plane_x = 0
		}
    }

	return 0
}

fn main() int {
	InitWindow(window_width, window_height, "Game")

	SetTargetFPS(60)

	while !WindowShouldClose() {
		handle_events()

		BeginDrawing()
			draw()
		EndDrawing()
	}

	CloseWindow()

	return 0
}
