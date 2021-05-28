/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* エフェクト再生の根底制御関係の処理群です。                                 */
/* 厳密にはcontrol.hの中に書かれるべき定義群ではありますが、コードの見通しの  */
/* 点から切り離してあります。                                                 */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>

#include "ss6player_data.h"
#include "ss6player_pack.h"
#include "ss6player_data_project.h"
#include "ss6player_control.h"
#include "ss6player_utility.h"

/* 前方宣言 */
namespace SpriteStudio6	{
class EntityEffect;
class Project;
}	/* SpriteStudio6 */

namespace SpriteStudio6	{
namespace Library	{
namespace Control	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* エフェクト再生管理 */
class Effect	{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* 前方宣言 */
	class Emitter;

	/* パーティクル再生管理 */
	class Particle	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		/* パーティクルの生存情報 */
		class Activity	{
			/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
		private:
		protected:
		public:

			/* ----------------------------------------------- Enums & Constants */
		private:
		protected:
		public:
			enum FlagBitStatus	{
				EXIST = 0x40000000,	/* RUNNING */
				BORN = 	0x20000000, /* GETUP */

				CLEAR = 0x00000000,
			};

			/* ----------------------------------------------- Variables */
		private:
		protected:
		public:
			Uint32 /* FlagBitStatus */ Status;
			int Cycle;
			int FrameStart;
			int FrameEnd;

			/* ----------------------------------------------- Functions */
		private:
			void UpdateMain(	int frame,
								Library::Control::Effect::Emitter& emitter,
								const Library::Data::Effect::Emitter& dataEmitter,
								int patternOffset,
								int durationTarget,
								int cycleTarget
						);

		protected:
		public:
			/* ********************************************************* */
			//! 未使用化
			/*!
			@param	なし

			@retval	なし

			本クラスの内容を未使用状態にします。
			*/
			inline void CleanUp(void)
			{
				Status = FlagBitStatus::CLEAR;
				Cycle = -1;
				FrameStart = -1;
				FrameEnd = -1;
			}

			/* ********************************************************* */
			//! コンストラクタ
			/*!
			@param	なし

			@retval	なし
			*/
			Activity(void)
			{
				CleanUp();
			}

			/* ********************************************************* */
			//! デストラクタ
			/*!
			@param	なし

			@retval	なし

			※このクラスは継承したりしないので、仮想化していません。
			*/
			~Activity(void)
			{
//				CleanUp();
			}

			/* ********************************************************* */
			//! 更新（静的データ用）
			/*!
			@param	frame
				フレーム位置
			@param	emitter
				所属エミッタ
			@param	dataEmitter
				所属エミッタの静的データ情報
			@param	patternEmit
				エミットパターン情報のインデックス
			@param	patternOffset
				パターンオフセット
			@param	patternEmitTarget
				ターゲットとするエミットパターン情報のインデックス

			@retval	なし

			本クラスの内容を指定フレーム数の状態に更新します。
			本関数はエミットパターンが静的データとして事前計算されている時
				（乱数種がデータで指定されている場合）用の関数です。
			*/
			void Update(	int frame,
							Library::Control::Effect::Emitter& emitter,
							const Library::Data::Effect::Emitter& dataEmitter,
							int indexPatternEmit,
							int patternOffset,
							int indexPatternEmitTarget
						);

			/* ********************************************************* */
			//! 更新（動的ワーク用）
			/*!
			@param	frame
				フレーム位置
			@param	emitter
				所属エミッタ
			@param	dataEmitter
				所属エミッタの静的データ情報
			@param	patternEmit
				エミットパターン情報
			@param	patternOffset
				パターンオフセット
			@param	patternEmitTarget
				ターゲットとするエミットパターン

			@retval	なし

			本クラスの内容を指定フレーム数の状態に更新します。
			本関数はエミットパターンが動的に確保されている時（乱数種が
				データで指定されていない場合）用の関数です。
			*/
			void Update(	int frame,
							Library::Control::Effect::Emitter& emitter,
							const Library::Data::Effect::Emitter& dataEmitter,
							const Library::Data::Effect::Emitter::PatternEmit& patternEmit,
							int patternOffset,
							const Library::Data::Effect::Emitter::PatternEmit& patternEmitTarget
						);

			/* ----------------------------------------------- Operators */

			/* ----------------------------------------------- Friends */
		};

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		int ID;
		int IDParent;
		int FrameStart;
		int FrameEnd;

		float PositionX;
		float PositionY;
		float RotateZ;
		float Direction;

		/* 計算領域のキャッシュ */
		/* MEMO: C#の時は確保してある方が有利だったが、C++の場合はスタック配置でも問題ない気がする。 */
		/*       実装を再チェックすること。                                                          */
		/* MEMO: ColorVertexはFPUにした方が時間が稼げるか？（計算用テンポラリなので、FPUの方が良さげ？） */
		/*       ※ベクトル同士の加減乗除とスカラとの乗算がどのくらいの個数あるか数えること。            */
		CPU::Color ColorVertex;
		alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Vector3 Scale;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! 未使用化
		/*!
		@param	なし

		@retval	なし

		本クラスの内容を未使用状態にします。
		*/
		inline void CleanUp(void)
		{
			ID = -1;
			IDParent = -1;
			FrameStart = -1;
			FrameEnd = -1;

//			PositionX = 
//			PositionY = 
//			RotateZ = 
//			Direction = 

//			ColorVertex = 
//			Scale = 
		}

		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		Particle(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし

		※このクラスは継承したりしないので、仮想化していません。
		*/
		~Particle(void)
		{
//			CleanUp();
		}

		/* ********************************************************* */
		//! 複製
		/*!
		@param	original
			複製元の本クラス

		@retval	なし

		複製元の内容を本クラスにディープコピーします。
		*/
		inline void Duplicate(Particle&original)
		{
			ID = original.ID;
			IDParent = original.IDParent;
			FrameStart = original.FrameStart;
			FrameEnd = original.FrameEnd;

			PositionX = original.PositionX;
			PositionY = original.PositionY;
			RotateZ = original.RotateZ;
			Direction = original.Direction;

			ColorVertex.Set(original.ColorVertex);
			Scale.Set(original.Scale);
		}

		/* ********************************************************* */
		//! 更新
		/*!
		@param	frame
			フレーム位置
		@param	index
			パーティクルのインデックス
		@param	entity
			エフェクトのエンティティ
		@param	controlEffect
			エフェクトの再生制御
		@param	emitter
			所属エミッタ
		@param	activity
			生存情報
		@param	indexEmitterParent
			親エミッタのインデックス
		@param	particleParent
			親のパーティクル

		@retval	関数値
			true == パーティクルが有効
			false == パーティクルは無効（最大数オーバーなどで描画されなかった……等）

		本クラスの内容を指定フレーム数の状態に更新します。
		*/
		bool Exec(	float frame,
					int index,
					EntityEffect& entity,
					Library::Control::Effect& controlEffect,
					Library::Control::Effect::Emitter& emitter,
					Activity& activity,
					int indexEmitterParent,
					Particle& particleParent
				);

		/* ********************************************************* */
		//! 計算
		/*!
		@param	frame
			フレーム位置
		@param	entity
			エフェクトのエンティティ
		@param	controlEffect
			エフェクトの再生制御
		@param	emitter
			所属エミッタ
		@param	dataEmitter
			所属エミッタの静的データ
		@param	indexEmitterParent
			親エミッタのインデックス
		@param	particleParent
			親のパーティクル
		@param	flagSimplicity
			true == 親の移動状態を無視する（高速算出）
			false == 親の移動状態から接線加速度を求める

		@retval	関数値
			true == パーティクルが有効
			false == パーティクルは無効（出現していない……等）

		指定されたフレーム位置のパーティクルの状態を算出します。
		*/
		bool Calculate(	float frame,
						EntityEffect& entity,
						Library::Control::Effect& controlEffect,
						Library::Control::Effect::Emitter& emitter,
						Library::Data::Effect::Emitter& dataEmitter,
						bool flagSimplicity = false
					);

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* エミッタ再生管理 */
	class Emitter	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum FlagBitStatus	{
			VALID = 0x40000000,
			RUNNING = 0x20000000,

			CHANGE_CELL_UNREFLECTED = 0x08000000,

			CLEAR = 0x00000000
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		Uint32 /* FlagBitStatus */ Status;
		int IndexParent;

		Library::Data::Project::Effect* DataEffect;
		int IDParts;
		int IndexEmitter;
		Library::Data::Effect::Emitter DataEmitter;

		Library::Data::Animation::Attribute::Cell DataCellApply;

		Library::Utility::Random::Generator* Random;

		void* WorkAreaTable;	/* 動的にテーブルが生成された時のみ!nullptrです */
		int CountTablePatternEmit;
		Library::Data::Effect::Emitter::PatternEmit* TablePatternEmit;
		int CountTablePatternOffset;
		const Sint32* TablePatternOffset;
		int CountTableSeedParticle;
		const Sint64* TableSeedParticle;
		Library::Control::Effect::Particle::Activity* TableActivityParticle;

		Uint32 SeedRandom;
		Uint32 SeedOffset;

		int Duration;
		CPU::Vector2 Position;
		int FrameGlobal;

		/* ワーク */
		/* MEMO: これらはprivateスコープでも良いかもしれない。 */
		Library::Control::Effect::Particle ParticleTempolary2;	/* (mainly) for TurnToDirection (mainly) */
		Library::Control::Effect::Particle ParticleTempolary;	/* (mainly) for Parent */
		Library::Control::Effect::Particle Particle;

		/* MEMO: 下記はパーティクルの基本情報です。                                                 */
		/*       この値に各パーティクル毎の値を加味して最終的な頂点データとして描画します。         */
		/*       本値群はエミッタが使用しているセルによって共通の値になるためキャッシュしています。 */
		/*       ※ただし、UVTextureDrawのzとwについては、各パーティクルの描画時に上書しています。  */
		/* MEMO: C#では（構文の関係上）配列を別途newしていましたが、（固定要素数なので）C++では配列実体を確保しています。 */
		PrimitiveBatcher::SettingRender SettingRender;
		CPU::Vector4 FlagsDraw;
		alignas(SpriteStudio6::AlignmentMemory::TUPPLE_VARIABLE) FPU::Vector4 CoordinateDraw[Library::KindVertex::TERMINATOR2];
		alignas(SpriteStudio6::AlignmentMemory::TUPPLE_VARIABLE) CPU::Vector4 UVTextureDraw[Library::KindVertex::TERMINATOR2];

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		/* ********************************************************* */
		//! 未使用化
		/*!
		@param	なし

		@retval	なし

		本クラスの内容を未使用状態にします。
		*/
		inline void CleanUp(void)
		{
			Status = FlagBitStatus::CLEAR;
			IndexParent = -1;

			DataEffect = nullptr;
			IDParts = -1;
			IndexEmitter = -1;

			DataCellApply.CleanUp();

			Random = nullptr;

			WorkAreaTable = nullptr;
			CountTablePatternEmit = 0;
			TablePatternEmit = nullptr;
			CountTablePatternOffset = 0;
			TableSeedParticle = nullptr;
			CountTableSeedParticle = 0;
			TableActivityParticle = nullptr;

			SeedRandom = (Uint32)Library::Data::Effect::Emitter::Constant::SEED_MAGIC;
			SeedOffset = 0;

			Duration = 0;
			Position = CPU::Vector2::Zero;
			FrameGlobal = 0;

			SettingRender.CleanUp();
//			FlagsDraw
//			CoordinateDraw
//			UVTextureDraw
		}

		/* ********************************************************* */
		//! エミッタ再生管理が使用するワークエリア長の取得
		/*!
		@param	entity
			エンティティ
		@param	dataEffect
			エフェクトデータ
		@param	indexEmitter
			エミッタ番号

		@retval	関数値
			必要なワークエリアのバイト長
			0 == ワークエリアが必要ない（ないしはエラー）

		本クラスが管理するエフェクトを再生するために必要な追加ワーク
			エリアのバイト長を取得します。
		※原則本関数が正常動作時に0を返すことはあり得ないので、0が
			返った場合はエラーと断定して構いません。
		*/
		static size_t SizeGetWorkArea(EntityEffect& entity, Library::Data::Project::Effect& dataEffect, int indexEmitter);

		/* ********************************************************* */
		//! 全再生フレーム数を取得
		/*!
		@param	なし

		@retval	関数値
			再生フレーム数

		本エミッタの再生フレーム数を計算します。
		*/
		inline int FrameGetFull(void)
		{
			Library::Data::Effect::Emitter dataEmitter;
			DataEffect->TableEmitter(&dataEmitter, IndexEmitter);

			Library::Data::Effect::Emitter::RangeFloat dataRangeFloat;
			dataEmitter.DurationParticle(&dataRangeFloat);

			return(	(int)(	dataEmitter.DurationEmitter()
							+ dataRangeFloat.Main
							+ dataRangeFloat.Sub
							+ dataEmitter.Delay()
						)
				);
		}

		/* ********************************************************* */
		//! 起動
		/*!
		@param	entity
			エンティティ
		@param	dataEffect
			エフェクトの静的データ
		@param	idParts
			パーツID
		@param	indexDataEmitter
			エミッタの静的データ内インデックス
		@param	indexParent
			親エミッタのインデックス
		@param	controlEffect
			再生管理情報
		@param	workArea
			追加ワークエリアの先頭（の格納先）

		@retval	関数値
			true == 成功
			false == 失敗（エラー）
		@param	workArea
			次の追加ワークエリアの先頭

		本クラスを起動します。
		*/
		bool BootUp(	EntityEffect& entity,
						Library::Data::Project::Effect& dataEffect,
						int idParts,
						int indexDataEmitter,
						int indexParent,
						Library::Control::Effect& controlEffect,
						Uint8*& workArea
				);

		/* ********************************************************* */
		//! 終了
		/*!
		@param	なし

		@retval	なし

		本クラスを終了します。
		*/
		void ShutDown(void);

		/* ********************************************************* */
		//! 更新
		/*!
		@param	frame
			更新するターゲットフレーム位置
		@param	entity
			エンティティ
		@param	dataEffect
			エフェクトの静的データ
		@param	indexEmitterParent
			親エミッタのインデックス

		@retval	関数値
			true == 成功
			false == 失敗（もしくは描画されなかった）

		本クラスの状態を指定フレームの状態に更新（再計算）します。
		*/
		bool Update(	float frame,
						EntityEffect& entity,
						Library::Control::Effect& controlEffect,
						int indexEmitterParent
					);

		/* ********************************************************* */
		//! 更新
		/*!
		@param	frame
			更新するターゲットフレーム位置
		@param	entity
			エンティティ
		@param	dataEffect
			エフェクトの静的データ
		@param	indexEmitterParent
			親エミッタのインデックス
		@param	emitterTarget
			ターゲットエミッタ

		@retval	関数値
			true == 成功
			false == 失敗（もしくは描画されなかった）

		子エミッタ群を指定フレームの状態に更新します。
		*/
		bool UpdateSubEmitters(	float frame,
								EntityEffect& entity,
								Library::Control::Effect& controlEffect,
								int indexEmitterTarget,
								Emitter& emitterTarget
							);

		/* ********************************************************* */
		//! パーティクルの使用セルを設定
		/*!
		@param	entity
			エンティティ

		@retval	関数値
			true == 成功
			false == 失敗

		エミッタで排出するパーティクルのセル情報を事前設定します。
		*/
		bool CellPresetParticle(EntityEffect& entity);

		/* ********************************************************* */
		//! コンストラクタ
		/*!
		@param	なし

		@retval	なし
		*/
		Emitter(void)
		{
			CleanUp();
		}

		/* ********************************************************* */
		//! デストラクタ
		/*!
		@param	なし

		@retval	なし

		※このクラスは継承したりしないので、仮想化していません。
		*/
		~Emitter(void)
		{
			ShutDown();
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum FlagBitStatus	{
		RUNNING = 0x40000000,
		PLAYING = 0x20000000,

		LOCKSEED = 0x08000000,
		INFINITE_EMIT = 0x04000000,
		LOOP = 0x02000000,

		CLEAR = 0x00000000
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	Uint32 /* FlagBitStatus */ Status;

	int CountTableEmitter;
	Emitter* TableEmitter;
	int CountMeshParticle;

	EntityEffect* Entity;	/* InstanceRootEffect */
	int DurationFull;

	/* MEMO: 下記SS6Player for Unityのバグです。                                                                            */
	/*       エミッタ毎で最大描画数をチェックしていたために、エフェクトのエンティティで設定している最大描画数をオーバーして */
	/*       描画されてしまう障害があったため、エミッタのワークに描画数計測を持つのをやめました。                           */
//	int LimitParticleDraw;
//	int CountParticleDraw;

	Uint32 Seed;
	Uint32 SeedOffset;

	Library::KindMasking::Kind Masking;

	alignas(AlignmentMemory::TUPPLE_VARIABLE) FPU::Matrix4x4 MatrixRoot;

	/* ----------------------------------------------- Functions */
private:
	void* BootUpWorkArea(EntityEffect& entity, void* workArea);
	void ShutDownWorkArea(void);

protected:
public:
	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし

	本クラスの内容を未使用状態にします。
	*/
	inline void CleanUp(void)
	{
		Status = FlagBitStatus::CLEAR;

		CountTableEmitter = 0;
		TableEmitter = nullptr;
		CountMeshParticle = 0;

		Entity = nullptr;
		DurationFull = -1;

		Seed = 0;
		SeedOffset = 0;

		Masking = (Library::KindMasking::Kind)(-1);	/* エラー値 */

		MatrixRoot.Set(FPU::Matrix4x4::Unit);
	}

	/* ********************************************************* */
	//! 乱数種の設定
	/*!
	@param	seed
		乱数種

	@retval	なし

	本クラスが使用している乱数発生器（のインスタンス）に対して使用
		する乱数の種を設定します。
	※本、乱数種は、エフェクトが統一して持っている乱数の種の基準値
		です（実際に使用される乱数種は、本値を考慮した乱数種オフ
		セットで、それがエミッタ毎にUpdate時に更新されて、使用され
		ます）。
	*/
	inline void SeedSet(Uint32 seed)
	{
		Seed = seed * (Uint32)Library::Data::Effect::Emitter::Constant::SEED_MAGIC;
	}

	/* ********************************************************* */
	//! 乱数種オフセットの設定
	/*!
	@param	offsetSeed
		乱数種オフセット

	@retval	なし

	本クラスが使用している乱数発生器（のインスタンス）に対して使用
		する乱数種オフセットを設定します。
	*/
	inline void SeedOffsetSet(Uint32 offsetSeed)
	{
		SeedOffset = (0 != (Status & FlagBitStatus::LOCKSEED)) ? 0 : offsetSeed;
	}

	/* ********************************************************* */
	//! エフェクトのループ再生の設定
	/*!
	@param	flagSwitch
		true == ループさせる
		false == ループさせない

	@retval	なし

	エフェクト再生のループ状態を設定します。
	*/
	inline void StatusSetLoop(bool flagSwitch)
	{
		Status = (true == flagSwitch) ? (Status | FlagBitStatus::LOOP) : (Status & ~FlagBitStatus::LOOP);
	}

	/* ********************************************************* */
	//! エフェクト再生管理が使用するワークエリア長の取得
	/*!
	@param	entity
		エンティティ

	@retval	関数値
		必要なワークエリアのバイト長
		0 == ワークエリアが必要ない（ないしはエラー）

	本クラスが管理するエフェクトを再生するために必要な追加ワーク
		エリアのバイト長を取得します。
	※原則本関数が正常動作時に0を返すことはあり得ないので、0が
		返った場合はエラーと断定して構いません。
	*/
	static size_t SizeGetWorkArea(EntityEffect& entity);

	/* ********************************************************* */
	//! 起動
	/*!
	@param	entity
		エンティティ
	@param	workArea
		追加ワークエリアの先頭

	@retval	関数値
		true == 成功
		false == 失敗

	エンティティの設定に従って、エフェクト再生管理を初期起動します。
	*/
	bool BootUp(EntityEffect& entity, void* workArea);

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし

	現在割り当たっているエフェクト再生設定管理を終了します。
	*/
	void ShutDown(void);

	/* ********************************************************* */
	//! 更新
	/*!
	@param	entity
		エンティティ
	@param	masking
		マスキング種別
	@param	argumentUpdate
		描画行列情報
	@param	flagPlanarization
		true == エフェクトの描画を平面化する（深度が統一される）
		false == エフェクトをそのまま描画する（深度はデータによる）
	@param	matrixParent
		親アニメーションから継承する姿勢行列

	@retval	なし

	本クラスが持つエフェクトデータを更新します。
	※原則、本関数はEntityEffectのDraw（描画コマンドリスト作成）
		処理から呼び出してください。
	※argumentUpdateは名称的には「更新」用になっていますが、現状
		厳密には「描画」用となります（現在本Update関数は、描画コ
		マンド作成段で呼ばれるため）。
	*/
	void Update(	EntityEffect& entity,
					Library::KindMasking::Kind masking,
					const Library::Control::Animation::ArgumentContainer& argumentUpdate,
					bool flagPlanarization,
					const FPU::Matrix4x4& matrixParent
			);

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

}	/* Control */
}	/* Library */
}	/* SpriteStudio6 */
