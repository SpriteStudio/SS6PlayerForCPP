/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* Xor-Shift乱数の実装部です。                                                */
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
/* XorShift32乱数発生器 */
class XorShift32 :
	public Generator
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:

	/* ----------------------------------------------- Variables */
private:
protected:
	Uint32 SeedNow;

public:

	/* ----------------------------------------------- Functions */
private:
protected:
	inline void init_genrand(Uint32 seed)
	{
		SeedNow = seed;
	}

	inline Uint32 genrand_uint32(void)
	{
		SeedNow = SeedNow ^ (SeedNow << 13);
		SeedNow = SeedNow ^ (SeedNow >> 17);
		SeedNow = SeedNow ^ (SeedNow << 15);

		return(SeedNow);
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
	XorShift32(void);

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	virtual ~XorShift32(void)
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

