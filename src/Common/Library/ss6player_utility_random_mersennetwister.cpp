/*
Copyright (c) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura.

This source-list (C# porting -> C++ re-porting and customizing)
                       is coded by Web Technology Corp. 2015 March 18, 2020.


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:



The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_utility_random_mersennetwister.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Utility	{
namespace Random	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */
/* エミッタ用乱数種 */
const Uint32 ListSeedEffect[] = {
	485,	583,	814,	907,	1311,	1901,	2236,	3051,	3676,	4338,
	4671,	4775,	4928,	4960,	5228,	5591,	5755,	5825,	5885,	5967,	6014,	6056,
	6399,	6938,	7553,	8280,	8510,	8641,	8893,	9043,	31043,	31043,
};
const size_t SizeListSeedEffect = sizeof(ListSeedEffect) / sizeof(Uint32);

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
/*                                                            MersenneTwister */
/* -------------------------------------------------------------------------- */
// void MersenneTwister::init_genrand(Uint32 seed)
// Uint32 MersenneTwister::genrand_uint32(void)
// Float32 MersenneTwister::genrand_float32(void)
// Float64 MersenneTwister::genrand_real1(void)
// Float64 MersenneTwister::genrand_real2(void)
// Sint32 MersenneTwister::genrand_N(Sint32 iN)

/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
MersenneTwister::MersenneTwister(void) :
	Generator()
{
	static const Uint64 auInit[] =	{
		0x123UL,
		0x234UL,
		0x345UL,
		0x456UL
	};

	m_iMTI = Constants::iN + 1;	/* mti==N+1 means mt[N] is not initialized */
	init_by_array(auInit, (sizeof(auInit) / sizeof(Uint64)));	/* use default seed */
}
void MersenneTwister::init_by_array(const Uint64 aulInitKey[], size_t iKeyLength)
{
	init_genrand(19650218UL);
	int iI = 1;
	int iJ = 0;
	{
		for(int k=((Constants::iN > iKeyLength) ? Constants::iN : (int)iKeyLength); k>0; k--)	{
			m_aulMT[iI] = ((m_aulMT[iI] ^ ((m_aulMT[iI - 1] ^ (m_aulMT[iI - 1] >> 30)) * 1664525UL)) + aulInitKey[iJ] + (Uint64)iJ);	/* non linear */
			m_aulMT[iI] &= 0xffffffffUL;	/* for WORDSIZE > 32 machines */
			iI++;
			iJ++;
			if(iI >= Constants::iN)	{
				m_aulMT[0] = m_aulMT[Constants::iN - 1];
				iI = 1;
			}
			if(iJ >= iKeyLength)	{
				iJ = 0;
			}
		}
	}
	{
		for(int k=Constants::iN - 1; k>0; k--)	{
			m_aulMT[iI] = (Uint32)((m_aulMT[iI] ^ ((m_aulMT[iI - 1] ^ (m_aulMT[iI - 1] >> 30)) * 1566083941UL)) - (Uint64)iI);	/* non linear */
			m_aulMT[iI] &= 0xffffffffUL;	/* for WORDSIZE > 32 machines */
			iI++;
			if(iI >= Constants::iN)
			{
				m_aulMT[0] = m_aulMT[Constants::iN - 1];
				iI = 1;
			}
		}
	}
	m_aulMT[0] = 0x80000000; /* MSB is 1; assuring non-zero initial array */
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// MersenneTwister::~MersenneTwister(void)

/* ********************************************************* */
//! 乱数種の初期化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void MersenneTwister::InitSeed(Uint32 seed)
{
	if(0xffffffff != seed)	{	/* Seed Set */
		if(seed < SizeListSeedEffect)
		{
			init_genrand(ListSeedEffect[seed]);
		} else {
			init_genrand(seed);
		}
	}
}

/* ********************************************************* */
//! 乱数種の取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
const Uint32* MersenneTwister::ListGetSeed(int* lengthList)
{
	if(nullptr != lengthList)	{
		*lengthList = SizeListSeedEffect;
	}
	return(&ListSeedEffect[0]);
}

/* ********************************************************* */
//! 乱数の取得（Uint32幅）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Uint32 MersenneTwister::RandomUint32(void)
{
	return(genrand_uint32());
}

/* ********************************************************* */
//! 乱数の取得（Float64幅）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Float64 MersenneTwister::RandomDouble(Float64 valueMax)
{
	return((genrand_uint32() * (valueMax / 4294967296.0)));
}

/* ********************************************************* */
//! 乱数の取得（Float32幅）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Float32 MersenneTwister::RandomFloat(Float32 valueMax)
{
	return((Float32)(genrand_uint32() * (valueMax / 4294967296.0f)));
}

/* ********************************************************* */
//! 乱数の取得（指定値まで）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Sint32 MersenneTwister::RandomN(Sint32 valueMax)
{
	return(genrand_N(valueMax));
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* Random */
}	/* Utility */
}	/* Library */
}	/* SpriteStudio6 */
