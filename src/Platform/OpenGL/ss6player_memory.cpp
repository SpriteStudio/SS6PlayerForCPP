/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL(GLFW+GLEW)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_memory.h"

#include <stdio.h>
#include <cstdlib>

namespace SpriteStudio6	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                     Memory */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! ヒープの取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void* Memory::Allocate(void** area, size_t size, size_t alignment)
{
	size_t sizeBuffer = size;
	if(0 != alignment)	{	/* アライメント指定がある */
		sizeBuffer += alignment;	/* アライメント分の余分を取る（本当はalignemnt-1で良い） */
	}

	void* heapBuffer = malloc(sizeBuffer);
	char* heapBufferChar = reinterpret_cast<char*>(heapBuffer);
	if(nullptr == heapBufferChar)	{	/* メモリが取得できなかった */
		*area = nullptr;

		return(nullptr);
	}

	/* バッファ内のアライメントを取る */
	uintptr_t heapTop = (uintptr_t)heapBufferChar;
	uintptr_t dataTop = heapTop;
	if(0 != alignment)	{	/* アライメント指定がある */
		/* MEMO: 一番近いアライメントに適合した位置を取得 */
		dataTop = ((heapTop + (alignment - 1)) / alignment) * alignment;	/* 一番近いアライメントに適合した位置を取得 */
	}
	size_t indexHeap = dataTop - heapTop;	/* インデックスを取得 */

	*area = heapBuffer;

	return(reinterpret_cast<void*>(&heapBufferChar[indexHeap]));
}

/* ********************************************************* */
//! ヒープの解放
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Memory::Free(void* area)
{
	if(nullptr != area)	{	/* ポインタが有効 */
		free(area);
	}
}

/* ********************************************************* */
//! メモリ（サイズ・インデックス）をアライメント
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// size_t Memory::Alignment(size_t index, size_t alignment)


/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
