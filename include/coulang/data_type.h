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

#ifndef COULANG_DATA_TYPE_H
#define COULANG_DATA_TYPE_H

#ifdef __ASSEMBLER__
COULANG_DATA_TYPE_INT=0
COULANG_DATA_TYPE_FLOAT=1
COULANG_DATA_TYPE_LIST=2
COULANG_DATA_TYPE_STR=3
COULANG_DATA_TYPE_PTR=4
#else
enum CoulangDataType {
  COULANG_DATA_TYPE_INT,
  COULANG_DATA_TYPE_FLOAT,
  COULANG_DATA_TYPE_LIST,
  COULANG_DATA_TYPE_STR,
  COULANG_DATA_TYPE_PTR,
};
#endif

#endif // COULANG_DATA_TYPE_H
