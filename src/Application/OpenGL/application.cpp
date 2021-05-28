/**
	SS6Player for CPP(C++/ssbp2)
	Platform: OpenGL

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "application.h"

#include <ss6player_project.h>
#include <ss6player_entity.h>
#include <ss6player_sequencer.h>
#include <ss6player_entity_replicate.h>

#include <vector>

/* コンパイルオプション: シーケンサ再生 */
/* MEMO: 定義すると、「シーケンサ（シーケンスデータ）」の再生サンプルになります。                */
/*       定義しないと、「エンティティ（1アニメーションオブジェクト）」の再生サンプルになります。 */
#define _COMPILEOPTION_TEST_SEQUENCER_

/* コンパイルオプション: 複製描画の追加 */
/* MEMO: 定義すると、再生しているオブジェクトを(100, 100, 0)の位置に複製描画します。 */
#define _COMPILEOPTION_TEST_REPLICATE_

/* コンパイルオプション: テクスチャを外部定義 */
/* MEMO: 少し変わった運用時のための例です。                                                      */
/*       ※例えば、SS6のデータと他のモデルや描画物がテクスチャを共用しているような場合……など。 */
// #define _COMPILEOPTION_TEXTURE_EXTERNALENTITY_

/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */
/* サンプル: 再生制御クラス */
/* MEMO: 定型の処理や、EntityとSequencerでの扱いの差異を埋めるようなサンプルとして……。 */
class HandlerSpriteStudio6
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 再生対象データ */
	void* Data;
	void* HandleHeap;
	SpriteStudio6::Project Project;

	/* 再生制御 */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
	SpriteStudio6::Sequencer Sequencer;								/* シーケンサ *//* ※シーケンサは内部にEntityを持っているので、Entityを直接設置しないでください。 */
#else
	SpriteStudio6::Entity Entity;									/* 単独再生オブジェクト */
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

	/* 使用テクスチャ群 */
	int CountTexture;
	void** HandleHeapTexture;
#if defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_)
	SpriteStudio6::Texture* Texture;
#else
#endif	/* defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_) */

	/* ----------------------------------------------- Functions */
private:
protected:
	/* ********************************************************* */
	//! 未使用化
	/*!
	@param	なし

	@retval	なし
	*/
	void CleanUp(void)
	{
		Data = nullptr;
		HandleHeap = nullptr;
//		Project

#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
//		Sequencer
#else
//		Entity
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

		CountTexture = 0;
		HandleHeapTexture = nullptr;
#if defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_)
		Texture = nullptr;
#endif	/* defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_) */
	}

public:
	/* ********************************************************* */
	//! 本クラスが有効か？
	/*!
	@param	なし

	@retval	関数値
		true == 有効
		false == 無効

	本クラスが描画用として使用可能かを返します。
	*/
	inline bool IsValid(void)
	{
		if(nullptr == Data)	{
			return(false);
		}

#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		return(Sequencer.IsValid());
#else
		return(Entity.IsValid());
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */
	}

	/* ********************************************************* */
	//! SpriteStudio6のデータ（SSFB2）をロード
	/*!
	@param	nameFile
		SSFB2ファイル名

	@retval	関数値
		true == 成功
		false == 失敗

	指定されたSSFB2ファイルを読み込んで、エンティティを描画操作
		可能な状態にします。
	同時にSSFB2内で使用しているテクスチャもロードしています。

	nameFileは"rom:/"で開始するROM上の絶対パスである必要があります。
	※この仕様は後に（マウント名が必要なく・指定フォルダ基準の相対
		記述に）修正します。
	*/
	bool DataLoadSSFB2(const char* nameFile)
	{
		/* SSFB2をロード */
		/* MEMO: SSFB2自体はファイルアライメントにあわせる必要はないのですが、 */
		/*       一応他のリソースファイル群と同じアライメントにしておきます。  */
		/* MEMO: 横着して、SS6Playerの実装関数を呼んでいますが、アプリケーション側のファイルシステムを使用して読み込むことを推奨します。 */
		Data = SpriteStudio6::Platform::File::DataLoad(&HandleHeap, nameFile, SpriteStudio6::AlignmentMemory::FILE_BUFFER);
		if(nullptr == Data)	{	/* エラー */
			CleanUp();
			return(false);
		}

		/* データを使用可能化 */
		{
			/* プロジェクトにデータを設定 */
			if(false == Project.BootUp(Data))	{
				CleanUp();
				return(false);
			};

			/* 付随するテクスチャをロード・設定 */
			int countTexture = Project.CountGetTexture();
			CountTexture = countTexture;
#if defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_)
			/* MEMO: 外部テクスチャを使用する場合、テーブルを作成して定義します。 */
			Texture = new SpriteStudio6::Texture[countTexture];
#else
#endif	/* defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_) */
			HandleHeapTexture = new void*[countTexture];
			for(int i=0; i<countTexture; i++)	{
				HandleHeapTexture[i] = nullptr;
			}

			std::string nameTexture;
			void* handleDataTexture;
			for(int i=0; i<countTexture; i++)	{
				/* SSFB2内で使用しているテクスチャファイルのロード */
				/* MEMO: 横着して、SS6Playerの実装関数を呼んでいますが、アプリケーション側のファイルシステムを使用して読み込むことを推奨します。 */
				nameTexture = u8"texture/" + std::string(Project.NameGetTexture(i)) + u8".png";
				handleDataTexture = SpriteStudio6::Platform::File::DataLoad(&HandleHeapTexture[i], nameTexture.c_str(), SpriteStudio6::AlignmentMemory::FILE_BUFFER);
				if(nullptr == handleDataTexture)	{	/* 失敗 */
					return(false);
				}

#if defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_)
				/* MEMO: 外部テクスチャを使用する場合、テクスチャのキャッシュへの登録はSpriteStudio6::Textureクラスで */
				/*       提供されている関数を使用して、自身で行わなくてはいけません。                                 */
				/* テクスチャをキャッシュに登録 */
				bool flagExistCacheTexture;
				SpriteStudio6::TypeKeyCacheTexture keyCacheTexture = SpriteStudio6::Texture::CacheStore(&flagExistCacheTexture, handleDataTexture);
				if(false == Texture[i].BootUp(keyCacheTexture))	{	/* 失敗 */
					/* MEMO: テクスチャデータ領域を解放すること */
					return(false);
				}

				/* プロジェクトにテクスチャを登録 */
				if(false == Project.TextureSet(i, Texture[i]))	{	/* 失敗 */
					/* MEMO: テクスチャデータ領域を解放すること */
					return(false);
				}
#else
				/* MEMO: 外部でテクスチャを管理しない場合、Projectクラスに使用するテクスチャのハンドルを設定する */
				/*       ことで、あとの操作はSS6Playerが行います。                                               */
				/* プロジェクトにテクスチャを定義 */
				if(false == Project.TextureSet(i, handleDataTexture))	{
					/* MEMO: テクスチャデータ領域を解放すること */
					return(false);
				}
#endif	/* defined(_COMPILEOPTION_TEXTURE_EXTERNALENTITY_) */
			}
		}

#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		/* シーケンサにSSFB2を設定 */
		if(false == Sequencer.BootUp(Project))	{
			return(false);
		}
#else
		/* エンティティにSSFB2を設定 */
		/* MEMO: マルチトラック再生機能を使用する場合、ここでトラック数指定をしてしまうのが良いかと思われます。 */
		/*       ※Entity::TrackRebootを使用すると、再アロケートなどが起こって若干非効率だったりするので、最大  */
		/*         使用トラック数が確定できる場合は、BootUp時に第2引数（countTrack）で与えて確保してしまうのが  */
		/*         良策ではあります。                                                                           */
		if(false == Entity.BootUp(Project))	{
			return(false);
		}
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

		return(true);
	}

#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
	/* ********************************************************* */
	//! シーケンサにシーケンスパックを設定
	/*!
	@param	nameFile
		シーケンスパック（SSQEファイルボディ）名

	@retval	関数値
		true == 成功
		false == 失敗

	指定された（プロジェクト内の）シーケンスパックが持つ諸情報を
		シーケンサに設定します。
	この処理によってシーケンサが指定シーケンスパックが持つシー
		ケンス群を再生可能になります。
	*/
	bool PackSetSequence(const char* namePackSequence)
	{
		/* MEMO: エンティティには同等の処理がない（PackSet関数はありますが趣がかなり違うので……）。 */
		int indexPackSequence = Project.IndexGetSequencePack(namePackSequence);
		if(0 > indexPackSequence)	{
			return(false);
		}
		return(Sequencer.PackSet(indexPackSequence));
	}
#else
	/* ********************************************************* */
	//! エンティティにアニメーションパック（モデル）を設定
	/*!
	@param	namePackAnimation
		アニメーションパック（SSAEファイルボディ）名

	@retval	関数値
		true == 成功
		false == 失敗

	指定された（プロジェクト内の）アニメーションパックが持つ
		パーツ構成情報（モデル情報）をエンティティ内に設定します。
	この処理によってエンティティが指定アニメーションパックが持つ
		アニメーション群を再生可能になります。
	*/
	bool PackSetAnimation(const char* namePackAnimation)
	{
		/* MEMO: シーケンサには同等の処理がない（PackSet関数はありますが趣がかなり違うので……）。 */
		int indexPackAnimation = Project.IndexGetAnimationPack(namePackAnimation);
		if(0 > indexPackAnimation)	{
			return(false);
		}
		return(Entity.PackSet(indexPackAnimation));
	}
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

	/* ********************************************************* */
	//! 終了
	/*!
	@param	なし

	@retval	なし

	終了処理をします。
	*/
	void ShutDown(void)
	{
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		Sequencer.ShutDown();
#else
		Entity.ShutDown();
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

		/* プロジェクトの解放 */
		Project.ShutDown();

		/* テクスチャのメモリを解放 */
		if(nullptr != HandleHeapTexture)	{
			for(int i=0; i<CountTexture; i++)	{
				if(nullptr != HandleHeapTexture[i])	{
					if(nullptr != HandleHeapTexture[i])	{
						SpriteStudio6::Memory::Free(HandleHeapTexture[i]);
						HandleHeapTexture[i] = nullptr;
					}
				}
			}

			delete[] HandleHeapTexture;
			HandleHeapTexture = nullptr;
		}

		/* SSFB2データの解放 */
		if(nullptr != HandleHeap)	{
			SpriteStudio6::Memory::Free(HandleHeap);
			HandleHeap = nullptr;
		}
	}

	/* ********************************************************* */
	//! アニメーションの更新
	/*!
	@param	timeElaapsed
		経過時間（単位: 1.0f == 1秒）

	@retval	関数値
		true == 成功
		false == 失敗

	必ず定常の1処理ループで1回呼び出す必要があります（1つのハンドラ
		で複数回呼ばないでください）。
	*/
	bool Update(float timeElapsed)
	{
		if(false == IsValid())	{
			return(false);
		}

		/* 再生実体の更新 */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		return(Sequencer.Update(timeElapsed));
#else
		return(Entity.Update(timeElapsed, SpriteStudio6::Library::KindMasking::FOLLOW_DATA));
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */
	}

	/* ********************************************************* */
	//! アニメーションの描画
	/*!
	@param	commandList
		描画情報を書き込むためのコマンドリスト
	@param	matrixProjection
		描画するためのプロジェクション行列
	@param	matrixView
		描画するためのビュー行列
	@param	matrixWorld
		描画するためのローカル→ワールド変換行列

	@retval	関数値
		更新されたコマンドリストの値
		※殆どのプラットフォームの場合引数と変わらないです。
		DrawCommandListInvalid == 失敗

	本クラスで再生中のアニメーションの状態を描画します。
	*/
	SpriteStudio6::TypeDrawCommandList Draw(	SpriteStudio6::TypeDrawCommandList commandList,
												const SpriteStudio6::CPU::Matrix4x4& matrixProjection,
												const SpriteStudio6::CPU::Matrix4x4& matrixView,
												const SpriteStudio6::CPU::Matrix4x4& matrixWorld
										)
	{
		if(false == IsValid())	{
			return(nullptr);
		}

		/* 再生実体の更新 */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		return(Sequencer.Draw(commandList, matrixProjection, matrixView, matrixWorld));
#else
		return(Entity.Draw(commandList, matrixProjection, matrixView, matrixWorld));
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */
	}

	/* ********************************************************* */
	//! アニメーションの一時停止状態の設定
	/*!
	@param	flagSwitch
		true == 一時停止
		false == 一時停止解除

	@retval	関数値
		true == 成功
		false == 失敗

	必ず定常の1処理ループで1回呼び出す必要があります（1つのハンドラ
		で複数回呼ばないでください）。
	*/
	bool PauseSet(bool flagSwitch)
	{
		if(false == IsValid())	{
			return(false);
		}

		/* 再生実体に一時停止状態を設定 */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
		return(Sequencer.PauseSet(flagSwitch));
#else
		/* MEMO: とりあえずトラック指定なし（全トラック停止＋トラック0で再生開始）です。 */
		/*       ※アニメーション合成を行わないのであれば、通常はこれで大丈夫です。      */
		return(Entity.AnimationPause(-1, flagSwitch));
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */
	}

	/* ********************************************************* */
	//! コンストラクタ
	/*!
	@param	なし

	@retval	なし
	*/
	HandlerSpriteStudio6(void)
	{
		CleanUp();
	}

	/* ********************************************************* */
	//! デストラクタ
	/*!
	@param	なし

	@retval	なし

	本クラスを継承する場合を考慮して、仮想デストラクタになっています。
	*/
	~HandlerSpriteStudio6(void)
	{
	}

	/* ----------------------------------------------- Functions (static) */
private:
protected:
public:

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* -------------------------------------------------------------------------- */
/*                                                   [Global-Scope] Variables */
/* -------------------------------------------------------------------------- */
static float TimeElapsed = 0.0f;

/* -------------------------------------------------------------------------- */
/*                                            [File-Scope internal] Variables */
/* -------------------------------------------------------------------------- */
static HandlerSpriteStudio6 HandlerSS6;
#if defined(_COMPILEOPTION_TEST_REPLICATE_)
enum {
	COUNT_REPLICATE_X = 10,
	COUNT_REPLICATE_Y = 10,
	BOUND_REPLICATE_X = 192,
	BOUND_REPLICATE_Y = 108,
};
static SpriteStudio6::EntityReplicate EntityReplicateSS6[COUNT_REPLICATE_Y][COUNT_REPLICATE_X];
#endif	/* defined(_COMPILEOPTION_TEST_REPLICATE_) */

/* -------------------------------------------------------------------------- */
/*              [File-Scope internal] Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
static bool PauseSet(HandlerSpriteStudio6& handler, bool flagSwitch)
{
	return(handler.Sequencer.PauseSet(flagSwitch));
}
#else
static bool PauseSet(HandlerSpriteStudio6& handler, bool flagSwitch)
{
	return(handler.Entity.AnimationPause(-1, flagSwitch));
}
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

/* -------------------------------------------------------------------------- */
/*                                  [File-Scope internal] Function Prototypes */
/* -------------------------------------------------------------------------- */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
#else
/* ユーザデータ検知のコールバック受領関数 */
static void CallBackFunctionUserData(	SpriteStudio6::Entity& entity,
										void* pointer,
										const char* nameParts,
										int idParts,
										int indexAnimation,
										int frameDecode,
										int frameKeyData,
										const SpriteStudio6::Library::Data::Animation::Attribute::UserData& userData,
										bool flagWayBack
									)
{
//	bool flagNumber = userData.IsNumber();
}

/* シグナル検知のコールバック受領関数 */
static void CallBackFunctionSignal(	SpriteStudio6::Entity& entity,
								void* pointer,
								const char* nameParts,
								int idParts,
								int indexAnimation,
								int frameDecode,
								int frameKeyData,
								const SpriteStudio6::Library::Data::Animation::Attribute::Signal& signal,
								bool flagWayBack
							)
{
	SpriteStudio6::Library::Data::Animation::Attribute::Signal::Command command;
	SpriteStudio6::Library::Data::Animation::Attribute::Signal::Command::Parameter parameterSignal;

	int countCommand = signal.CountGetCommand();
	for(int i=0; i<countCommand; i++)	{
		signal.CommandGet(&command, i);

		const char* id = command.IDGet();
		int countParameter = command.CountGetParameter();
		const char* note = command.NoteGet();

		for(int j=0; j<countParameter; j++)	{
			command.ParameterGet(&parameterSignal, j);

			SpriteStudio6::Library::Data::Animation::Attribute::Signal::Command::Parameter::KindType type = parameterSignal.TypeGet();
			const char* name = parameterSignal.NameGet();
		}
	}
}
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! アプリケーションの初期起動処理
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool ApplicationBootUp(void)
{
	SpriteStudio6::FPU::Matrix4x4 matrix01(	0, 1, 2, 3,
											4, 5, 6, 7,
											8, 9, 10, 11,
											12, 13, 14, 15
										);

	SpriteStudio6::FPU::Matrix4x4 matrix02(	100, 101, 102, 103,
											104, 105, 106, 107,
											108, 109, 110, 111,
											112, 113, 114, 115
										);
	SpriteStudio6::FPU::Matrix4x4 matrix03(	3, 1, 1, 2,
											5, 1, 3, 4,
											2, 0, 1, 0,
											1, 3, 2, 1
										);

	SpriteStudio6::FPU::Matrix4x4 matrixResult;
	SpriteStudio6::MatrixMul(&matrixResult, matrix01, matrix02);
	SpriteStudio6::MatrixInverse(&matrixResult, matrix03);
	SpriteStudio6::MatrixTranspose(&matrixResult, matrix03);

	/* データのロード・エンティティの起動・対応テクスチャのロード */
#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
	HandlerSS6.DataLoadSSFB2(u8"Doll.ssfb2");
#else
	HandlerSS6.DataLoadSSFB2(u8"Doll.ssfb2");
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

#if defined(_COMPILEOPTION_TEST_SEQUENCER_)
	/* アニメーションのモデルをシーケンスに割当 */
	HandlerSS6.PackSetSequence(u8"Sequence");	/* シーケンサにシーケンスパックを割当 */

	/* シーケンスの再生開始 */
	/* MEMO: シーケンスパック中の指定番号のシーケンスを再生します。 */
	int indexSequence = HandlerSS6.Sequencer.IndexGetSequence(u8"Seq_01");
	HandlerSS6.Sequencer.Play(indexSequence, 1.0f);
#else
	/* アニメーションのモデルをエンティティに割当 */
	HandlerSS6.PackSetAnimation(u8"Action01_Body");

#if 0	/* MEMO: 再生終了などのコールバックを受け取る場合は、下記の方法で関数を定義します（無論、直接EntityやSequencerに設定しても構いません）。 */
	/* コールバック関数群設定 */
	HandlerSS6.Entity.CallBackUserData.Set(CallBackFunctionUserData, nullptr);
	HandlerSS6.Entity.CallBackSignal.Set(CallBackFunctionSignal, nullptr);
#endif

	/* アニメーションの再生開始 */
	/* MEMO: アニメーションパック中の指定番号のアニメーションを再生します。 */
	int indexAnimation = HandlerSS6.Entity.IndexGetAnimation(u8"PutOutL");

	HandlerSS6.Entity.AnimationPlay(-1, indexAnimation);
#endif	/* defined(_COMPILEOPTION_TEST_SEQUENCER_) */

#if defined(_COMPILEOPTION_TEST_REPLICATE_)
	for(int y=0; y<COUNT_REPLICATE_Y; y++)	{
		for(int x=0; x<COUNT_REPLICATE_X; x++)	{
			EntityReplicateSS6[y][x].BootUp();
		}
	}
#endif	/* defined(_COMPILEOPTION_TEST_REPLICATE_) */

	return(true);
}

/* ********************************************************* */
//! アプリケーションの終了処理
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void ApplicationShutDown(void)
{
	/* ハンドラの削除 */
	HandlerSS6.ShutDown();

#if defined(_COMPILEOPTION_TEST_REPLICATE_)
	for(int y=0; y<COUNT_REPLICATE_Y; y++)	{
		for(int x=0; x<COUNT_REPLICATE_X; x++)	{
			EntityReplicateSS6[y][x].ShutDown();
		}
	}
#endif	/* defined(_COMPILEOPTION_TEST_REPLICATE_) */
}

/* ********************************************************* */
//! アプリケーションの定常更新処理
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool ApplicationUpdate(SpriteStudio6::Float32 timeElapsed)
{
	/* デバグ操作 */
	TimeElapsed = timeElapsed;
	bool flagTogglePause = false;
#if defined(NN_SDK_BUILD_DEBUG)
	bool flagContinueApplication = ApplicationDebugUpdate(HandlerSS6, &timeElapsed, &flagTogglePause);
#else
	bool flagContinueApplication = true;
#endif	/* defined(NN_SDK_BUILD_DEBUG) */

	/* 各アニメーションハンドラの更新 */
	if(false == HandlerSS6.Update(timeElapsed))	{	/* 失敗 */
	}

	/* 一時停止の処理 */
	/* MEMO: Updateの後で一時停止を行うと、実際には次のループ処理のUpdate時に停止します。 */
	if(true == flagTogglePause)	{
		HandlerSS6.PauseSet(true);
	}

	return(flagContinueApplication);
}

/* ********************************************************* */
//! アプリケーションの描画処理
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool ApplicationDraw(SpriteStudio6::TypeDrawCommandList commandList)
{
	/* 静的レンダラをリフレッシュ */
	SpriteStudio6::Renderer& renderer = SpriteStudio6::Renderer::Instance;
	renderer.Refresh();	/* 内部情報をリフレッシュ */
	renderer.Rasterizer.Set();	/* ラスタライザ情報を出力（原則、描画処理の最初だけで良いはず） */

	/* フレームバッファをクリア */
	/* MEMO: ここに入っていますが、基本的にはアプリケーションの描画時処理に依存します。          */
	/*       フレームバッファのクリアはSpriteStudio6::Rendererを使用して行わなくても構いません。 */
	renderer.Target.ColorSet(0, &SpriteStudio6::Texture::TargetMasterColor, true, SpriteStudio6::CPU::Color(0.03f, 0.28f, 0.22f, 0.0f), true);
	renderer.Target.TestSet(&SpriteStudio6::Texture::TargetMasterTest, true, 0, 0.0f, true);
	commandList = renderer.MakeCommand(commandList);	/* 一旦レンダラの内容を出力しています。 */

	/* プロジェクション行列の作成 */
	SpriteStudio6::CPU::Matrix4x4 matrixProjection;
	SpriteStudio6::MatrixGetProjectionOrtho(&matrixProjection, -960.0f, 960.0f, 540.0f, -540.0f, 0.0f, 1000.0f);

	commandList = HandlerSS6.Draw(	commandList,
										matrixProjection,
										SpriteStudio6::CPU::Matrix4x4::Unit,
										SpriteStudio6::CPU::Matrix4x4::Unit
								);
	if(nullptr == commandList)	{	/* 失敗 */
		return(false);
	}

#if defined(_COMPILEOPTION_TEST_REPLICATE_)
	{
		SpriteStudio6::FPU::Matrix4x4 matrixWorld;
		for(int y=0; y<COUNT_REPLICATE_Y; y++)	{
			for(int x=0; x<COUNT_REPLICATE_X; x++)	{

				SpriteStudio6::MatrixCreateTRS(	&matrixWorld,
												SpriteStudio6::FPU::Vector3((float)(x * BOUND_REPLICATE_X) - 960.0f + 100.0f, (float)(y * BOUND_REPLICATE_Y) - 540.0f, 0.0f),
												SpriteStudio6::FPU::Vector3::Zero,
												SpriteStudio6::FPU::Vector3::One
											);

#if defined(_COMPILEOPTION_TEST_SEQUENCER_)	/* MEMO: シーケンサ用デバグ処理 */
				commandList = EntityReplicateSS6[y][x].Draw(	commandList,
																matrixProjection,
																SpriteStudio6::CPU::Matrix4x4::Unit,
																matrixWorld,
																HandlerSS6.Sequencer.Entity
															);
				if(nullptr == commandList)	{	/* 失敗 */
				}
#else
				commandList = EntityReplicateSS6[y][x].Draw(	commandList,
																matrixProjection,
																SpriteStudio6::CPU::Matrix4x4::Unit,
																matrixWorld,
																HandlerSS6.Entity
														);
				if(nullptr == commandList)	{	/* 失敗 */
				}
#endif
			}
		}
	}
	if(nullptr == commandList)	{	/* 失敗 */
	}
#endif	/* defined(_COMPILEOPTION_TEST_REPLICATE_) */

	return(true);
}

