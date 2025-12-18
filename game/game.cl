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
	SetTargetFPS int
load "/usr/lib/libc.so.6"

fn main() int {
	InitWindow(700, 700, "Game")

	SetTargetFPS(60)

	while !WindowShouldClose() {
		BeginDrawing()
		ClearBackgroundRGBA(255, 255, 255, 255)
		DrawRectangleRGBA(0, 0, 100, 100, 230, 41, 55, 255)
		EndDrawing()
	}

	CloseWindow()

	return 0
}
