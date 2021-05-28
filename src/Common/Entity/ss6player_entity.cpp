/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_entity.h"
#include "ss6player_entity_effect.h"
#include "../Library/ss6player_utility.h"
#include "../Project/ss6player_project.h"

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
/* デフォルトで適用される初期再生トラック数 */
int Entity::LimitTrack = 1;

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
/*                                                    Entity::InformationPlay */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// Entity::InformationPlay::InformationPlay(void)

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// void Entity::InformationPlay::CleanUp(void)

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                                     Entity */
/* -------------------------------------------------------------------------- */
/* ********************************************************* */
//! コンストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Entity::Entity(void)
{
	CleanUp();
}

/* ********************************************************* */
//! デストラクタ
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
Entity::~Entity(void)
{
//	CleanUp();
}

/* ********************************************************* */
//! 未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::CleanUp(void)
{
	base::CleanUp();

	Status = FlagBitStatus::CLEAR;

	CleanUpWorkAreaProject();
	CleanUpWorkAreaPack();

	ListPartsDraw.clear();
	ListPartsPreDraw.clear();

	CallBackPlayEnd.CleanUp();
	CallBackUserData.CleanUp();
}

/* ********************************************************* */
//! プロジェクト共通ワークエリア未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::CleanUpWorkAreaProject(void)
{
	DataAnimation = nullptr;

	WorkAreaProject = nullptr;
	TableInformationPlay = nullptr;
	TableControlTrack = nullptr;

	WorkAreaSetTexture(0, nullptr);

	CallBackPlayEndTrack = nullptr;
}

/* ********************************************************* */
//! アニメーションパック依存ワークエリア未使用化
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::CleanUpWorkAreaPack(void)
{
	IndexPack = -1;

	WorkAreaPack = nullptr;
	CountParts = 0;
	TableControlParts = nullptr;
	CountPartsDraw = 0;
	TableControlPartsDraw = nullptr;
	WorkAreaPackAdditional = nullptr;

	CountEntityChildInstance = 0;
	ListEntityChildInstance = nullptr;
	CountEntityChildEffect = 0;
	ListEntityChildEffect = nullptr;

	ModelDraw = nullptr;

	/* MEMO: 最終的にこの連中も含みたい。 */
//	ListPartsDraw = nullptr;
//	ListPartsPreDraw = nullptr;
//	ListPartsIDRender = nullptr;
//	ListPartsIDPreRender = nullptr;
}

/* ********************************************************* */
//! プロジェクト共通ワークエリア起動
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::BootUpWorkAreaProject(int countTrack, int countTexture, int countTextureOffscreen)
{
	/* MEMO: このワークエリアには、原則「プロジェクト（SSPJ）を変更しない限り */
	/*       構成が変わらない」ものが配置されています。                       */
	/*       - トラック再生（再生ワーク・再生情報）                           */
	/*       - トラック再生終了コールバック                                   */
	/*       - テクスチャ（描画使用・オフスクリーン）                         */
	CountTrack = countTrack;
	CountTexture = countTexture;

	const size_t sizeShaderData = sizeof(ShaderData);
	const size_t sizeInformationPlay = sizeof(InformationPlay) * countTrack;
	const size_t sizeTableControlTrack = sizeof(Library::Control::Animation::Track) * countTrack;
	const size_t sizeTexture = base::SizeGetWorkAreaTexture(countTexture);
	const size_t sizeTextureOffscreenColor = 0;
	const size_t sizeTextureOffscreenTest = 0;
	const size_t sizeCallBackPlayEndTrack = sizeof(Library::Control::CallBack<TypeFunctionControlEndTrackPlay>) * countTrack;
	size_t sizeWorkArea =	sizeInformationPlay
							+ sizeTableControlTrack
							+ sizeTexture
							+ sizeTextureOffscreenColor
							+ sizeTextureOffscreenTest
							+ sizeCallBackPlayEndTrack;
	if(nullptr == EntityParent)	{
		/* MEMO: 最親の時にはシェーダ定数を持っています。 */
		sizeWorkArea += sizeShaderData;
	}

	/* ヒープ取得 */
	Memory::Allocate(&WorkAreaProject, sizeWorkArea);
	if(nullptr == WorkAreaProject)	{
		return(false);
	}
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(WorkAreaProject);

	/* 各ワークへの領域割当 */
	TableInformationPlay = reinterpret_cast<InformationPlay*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeInformationPlay;
	TableControlTrack = reinterpret_cast<Library::Control::Animation::Track*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeTableControlTrack;

	WorkAreaSetTexture(countTexture, pointWorkAreaNow);
		pointWorkAreaNow += sizeTexture;

	CallBackPlayEndTrack = reinterpret_cast<Library::Control::CallBack<TypeFunctionControlEndTrackPlay>*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeCallBackPlayEndTrack;

	if(nullptr == EntityParent)	{
		ConstantShader = reinterpret_cast<ShaderData*>(pointWorkAreaNow);
			pointWorkAreaNow += sizeShaderData;
	} else {
		ConstantShader = EntityParent->ConstantShader;
	}

	/* 各領域のクリア */
	/* MEMO: 実体配列を取得した領域は、念のため配置newで初期化しています。 */
	/*       ※仮想関数やコンストラクタ必須のメンバができた時のため。      */
	/* MEMO: テクスチャ関係はBufferSetTexture関数の中でクリアされています。 */
	for(int i=0; i<countTrack; i++)	{
		new(&TableInformationPlay[i]) InformationPlay;
		new(&TableControlTrack[i]) Library::Control::Animation::Track;
		new(&CallBackPlayEndTrack[i]) Library::Control::CallBack<TypeFunctionControlEndTrackPlay>;
	}

	if(nullptr == EntityParent)	{
		/* MEMO: シェーダ定数は起動までしておく。 */
		new(ConstantShader) ShaderData;
		ConstantShader->BootUp();
	}

	return(true);
}

/* ********************************************************* */
//! アニメーションパック依存ワークエリア起動
/* --------------------------------------------------------- */
/* MEMO:
 * この関数はPackSetの中からIndexPack・DataAnimationの設定が
 * 完了した段階で呼び出してください。
 */
bool Entity::BootUpWorkAreaPack(void)
{
	/* MEMO: このワークエリアには、原則「再生するアニメーションパック（SSAE）が変更されると */
	/*       構成が変わる」ものが配置されています（主にパーツ構成に影響を受けるもの）。     */
	/*       - パーツ制御（再生ワーク）                                                     */
	/*       - パーツ描画制御                                                               */
	/*       - 子アニメーション（インスタンス・エフェクトテクスチャ）用エンティティ         */
	CountParts = DataAnimation->CountTableParts();
	Library::Data::Parts::Animation::Catalog catalogParts;
	DataAnimation->CatalogParts(&catalogParts);
	CountEntityChildInstance = catalogParts.CountTableIDPartsInstance();
	CountEntityChildEffect = catalogParts.CountTableIDPartsEffect();
	CountPartsDraw =	catalogParts.CountTableIDPartsNormal()
						+ catalogParts.CountTableIDPartsMask()
						+ catalogParts.CountTableIDPartsMesh()
						+ CountEntityChildInstance
						+ CountEntityChildEffect;

	const size_t sizeControlParts = sizeof(Library::Control::Animation::Parts) * CountParts;
	const size_t sizeControlPartsDraw = sizeof(Library::Control::Animation::Draw) * CountPartsDraw;
	const size_t sizeEntityChildInstance = sizeof(Entity) * CountEntityChildInstance;
	const size_t sizeEntityChildEffect = sizeof(EntityEffect) * CountEntityChildEffect;
	const size_t sizePrimitiveBatcher = (nullptr == EntityParent) ? sizeof(PrimitiveBatcher) : 0;	/* 自身が最親エンティティの時だけ確保 */

	size_t sizeWorkArea = Memory::Alignment(	sizeControlParts
												+ sizeControlPartsDraw
												+ sizeEntityChildInstance
												+ sizeEntityChildEffect
												+ sizePrimitiveBatcher
											);

	/* パーツ毎の追加ワークエリア長の取得 */
	size_t sizeWorkAreaAdditional = 0;
	for(int i=0; i<CountParts; i++)	{
		sizeWorkAreaAdditional += Library::Control::Animation::Parts::SizeGetWorkArea(*this, i);
	}

	/* ヒープ取得 */
	Memory::Allocate(&WorkAreaPack, sizeWorkArea + sizeWorkAreaAdditional);
	if(nullptr == WorkAreaPack)	{
		return(false);
	}
	Uint8* pointWorkAreaNow = reinterpret_cast<Uint8*>(WorkAreaPack);
	WorkAreaPackAdditional = reinterpret_cast<void*>(&pointWorkAreaNow[sizeWorkArea]);

	/* 各ワークへの領域割当 */
	TableControlParts = reinterpret_cast<Library::Control::Animation::Parts*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeControlParts;
	TableControlPartsDraw = reinterpret_cast<Library::Control::Animation::Draw*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeControlPartsDraw;
	ListEntityChildInstance = reinterpret_cast<Entity*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeEntityChildInstance;
	ListEntityChildEffect = reinterpret_cast<EntityEffect*>(pointWorkAreaNow);
		pointWorkAreaNow += sizeEntityChildEffect;

	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		ModelDraw = reinterpret_cast<PrimitiveBatcher*>(pointWorkAreaNow);
	}
	pointWorkAreaNow += sizePrimitiveBatcher;

	/* 各領域のクリア */
	/* MEMO: 実体配列を取得した領域は、念のため配置newで初期化しています。 */
	/*       ※仮想関数やコンストラクタ必須のメンバができた時のため。      */
	for(int i=0; i<CountParts; i++)	{
		new(&TableControlParts[i])	Library::Control::Animation::Parts;
	}
	for(int i=0; i<CountPartsDraw; i++)	{
		new(&TableControlPartsDraw[i])	Library::Control::Animation::Draw;
	}
	for(int i=0; i<CountEntityChildInstance; i++)	{
		new(&ListEntityChildInstance[i])	Entity;
	}
	for(int i=0; i<CountEntityChildEffect; i++)	{
		new(&ListEntityChildEffect[i])	EntityEffect;
	}

	/* 描画用モデルの設置 */
	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		/* MEMO: ここではクラスのコンストラクタだけ呼んでおきます（実際の初期起動は呼出元で）。 */
		new(ModelDraw) PrimitiveBatcher;
	}

	return(true);
}

/* ********************************************************* */
//! プロジェクト共通ワークエリア終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::ShutDownWorkAreaProject(void)
{
	int countTrack = CountTrack;
	int countTexture = CountTexture;

	/* 各領域のクリア */
	/* MEMO: 実体配列を取得した領域は、念のため明示デストラクトしておきます。 */
	/*       ※配置newの対として……。                                        */
	if(nullptr != TableInformationPlay)	{
		for(int i=0; i<countTrack; i++)	{
			TableInformationPlay[i].~InformationPlay();
		}
	}
	if(nullptr != TableControlTrack)	{
		for(int i=0; i<countTrack; i++)	{
			TableControlTrack[i].~Track();
		}
	}
	if(nullptr != Texture)	{
		for(int i=0; i<countTexture; i++)	{
			Texture[i] = nullptr;
		}
	}
	if(nullptr != IndexTextureCellMap)	{
		for(int i=0; i<countTexture; i++)	{
			IndexTextureCellMap[i] = -1;
		}
	}
	if(nullptr != CallBackPlayEndTrack)	{
		for(int i=0; i<countTrack; i++)	{
			CallBackPlayEndTrack[i].~CallBack();
		}
	}

	if(nullptr == EntityParent)	{	/* 親がいない */
		/* MEMO: シェーダ定数の実体を持っているのは最親のみなので、子が解放しないように注意してください。 */
		if(nullptr != ConstantShader)	{
			/* MEMO: シェーダ定数は終了まで行う。 */
			ConstantShader->ShutDown();
			ConstantShader->~ShaderData();
		}
	}
	ConstantShader = nullptr;

	/* MEMO: 色々なワーク配列などのクリアは末尾のCleanUpWorkAreaProjectで一括してやっています。 */

	/* ヒープ解放 */
	if(nullptr != WorkAreaProject)	{
		Memory::Free(WorkAreaProject);
	}

	/* 未使用化 */
	CleanUpWorkAreaProject();
}

/* ********************************************************* */
//! アニメーションパック依存ワークエリア終了
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::ShutDownWorkAreaPack(void)
{
	/* 描画用モデルの終了 */
	/* MEMO: ここではデストラクタを走らせるだけ（実際の終了は本関数を呼ぶ前に呼出元で）。 */
	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		if(nullptr != ModelDraw)	{
			ModelDraw->~PrimitiveBatcher();
		}
	}

	/* 各領域のクリア */
	/* MEMO: 実体配列を取得した領域は、念のため明示デストラクトしておきます。 */
	/*       ※配置newの対として……。                                        */
	if(nullptr != TableControlParts)	{
		for(int i=0; i<CountParts; i++)	{
			TableControlParts[i].~Parts();
		}
	}
	if(nullptr != TableControlPartsDraw)	{
		for(int i=0; i<CountPartsDraw; i++)	{
			TableControlPartsDraw[i].~Draw();
		}
	}
	if(nullptr != ListEntityChildInstance)	{
		for(int i=0; i<CountEntityChildInstance; i++)	{
			ListEntityChildInstance[i].~Entity();
		}
	}
	if(nullptr != ListEntityChildEffect)	{
		for(int i=0; i<CountEntityChildEffect; i++)	{
			ListEntityChildEffect[i].~EntityEffect();
		}
	}

	/* MEMO: 色々なワーク配列などのクリアは末尾のCleanUpWorkAreaProjectで一括してやっています。 */

	/* ヒープ解放 */
	/* MEMO: WorkAreaPackAdditionalはWorkAreaPackの中に含まれるので、明示的に解放しない。 */
	if(nullptr != WorkAreaPack)	{
		Memory::Free(WorkAreaPack);
	}

	/* 未使用化 */
	CleanUpWorkAreaPack();
}

/* ********************************************************* */
//! 本クラスが有効か？
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::IsValid(void)
{
	/* MEMO: BootUpとPackSetが行われているかをチェックしています。     */
	/*       ※PackGetを使用していますが、「nullptr != DataAnimation」 */
	/*        で判定しても、ほぼ条件等価です。                         */
	return((0 != (Status & FlagBitStatus::VALID)) && (0 <= PackGet()));	/* ? true : false */
}

/* ********************************************************* */
//! 本クラスの起動（初期起動）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::BootUp(Project& project, int countTrack, Entity* entityParent)
{
	if(false == project.IsValid())	{
		return(false);
	}

	/* 基底を起動 */
	base::BootUp();

	/* 親エンティティを設定 */
	/* MEMO: ワークエリアの初期化時にEntityParentを見ているので、先に設定して */
	/*       おかなくてはならないことに注意してください。                     */
	EntityParent = entityParent;

	/* プロジェクトの持つ情報を定義 */
	DataProject = &project;
	DataCellMap = &project.CellMap;

	/* コールバック群 */
	/* MEMO: トラック再生終了コールバックは、少し後のトラック制御の起動で一緒に設置しています。 */
	CallBackPlayEnd.CleanUp();
	CallBackUserData.CleanUp();

	/* ワークエリアの確保・設定 */
	/* MEMO: CountTrack・CountTexture・CountTextureOffscreenも、本関数の中で設定されて帰ってきます。 */
	if(false == BootUpWorkAreaProject(	((0 >= countTrack) ? LimitTrack : countTrack),	/* トラック数 */
										DataProject->CountGetTexture(),					/* テクスチャ数 */
										Defaults::COUNT_TEXTURE_OFFSCREEN				/* オフスクリーンテクスチャ数 */
									)
	)	{
		goto BootUp_ErrorEnd;
	}

	/* 再生トラック制御の起動 */
	/* MEMO: ここでやることはない。 */

	/* 使用テクスチャ群・セルマップとテクスチャ変換テーブルの設定 */
	if(false == DataProject->TextureBootUp(Texture, IndexTextureCellMap))	{
		goto BootUp_ErrorEnd;
	}

	/* オフスクリーンテクスチャの設定 */
	/* MEMO: ここでやることはない。 */

	/* 現在のアニメーションパックの解放 */
	PackRelease();

	/* 初期化終了 */
	Status |= FlagBitStatus::VALID;

	return(true);

BootUp_ErrorEnd:;
	ShutDown();

	return(false);
}

/* ********************************************************* */
//! 本クラスの終了（未使用化）
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
void Entity::ShutDown(void)
{
	/* パーツを解放 */
	/* MEMO: 本クラスに持っているワークの実体を解放するより前に、本処理を行ってください。 */
	/*       ※本処理でパーツワークが使用している描画制御・子エンティティなども終了します */
	/*         ので、本処理の後は領域は無造作に解放して構いません。                       */
	PackRelease();

	/* ワークエリアの解放 */
	ShutDownWorkAreaProject();

	/* 基底を終了 */
	base::ShutDown();

	CleanUp();
}

/* ********************************************************* */
//! 再生状態の定常更新
/* --------------------------------------------------------- */
/* MEMO:
 * 3つの行列については、UpdateではなくDrawで受け取るようにした方が
 * 良いかもしれない（描画だけを複数回回すとかできるので）。
 */
bool Entity::Update(float timeElapsed, Library::KindMasking::Kind masking)
{
	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		/* MEMO: 子オブジェクト（インスタンス・エフェクト）は外部からのUpdateでは更新しません。 */
		/*       最親のUpdate処理の中からUpdateMainが呼び出されます。                           */
		/*       ※親と子のフレーム同期ずれを最小限に抑えるためです。                           */

		/* 描画用情報コンテナの初期化 */
		/* MEMO: 毎ループ内容はクリアして使用します。 */
		ArgumentContainerControl.CleanUp();

		/* アニメーション更新＋描画モデル作成 */
		UpdateMain(	timeElapsed,
					false,
					Library::KindMasking::FOLLOW_DATA,
					ArgumentContainerControl,
					true,
					FlagPlanarization,
					FPU::Matrix4x4::Unit	/* 最親は単位行列 */
				);
	}

	return(true);
}
bool Entity::UpdateMain(	float timeElapsed,
							bool flagHideDefault,
							Library::KindMasking::Kind masking,
							Library::Control::Animation::ArgumentContainer& argumentUpdate,
							bool flagInitializeMatrixCorrection,
							bool flagPlanarization,
							const FPU::Matrix4x4& matrixParent
					)
{	/* MEMO: 更新の主要処理はUpdateMainに記載されています。                                     */
	/*       Updateはアニメーションオブジェクトを更新するための（メインプロセスからの）関数で、 */
	/*       UpdateMainは子オブジェクトを親オブジェクトから更新するための関数です。             */
	/* MEMO: Library::Control::Animation::Partsの更新プロセスは下記に分割されています。 */
	/*       - Update～ : 各パーツのアニメーション情報更新（アトリビュートの読解）      */
	/*       - Draw～   : 描画パーツのアニメーション情報の描画用計算                    */
	/*       - Render～ : Drawで作成した計算結果を実際にGPUに送る（描画コマンドの作成） */
	/*                                                                                  */
	/*       その内、本Updateで行うのは、UpdateとDrawです（Render～は、別スレッド動作が */
	/*       基本になるため、本クラスのDraw関数内で行われます）。                       */
	/*       その上で、Drawという単語もいくつか使われていますので、名称が混乱しがちでは */
	/*       あるので注意してください。                                                 */
	/*       ※さらに、処理の最適化や描画情報の最終決定の都合などから、「インスタンス」 */
	/*        「エフェクト」などの子オブジェクト群の本クラスのUpdateは、上記のパーツ制  */
	/*         御（Library::Control::Animation::Parts）のDraw～の中から呼ばれていますの */
	/*         で、その点にも注意が必要です。                                           */
	if(false == IsValid())	{	/* アニメーション再生できる状態でない（未初期化状態） */
		return(false);
	}
	if(false == DataAnimation->IsValid())	{	/* アニメーションデータが異常 */
		return(false);
	}
	if(false == DataCellMap->IsValid())	{	/* セルマップデータが異常 */
		return(false);
	}

	/* 基底を更新 */
	base::Update();

	/* 基本の非表示状態を取得 */
	bool flagHide = flagHideDefault | FlagHideForce;

	/* 描画パーツリストのクリア */
	ListPartsPreDraw.clear();
	ListPartsDraw.clear();

	/* 再生トラックを更新 */
	int countControlTrack = CountTrack;
	for(int i=0; i<countControlTrack; i++)	{
		TableControlTrack[i].Update(timeElapsed);
	}

	/* パーツを更新（再生フレームのアニメーションデータへの情報更新） */
	Status &= ~Entity::FlagBitStatus::ANIMATION_SYNTHESIZE;
	int countControlParts = CountParts;
	int indexTrackRoot = TableControlParts[0].IndexControlTrack;	/* (0 < countControlParts) ? TableControlParts[0].IndexControlTrack : -1; */
	for(int i=0; i<countControlParts; i++)	{
		TableControlParts[i].Update(	*this,
										i,
										argumentUpdate,
										flagHide,
										indexTrackRoot,
										matrixParent
								);
	}
	bool flagAnimationSynthesize = (0 != (Status & FlagBitStatus::ANIMATION_SYNTHESIZE));	/* ? true : false */

	/* 描画モデル（バッチングメッシュ）のクリア */
	/* MEMO: 描画モデルの直接管理は最親エンティティしか行いません。 */
	if(nullptr == EntityParent)	{	/* 最親エンティティ */
		/* モデルを更新開始 */
		/* MEMO: PrimitiveBeginの中で描画頂点の消去が行われています。 */
		ModelDraw->PrimitiveBegin();
	}

	/* 描画パーツの処理 */
	int idPartsDrawNext;
	if(true == flagAnimationSynthesize)	{	/* アニメーション合成が行われている */
		int countChainDraw;

		/* 描画パーツの順序決定 */
		/* MEMO: アニメーション合成を行っている場合、SS6ConverterVer2の事前計算で求めた描画チェインデータが */
		/*       使用できない（複数のアニメーション内の優先度適用が混じる）ため、リアルタイムでソートしてい */
		/*       ます（アニメーション合成しない場合は、事前計算データを使用します）。                       */
		/* MEMO: 現時点ではソートのキーは衝突しない（優先度とパーツ番号の合成なので同じ値が成立しない）ため同着を */
		/*       考慮していません（マージ:stable_sortではなくクイック:sortしています）。                          */
		/* MEMO: 描画前描画（ListPartsPreDraw）は描画（ListPartsDraw）とは描画順が逆になるため、 */
		/*       降順ソートしなくてはならないように思えるが、ListPartsPreDrawはキーの符号を逆転  */
		/*       させてあるため（通常のソートでそのまま）逆転するようになっています。            */
		std::sort(ListPartsPreDraw.begin(), ListPartsPreDraw.end());
		std::sort(ListPartsDraw.begin(), ListPartsDraw.end());

		if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
			/* 描画前描画用の描画情報計算 */
			/* MEMO: 描画前描画はマスクが効果する「最親のアニメーションオブジェクト」でしか */
			/*       作成しません（インスタンスオブジェクトのアニメーションは無視されるのが */
			/*       仕様です）。                                                           */
			countChainDraw = (int)(ListPartsPreDraw.size());
			for(int i=0; i<countChainDraw; i++)	{
				idPartsDrawNext = ListPartsPreDraw[i] & Library::Control::Animation::MaskSortKeyIDParts;
				if(true == TableControlParts[idPartsDrawNext].PreDraw(	*this,
																		idPartsDrawNext,
																		argumentUpdate,
																		flagHide,
																		flagPlanarization,
																		masking
																	)
				)	{	/* 描画する */
					/* MEMO: 現在は何も追加処理を行わない。 */
				}
			}
		}

		/* 描画用の描画情報計算 */
		countChainDraw = (int)(ListPartsDraw.size());
		for(int i=0; i<countChainDraw; i++)	{
			idPartsDrawNext = ListPartsDraw[i] & Library::Control::Animation::MaskSortKeyIDParts;
			if(true == TableControlParts[idPartsDrawNext].Draw(	*this,
																idPartsDrawNext,
																argumentUpdate,
																flagHide,
																flagPlanarization,
																masking
															)
			)	{	/* 描画する */
				/* MEMO: 現在は何も追加処理を行わない。 */
			}
		}
	} else {	/* アニメーション合成をしていない（事前計算順序を使用） */
		if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
			/* 描画前描画用の描画情報計算 */
			/* MEMO: 描画前描画はマスクが効果する「最親のアニメーションオブジェクト」でしか */
			/*       作成しません（インスタンスオブジェクトのアニメーションは無視されるのが */
			/*       仕様です）。                                                           */
			idPartsDrawNext = TableControlParts[0].IDPartsNextPreDraw();
			while(0 <= idPartsDrawNext)	{
				if(true == TableControlParts[idPartsDrawNext].PreDraw(	*this,
																		idPartsDrawNext,
																		argumentUpdate,
																		flagHide,
																		flagPlanarization,
																		masking
																	)
				)	{	/* 描画する */
					/* MEMO: 現在は何も追加処理を行わない。 */
				}
				idPartsDrawNext = TableControlParts[idPartsDrawNext].IDPartsNextPreDraw();
			}
		}

		/* カメラの決定 */
		/* MEMO: 現在、インスタンス含めた全ての描画パーツの更新が終わった後、カメラの行列を決定している。 */
		/*       この方法だと、実描画（Render）の中でこの行列を全頂点に乗算しなくてはならないために、CPU  */
		/*       的に無駄が大きい。                                                                       */
		/*       ただ、もしこれを定常更新（Draw）の中での行列計算でまとめて求めようとすると定常更新の中の */
		/*       UpdateとDrawを分けて・インスタンスの更新もUpdateとDrawとで処理を分けたりなど、大工事にな */
		/*       るため、現在暫定でこの処理で行っている（計算をシェーダに任せるという手もありますし）。   */
		if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
			if(nullptr != argumentUpdate.MatrixTransformPartsCamera)	{	/* カメラに使用する行列が指定されている */
				/* カメラ行列の逆行列を求める */
				/* MEMO: 現在逆行列を求めるのに正直に求めていますが……ここで使用される行列は回転・スケール部分（各軸ベクトル） */
				/*       は直行行列であるため、各軸ベクトル部を転置して・平行移動部分（原点位置）を符号逆転しても求められる。   */
				MatrixInverse(&argumentUpdate.MatrixCamera, *argumentUpdate.MatrixTransformPartsCamera);
			}
		}

		/* 描画用の描画情報計算 */
		idPartsDrawNext = TableControlParts[0].IDPartsNextDraw();
		while(0 <= idPartsDrawNext)	{
			if(true == TableControlParts[idPartsDrawNext].Draw(	*this,
																idPartsDrawNext,
																argumentUpdate,
																flagHide,
																flagPlanarization,
																masking
															)
			)	{	/* 描画する */
				/* MEMO: 現在は何も追加処理を行わない。 */
			}
			idPartsDrawNext = TableControlParts[idPartsDrawNext].IDPartsNextDraw();
		}
	}

	/* 描画モデル（バッチングメッシュ）の更新終了 */
	/* MEMO: 描画モデルの直接管理は最親エンティティしか行いません。 */
	if(nullptr == EntityParent)	{	/* 最親エンティティ */
		/* モデルを更新終了 */
		ModelDraw->PrimitiveEnd();
	}

	/* 再生トラック再生終了判定 */
	bool flagStopAllTrack = true;
	{
		int indexTrackSlave = -1;
		bool flagDecodeNextForce = false;
		bool flagRequestPlayEndTrack;
		int indexAnimation;
		for(int i=0; i<countControlTrack; i++)	{
			if(true == TableControlTrack[i].StatusGetIsPlaying())	{
				flagRequestPlayEndTrack = TableControlTrack[i].StatusGetIsRequestPlayEnd();
				indexAnimation = TableControlTrack[i].ArgumentContainer.IndexAnimation;

				/* Check Transition-End */
				flagDecodeNextForce = false;
				if(true == TableControlTrack[i].StatusGetIsRequestTransitionEnd())	{
					indexTrackSlave = TableControlTrack[i].IndexTrackSlave;
					if(0 <= indexTrackSlave)	{
						/* スレーブ再生トラックの状態をマスタ再生トラックに移行 */
						flagRequestPlayEndTrack = TrackChangeSlaveToMaster(i, indexTrackSlave);

						/* トランジション終了のコールバック発行 */
						if(true == CallBackPlayEndTrack[i].IsValid())	{
							CallBackPlayEndTrack[i].Function(	*this,
																CallBackPlayEndTrack[i].Pointer,
																i,
																indexTrackSlave,
																indexAnimation,
																TableControlTrack[i].ArgumentContainer.IndexAnimation
														);
						}
						flagDecodeNextForce = true;
					}
				}

				/* トラック再生終了をチェック */
				if(true == flagRequestPlayEndTrack)	{
					/* トラック再生を終了 */
					TableControlTrack[i].Stop();

					/* トラック再生終了をコールバック発行 */
					/* MEMO: トラックの再生終了の場合は、indexTrackSlaveは常に-1（なし）です。 */
					if(true == CallBackPlayEndTrack[i].IsValid())	{
						/* コールバック発行 */
						CallBackPlayEndTrack[i].Function(	*this,
															CallBackPlayEndTrack[i].Pointer,
															i,
															-1,
															indexAnimation,
															-1
													);
					}
				}
				else
				{
					flagStopAllTrack = false;
				}

				TableControlTrack[i].Status &= ~(	Library::Control::Animation::Track::FlagBitStatus::REQUEST_PLAYEND
													| Library::Control::Animation::Track::FlagBitStatus::REQUEST_TRANSITIONEND
												);
			}

			/* フレーム内ステータスのクリア */
			TableControlTrack[i].StatusClearTransient(flagDecodeNextForce);
			/* MEMO: この処理、StatusClearTransientに混ぜても良いのでは？ */
			if(true == flagDecodeNextForce)	{
				TableControlTrack[i].Status |= Library::Control::Animation::Track::FlagBitStatus::DECODE_ATTRIBUTE;
			}

			TableControlTrack[i].TimeElapseReplacement = 0.0f;

		}
	}

	/* 一時ステータスの調整 */
	Status &= ~(	FlagBitStatus::UPDATE_RATE_SCALELOCAL
					| FlagBitStatus::UPDATE_RATE_OPACITY
					| FlagBitStatus::CHANGE_TABLEMATERIAL
					| FlagBitStatus::CHANGE_CELLMAP
				);
	if(nullptr != AdditionalColor)	{
		AdditionalColor->Status &= ~Library::Control::AdditionalColor::FlagBitStatus::CHANGE;
	}

	/* 再生終了判定 */
	if(true == flagStopAllTrack)	{
		if(0 != (Status & FlagBitStatus::PLAYING))	{	/* 再生状態から全トラック停止状態になった */
			if(true == CallBackPlayEnd.IsValid())	{	/* コールバック関数が定義されている */
				/* コールバック発行 */
				CallBackPlayEnd.Function(*this, CallBackPlayEnd.Pointer);
			}
		}

		Status &= ~FlagBitStatus::PLAYING;
	} else {
		Status |= FlagBitStatus::PLAYING;
	}

	return(true);
}
bool Entity::TrackChangeSlaveToMaster(int indexTrackMaskter, int indexTrackSlave)
{
	bool flagStartAnimation = TableControlTrack[indexTrackMaskter].StatusGetIsTransitionCancelPause();
	bool flagRequestPlayEndTrack = TableControlTrack[indexTrackSlave].StatusGetIsRequestPlayEnd();

	/* Copy Track playing datas */
	/* トラックの再生情報をコピー */
	/* MEMO: トラック再生制御は、原則的に再生フレームのみを管理するので、（手抜きして）全情報をコピーして状態を全部引き継いでいます。 */
	TableControlTrack[indexTrackMaskter] = TableControlTrack[indexTrackSlave];

	/* パーツのTRS情報をコピー */
	/* MEMO: トラックトランジション終了時のデコード状態は（パーツ管理の）TRSSlaveにあるため、TRSSlaveをTRSMasterにコピーします。 */
	int countControlParts = CountGetParts();
	for(int i=0; i<countControlParts; i++)	{
		if(TableControlParts[i].IndexControlTrack == indexTrackMaskter)	{	/* マスタ再生トラックで再生されている */
			TableControlParts[i].TRSMaster = TableControlParts[i].TRSSlave;
			/* MEMO: 全てのアトリビュートを（次のUpdate）で再デコードしないと、 */
			/*       マスタとスレーブの遷移状態が混ざってしまうので注意。       */
			TableControlParts[i].CacheClearAttribute(*this, false);
		}
	}

	/* トラックトランジション情報をクリア */
	TableControlTrack[indexTrackMaskter].Status &= ~(Library::Control::Animation::Track::FlagBitStatus::REQUEST_TRANSITIONEND
														| Library::Control::Animation::Track::FlagBitStatus::REQUEST_TRANSITIONEND
														| Library::Control::Animation::Track::FlagBitStatus::TRANSITION_CANCEL_PAUSE
													);
	TableControlTrack[indexTrackMaskter].Transition(-1, 0.0f);

	/* 一時停止状態を解除 */
	if(true == flagStartAnimation)	{	/* 一時停止解除要求あり */
		TableControlTrack[indexTrackMaskter].Pause(false);
	}

	/* スレーブを止める */
	TableControlTrack[indexTrackSlave].Stop();

	return(flagRequestPlayEndTrack);
}

/* ********************************************************* */
//! 描画関数
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
TypeDrawCommandList Entity::Draw(	TypeDrawCommandList commandList,
									const CPU::Matrix4x4& matrixProjection,
									const CPU::Matrix4x4& matrixView,
									const CPU::Matrix4x4& matrixWorld
							)
{
//	if(false == DataCellMap->IsValid())	{	/* セルマップデータが異常 */
//		return(commandList);
//	}
	if(false == IsValid())	{	/* アニメーション再生できる状態でない（未初期化状態） */
		return(commandList);
	}

	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		if(false == FlagHideForce)	{	/* 非表示指定でない */
			/* レンダラをリセット */
			Renderer& renderer = Renderer::Instance;
			renderer.Refresh();

			/* 描画用シェーダ定数を設定 */
			/* MEMO: シェーダ定数バッファは基本的に最親エンティティにのみ存在し、子エ */
			/*       ンティティ群は、最親のものを共用しています。                     */
			/*       ※現時点では全スプライトで共通のシェーダ定数を使用しています。   */
			/*       ※逆に言うと、子アニメーション（インスタンス）であっても、シェー */
			/*         ダ定数はエンティティの中で閉じているので、エンティティである限 */
			/*         りシェーダ定数は個別で持つ必要があります（現状）。             */
			/*       ※パーツ毎のシェーダ定数的なものは、頂点データに入れています。   */
			/*         パーツ毎でシェーダ定数を切り替えるとGPUのキャッシュフラッシュ  */
			/*         が起こってスタック期間が生じるプラットフォームなどがあるので、 */
			/*         それを回避するために頂点データに編入しています。               */
			/* MEMO: 本来はnullptrチェックをすべきですが、IsValidがtrueの時（BootUpが */
			/*       終了している時）は必ず存在しているため、省略しています。         */
//			if(nullptr != ConstantShader)	{
				ConstantShader->MatrixSetProjection(matrixProjection);
				ConstantShader->MatrixSetView(matrixView);
				ConstantShader->MatrixSetWorld(matrixWorld);
				ConstantShader->DataSet();
//			}

			/* バッチングされたプリミティブ群を描画 */
			/* MEMO: 子エンティティ（インスタンス・エフェクトなど）は最親エンティティの描画モデルに集積 */
			/*       されているので、最親エンティティしか描画処理をしないことに注意してください。       */
			TypeDrawCommandList commandListNew = ModelDraw->Draw(commandList, *ConstantShader);
			if(DrawCommandListInvalid == commandListNew)	{	/* 失敗 */
				return(DrawCommandListInvalid);
			}
			commandList = commandListNew;
		}
	}

	return(commandList);
}

/* ********************************************************* */
//! アニメーションパックを設定
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
bool Entity::PackSet(int index)
{
	if(false == DataProject->IsValid())	{
		goto PackSet_ErrorEnd;
	}
	if(DataProject->Data.CountDataAnimation() <= index)	{
		goto PackSet_ErrorEnd;
	}

	if(IndexPack == index)	{	/* 今と同じパック */
		return(true);
	}

	/* 現状のパーツ制御・描画制御群を解放 */
	/* MEMO: トラック・コールバック・テクスチャなどは解放・破棄したらだめです。 */
	PackRelease();

	/* アニメーションパック依存処理の起動 */
	if(0 <= index)	{
		/* アニメーション情報を取得 */
		IndexPack = index;
		DataAnimation = &DataProject->Animation[index];
		if(false == DataAnimation->IsValid())	{
			goto PackSet_ErrorEnd;
		}

		/* パーツカタログを取得 */
		/* MEMO: パーツカタログは初期化内で多用するので、関数内ローカルで取得しておきます。 */
		Library::Data::Parts::Animation::Catalog catalogParts;
		DataAnimation->CatalogParts(&catalogParts);

		/* ワークエリアの確保・設定 */
		if(false == BootUpWorkAreaPack())	{
			goto PackSet_ErrorEnd;
		}

		/* パーツ制御の起動 */
		{
			/* MEMO: pointWorkAreaAdditionalNowはParts::BootUp内で更新されて帰ってきます。 */
			void* pointWorkAreaAdditionalNow = WorkAreaPackAdditional;
			int countDraw = 0;
			int countEntityInstance = 0;
			int countEntityEffect = 0;
			for(int i=0; i<CountParts; i++)	{
				if(false == TableControlParts[i].BootUp(*this, i, countDraw, countEntityInstance, countEntityEffect, pointWorkAreaAdditionalNow))	{
					goto PackSet_ErrorEnd;
				}
			}
		}

		/* 描画リストの起動 */
		{
			int countParts = DataAnimation->CountTableParts();

			/* 再配置領域を確保 */
			/* MEMO: 描画リスト関連は最大記録時が高々パーツ数分なので、あらかじめ領域を確保して */
			/*       再配置が起こらないようにしています。                                       */
			/*       ※ギリギリで設定していますが、再配置が起こってしまう場合にはパーツ数の2倍  */
			/*         程度確保してしまっても大丈夫だと思われます。                             */
			/* MEMO: 現在乱暴に取ってしまっていますが、厳密にはPreDrawはマスクパーツ数・Drawは  */
			/*       描画対象パーツ数だけあれば大丈夫です。                                     */
			ListPartsDraw.reserve(countParts);
			ListPartsDraw.clear();

			ListPartsPreDraw.reserve(countParts);
			ListPartsPreDraw.clear();
		}
	}

	/* 描画モデルの初期起動 */
	if(false == ModelSetDraw())	{	/* 失敗 */
		goto PackSet_ErrorEnd;
	}

	return(true);

PackSet_ErrorEnd:;
	return(false);
}
bool Entity::ModelSetDraw(void)
{
	/* MEMO: 内部処理でインスタンスやエフェクトを変更した場合に描画モデルの再起動（頂点バッファなどの再配置）が */
	/*       必要になるために、関数化しています。                                                               */
	/* 描画用モデルの初期起動 */
	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		/* 使用最大頂点数を取得 */
		int countMaterialMax;
		int countVertexMax = CountGetVertexDraw(&countMaterialMax, IndexPack);
		if(0 > countVertexMax)	{	/* エラー */
			return(false);
		}

		/* 現在の描画モデルの状態と比較 */
		/* MEMO: 描画モデルが既存の場合、最大フラグメント数と最大頂点数が要求スペックを満たすなら、既得バッファは流用する。 */
		bool flagBootUp = true;
		if(nullptr != ModelDraw)	{	/* 描画モデルが既存 */
			if(true == ModelDraw->IsValid())	{	/* 描画モデルが有効 */
				if((ModelDraw->CountVertexMax < countVertexMax) || (ModelDraw->CountFragmentMax < countMaterialMax))	{	/* 既存のバッファで事足りる */
					flagBootUp = false;
				}
			}
		}

		/* 描画モデルを初期起動 */
		if(true == flagBootUp)	{
			if(false == ModelDraw->BootUp(countMaterialMax, countVertexMax))	{
				return(false);
			}
		}
	} else {
		ModelDraw = EntityParent->ModelDraw;
	}

	return(true);
}

/* ********************************************************* */
//! アニメーションパックを取得
/* --------------------------------------------------------- */
/* MEMO:
 * 
 */
// int Entity::PackGet(void)

/* ********************************************************* */
//! アニメーションパックの解放
/* --------------------------------------------------------- */
/* MEMO:
 * どちらかというと役割は（アニメーションパック毎に構成が決定されている）パーツのワークを解放することです。
 */
void Entity::PackRelease(void)
{
	/* 描画用モデルの終了 */
	if(nullptr == EntityParent)	{	/* 自身が最親エンティティ */
		if(nullptr != ModelDraw)	{
			ModelDraw->ShutDown();
		}
	}

	/* 描画リストのクリア */
	ListPartsDraw.clear();
	ListPartsPreDraw.clear();

	/* パーツ制御の解放 */
	if(nullptr != TableControlParts)	{
		for(int i=0; i<CountParts; i++)	{
			TableControlParts[i].ShutDown(*this, i);
		}
	}

	/* ワークエリアの解放 */
	ShutDownWorkAreaPack();
}

/* ********************************************************* */
//! アニメーションパックで使用する最大描画頂点数を取得
/* --------------------------------------------------------- */
/* MEMO:
 * インスタンスやエフェクトについては、すでに対象となるエンティ
 * ティやプロジェクト・アニメーションパックなどが設定されている
 * 必要があります。
 */
int Entity::CountGetVertexDraw(int* countFragmentMax, int index)
{
	int countVertexDraw = 0;
	int count;
	int countVertexPrimitive;

	*countFragmentMax = 0;	/* 初期化 */

	if((nullptr == DataProject) || (nullptr == DataAnimation))	{	/* プロジェクトかアニメーションパックが設定されていない */
		return(-1);
	}

	/* パーツカタログを取得・種類別パーツを走査して頂点数を算出 */
	Library::Data::Parts::Animation::Catalog catalogParts;
	DataAnimation->CatalogParts(&catalogParts);
	{	/* MEMO: 通常パーツは4三角形分割です。 */
		count = catalogParts.CountTableIDPartsNormal();
		if(0 < count)	{
			countVertexPrimitive = PrimitiveBatcher::CountGetVertex(Library::KindVertex::TERMINATOR4);
			countVertexDraw += countVertexPrimitive * count;
			*countFragmentMax += count;
		}
	}
	{	/* MEMO: マスクパーツは4三角形分割です。 */
		/* MEMO: マスクパーツは、描画前描画（ステンシル初期化用）と本描画があるので2倍の頂点数とマテリアル（フラグメント）数が必要です。 */
		count = catalogParts.CountTableIDPartsMask();
		if(0 < count)	{
			countVertexPrimitive = PrimitiveBatcher::CountGetVertex(Library::KindVertex::TERMINATOR4);
			countVertexDraw += (countVertexPrimitive * count) * 2;
			*countFragmentMax += count * 2;
		}
	}
	{	/* MEMO: メッシュパーツはメッシュバインド情報内のインデックステーブル長から算出。 */
		count = catalogParts.CountTableIDPartsMesh();
		if(0 < count)	{
			const Uint16* tableID = catalogParts.TableIDPartsMesh();

			Library::Data::Parts::Animation dataPartsAnimation;
			Library::Data::Parts::Animation::BindMesh dataBindMesh;
			for(int i=0; i<count; i++)	{
				/* メッシュバインド情報を取得 */
				DataAnimation->TableParts(&dataPartsAnimation, (int)((Uint32)tableID[i]));
				dataPartsAnimation.Mesh(&dataBindMesh);

				/* MEMO: 頂点インデックスは3頂点1組で並んでいるので、そのまま描画頂点数と同じです。 */
				countVertexDraw += dataBindMesh.CountTableIndexVertex();
				*countFragmentMax += 1;
			}
		}
	}
	{	/* MEMO: インスタンスは子アニメーションから値を受け取ります。 */
		/* MEMO: この処理があるので、アニメーションパック初期化時の手順には注意してください。 */
		/*       ※インスタンスのEntityがBootUp＆PackSetされていないと正常に動作しません。    */
		count = catalogParts.CountTableIDPartsInstance();
		if(0 < count)	{
			const Uint16* tableID = catalogParts.TableIDPartsInstance();

			int indexAnimation;
			int indexEntity;
			int countFragmentChild;
			Library::Control::Animation::Parts* controlParts;
			for(int i=0; i<count; i++)	{
				controlParts = &TableControlParts[(int)((Uint32)tableID[i])];

				/* 再生アニメーション番号を取得 */
				indexAnimation = controlParts->IndexAnimationUnderControl;
				indexEntity = controlParts->IndexEntityUnderControl;

				/* 子エンティティから値を取得 */
				countVertexPrimitive = ListEntityChildInstance[indexEntity].CountGetVertexDraw(&countFragmentChild, indexAnimation);
				if(0 > countVertexPrimitive)	{	/* エラー */
					return(-1);
				}

				/* MEMO: 子エンティティが算出した値をそのまま足しています。 */
				countVertexDraw += countVertexPrimitive;
				*countFragmentMax += countFragmentChild;
				*countFragmentMax += 1;	/* インスタンスパーツにも1つ描画情報が必要なので……念のため（実はいらない）。 */
			}
		}
	}
	{	/* MEMO: エフェクトは子アニメーションから値を受け取ります。 */
		/* MEMO: エフェクトにはアニメーション番号はありません。 */
		count = catalogParts.CountTableIDPartsEffect();
		if(0 < count)	{
			const Uint16* tableID = catalogParts.TableIDPartsEffect();

			int indexEntity;
			int countFragmentChild;
			Library::Control::Animation::Parts* controlParts;
			for(int i=0; i<count; i++)	{
				controlParts = &TableControlParts[(int)((Uint32)tableID[i])];

				/* 再生アニメーション番号を取得 */
				indexEntity = controlParts->IndexEntityUnderControl;

				/* 子エンティティから値を取得 */
				countVertexPrimitive = ListEntityChildEffect[indexEntity].CountGetVertexDraw(&countFragmentChild);
				if(0 > countVertexPrimitive)	{	/* エラー */
					return(-1);
				}

				/* MEMO: 子エンティティが算出した値をそのまま足しています。 */
				countVertexDraw += countVertexPrimitive;
				*countFragmentMax += countFragmentChild;
				*countFragmentMax += 1;	/* エフェクトパーツにも1つ描画情報が必要なので……念のため（実はいらない）。 */
			}
		}
	}

	return(countVertexDraw);
}

/* ********************************************************* */
//! ローカルスケール係数を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 直接RateScaleLocalForceを操作しないでください。
 */
void Entity::RateSetScaleLocal(float scaleX, float scaleY)
{
	RateScaleLocalForce.Set(scaleX, scaleY);
	Status |= FlagBitStatus::UPDATE_RATE_SCALELOCAL;
}

/* ********************************************************* */
//! ローカルスケール係数を設定
/* --------------------------------------------------------- */
/* MEMO:
 * 直接RateOpacityForceを操作しないでください。
 */
void Entity::RateSetOpacity(float rate)
{
	RateOpacityForce = rate;
	Status |= FlagBitStatus::UPDATE_RATE_OPACITY;
}

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Texture [static] */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/* -------------------------------------------------------------------------- */

}	/* SpriteStudio6 */
