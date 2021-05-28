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
/* ========================================================================== */
/* MEMO:                                                                      */
/* Mersenne twister乱数の実装部です。                                         */
/* Utility::Random::Generatorの継承クラスです。                               */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_utility.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Utility	{
namespace Random	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* Mersenne-Twister乱数発生器 */
class MersenneTwister :
	public Generator
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
	enum Constants	{
		iN = 624UL,						/* int */
		iM = 397UL,						/* int */
		ulMATRIX_A = 0x9908b0dfUL,
		ulUPPER_MASK = 0x80000000UL,
		ulLOWER_MASK = 0x7fffffffUL,
	};

public:

	/* ----------------------------------------------- Variables */
private:
protected:
	Uint64 m_aulMT[Constants::iN];
	Sint32 m_iMTI;

public:

	/* ----------------------------------------------- Functions */
private:
protected:
	void init_by_array(const Uint64 aulInitKey[], size_t iKeyLength);

	inline void init_genrand(Uint32 seed)
	{
		m_aulMT[0] = (Uint64)seed & 0xffffffffUL;
		{
			for(m_iMTI=1; m_iMTI<Constants::iN; m_iMTI++)	{
				m_aulMT[m_iMTI] = (Uint32)((1812433253UL * (m_aulMT[m_iMTI - 1] ^ (m_aulMT[m_iMTI - 1] >> 30)) + (Uint64)m_iMTI));
				/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
				/* In the previous versions, MSBs of the seed affect   */
				/* only MSBs of the array mt[].                        */
				/* 2002/01/09 modified by Makoto Matsumoto             */
				m_aulMT[m_iMTI] &= 0xffffffffUL;	/* for >32 bit machines */
			}
		}
	}

	inline Uint32 genrand_uint32(void)
	{
		static const Uint64 aulMag01[] =	{
			0,
			(Uint64)Constants::ulMATRIX_A
		};
		Uint64 ulY = 0;

		/* mag01[x] = x * MATRIX_A for x=0,1 */
		if(m_iMTI >= (Sint32)Constants::iN)	{
			/* generate N words at one time */
			if (m_iMTI == (Constants::iN + 1))	{
				/* if init_genrand() has not been called, */
				init_genrand(5489UL);	/* a default initial seed is used */
			}

			int iKK = 0;
			{
				for(; iKK<(Constants::iN - Constants::iM); iKK++)	{
					ulY = (m_aulMT[iKK] & (Uint64)Constants::ulUPPER_MASK) | (m_aulMT[iKK + 1] & (Uint64)Constants::ulLOWER_MASK);
					m_aulMT[iKK] = m_aulMT[iKK + Constants::iM] ^ (ulY >> 1) ^ aulMag01[ulY & 1];
				}
			}
			{
				for (; iKK <(Constants::iN - 1); iKK++)
				{
					ulY = (m_aulMT[iKK] & (Uint64)Constants::ulUPPER_MASK) | (m_aulMT[iKK + 1] & (Uint64)Constants::ulLOWER_MASK);
					m_aulMT[iKK] = m_aulMT[iKK + (Constants::iM - Constants::iN)] ^ (ulY >> 1) ^ aulMag01[ulY & 0x1UL];
				}
			}

			ulY = (m_aulMT[Constants::iN - 1] & (Uint64)Constants::ulUPPER_MASK) | (m_aulMT[0] & (Uint64)Constants::ulLOWER_MASK);
			m_aulMT[Constants::iN - 1] = m_aulMT[Constants::iM - 1] ^ (ulY >> 1) ^ aulMag01[ulY & 1];
			m_iMTI = 0;
		}

		ulY = m_aulMT[m_iMTI++];
		/* Tempering */
		ulY ^= (ulY >> 11);
		ulY ^= (ulY << 7) & 0x9d2c5680UL;
		ulY ^= (ulY << 15) & 0xefc60000UL;
		ulY ^= (ulY >> 18);
		return((Uint32)(ulY & 0xffffffffUL));
	}

	inline Float32 genrand_float32(void)
	{
		return((Float32)((genrand_uint32() >> 9) & 0x007fffff) * (1.0f / 8388607.0f));
	}

	/* generates a random floating point number on [0,1] */
	inline Float64 genrand_real1(void)
	{
		return(genrand_uint32() * (1.0 / 4294967295.0));	/* divided by 2^32-1 */
	}

	/* generates a random floating point number on [0,1] */
	inline Float64 genrand_real2(void)
	{
		return(genrand_uint32() * (1.0 / 4294967296.0));	/* divided by 2^32 */
	}

	/* generates a random integer number from 0 to N-1 */
	inline Sint32 genrand_N(Sint32 iN)
	{
		return((Sint32)(genrand_uint32() * (iN / 4294967296.0)));
	}

public:
	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	MersenneTwister(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	virtual ~MersenneTwister(void)
	{
	}

	/* ********************************************************* */
	//! 乱数種の初期化
	/*!
	Utility::Random::Generatorの実装です。
	*/
	virtual void InitSeed(Uint32 seed);

	/* ********************************************************* */
	//! 乱数種の取得
	/*!
	Utility::Random::Generatorの実装です。
	*/
	virtual const Uint32* ListGetSeed(int* lengthList);

	/* ********************************************************* */
	//! 乱数の取得（Uint32幅）
	/*!
	Utility::Random::Generatorの実装です。
	*/
	virtual Uint32 RandomUint32(void);

	/* ********************************************************* */
	//! 乱数の取得（Float64幅）
	/*!
	Utility::Random::Generatorの実装です。
	*/
	virtual Float64 RandomDouble(Float64 valueMax=1.0);

	/* ********************************************************* */
	//! 乱数の取得（Float32幅）
	/*!
	Utility::Random::Generatorの実装です。
	*/
	virtual Float32 RandomFloat(Float32 valueMax=1.0f);

	/* ********************************************************* */
	//! 乱数の取得（指定値まで）
	/*!
	Utility::Random::Generatorの実装です。
	*/
	virtual Sint32 RandomN(Sint32 valueMax);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */
}	/* Random */
}	/* Utility */
}	/* Library */
}	/* SpriteStudio6 */

