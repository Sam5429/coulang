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

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void ClearBackgroundRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Color color = { .r = r, .g = g, .b = b, .a = a };

	ClearBackground(color);
}

void DrawRectangleRGBA(int posX, int posY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Color color = { .r = r, .g = g, .b = b, .a = a };

	DrawRectangle(posX, posY, width, height, color);
}

// NOTE: For some reason IsKeyDown return garbage,
// so we will define IsKeyPressed2, to reproduce
// the behaviour of this function.
bool IsKeyPressed2(int key)
{
	return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

Texture2D *LoadTexturePtr(const char *fileName)
{
	Texture2D *texture_ptr = COULANG_ALLOC(sizeof(Texture2D));

	*texture_ptr = LoadTexture(fileName);

	return texture_ptr;
}

void UnloadTexturePtr(Texture2D *texture)
{
	UnloadTexture(*texture);
	free(texture);
}

void DrawTexturePtr(Texture2D *texture, int posX, int posY, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Color color = { .r = r, .g = g, .b = b, .a = a };

	DrawTexture(*texture, posX, posY, color);
}

int TextureWidth(Texture2D *texture) {
	return texture->width;
}

int TextureHeight(Texture2D *texture) {
	return texture->height;
}
