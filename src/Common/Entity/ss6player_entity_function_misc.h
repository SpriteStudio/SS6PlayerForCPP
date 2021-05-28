/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO: 【注意!!】                                                           */
/* 本ヘッダはentity.hの一部です（その他・未分類関数群）。                     */
/* 単独でincludeしないでください（一応コンパイルエラー防護はしているものの、  */
/* 内容が正常に適用されません）。                                             */
/* ※Entityクラスの機能が大きい巨大なため、各区分の機能を見通しやすくするため */
/*   に（C#のpartial的に）ファイルを分割してあるだけです。                    */
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#if defined(__SS6PLAYER_ENTITY_CLASS__)

/* -------------------------------------------------------------------------- */
/*                                                  Class Functions (Partial) */
/* -------------------------------------------------------------------------- */
// namespace SpriteStudio6	{
// class Entity	{
#if 0	/* MEMO: ここではいらない（親のentity.hでincludeしているため）が、メモとして残してあります。 */
/* 前方宣言 */
class EntityEffect;
#endif

	/* ----------------------------------------------- Functions */
private:
	void AdditionalColorSet(Library::Control::AdditionalColor* additionalColorNew, Library::Control::AdditionalColor* additionalColorCheck);
	bool RefreshCancelTransformMain(int idParts, bool flagPosition, bool flagRotation, bool flagScaling);
	bool AnimationChangeInstanceMain(	Library::Control::Animation::Parts* controlParts,
										Entity* entityChild,
										int indexAnimation,
										Library::KindIgnoreAttribute::Kind ignoreAttribute,
										bool flagStartImmediate,
										int timesPlay,
										float rateTime,
										Library::KindStylePlay::Kind style,
										const char* labelRangeStart,
										int frameRangeOffsetStart,
										const char* labelRangeEnd,
										int frameRangeOffsetEnd
								);

protected:
public:
	/* ********************************************************* */
	//! エンティティが使用可能かの状態を取得
	/*!
	@param	なし

	@retval	関数値
		true == BootUpが終了している（使用可能）
		false == BootUpされていない（or 失敗）

	エンティティがBootUpして使用可能な状態にあるかを取得します。
	ただし、本関数はBootUpが終了しているかだけをチェックしますので、
		PackSetの実行後か（アニメーションデータの設定も終わって再生
		可能な状態にあるか）を取得したい場合は、IsValid関数を使用
		してください。
	※つまり、本関数は「PackSet」を実行することが可能であるかの
		取得に使用します。
	*/
	bool StatustGetValidBootUp(void);

	/* ********************************************************* */
	//! 動的パーツカラーパラメータの生成
	/*!
	@param	flagInvolveChildren
		true == 子エンティティ群も同じパラメータを使用する
		false == 本パラメータで操作できるのは本エンティティのみ
		default: true

	@retval	関数値
		動的パーツカラーのポインタ

	動的パーツカラーのバッファを作成して、そのポインタを返します。
	原則として、動的パーツカラーは、各アニメーションエンティティ毎
		に1つのみ持つことが可能で・エンティティの種類によって挙動が
		若干異なります（ただし、本関数のflagInvolveChildrenにtrueを
		与えた場合、子エンティティ群とパラメータを共有するような
		設定を行います）。

	- Entityクラス（アニメーション）
		アニメーションが持つすべてのパーツに本値がパーツカラーとして
			適用されます。
		アニメーションデータの「パーツカラー」を持っているパーツの場
			合には、そのデータへの上書き（差し替え）で適用されます。
		データと本値が同時に効果するわけではありませんので、注意して
		ください。
	- EntityEffectクラス（エフェクト）
		エフェクトが発生させている全てのパーティクルに本値が乗算適用
			されます（つまり、「なし」か「乗算」しか有効ではありませ
			ん）。
		また、色の設定は頂点毎には行えず・SetOverallのみでの設定にな
			ります（SetVertexで設定した場合、左上の色が適用されます）。
		※本機能については、現状非対応です。

	動的パーツカラーが必要なくなった場合、AdditionalColorReleaseで
		パラメータを解放してください。
	一時的に使用しない場合はAdditionalColorReleaseではなく、本パラ
		メータの合成方法にNONを与えて設定することで一時的に動的パー
		ツカラーの効果を無効にできます。

	動的パーツカラーを使い続ける限り、本関数が返すパラメータは静的に
		確保されており、ポインタが変わることはありません。

	本関数は複数回アクセスしても、（作成済かつ解放されていなければ）
		同じパラメータ（ポインタ）を返します。
	*/
	Library::Control::AdditionalColor* AdditionalColorCreate(bool flagInvolveChildren=true);

	/* ********************************************************* */
	//! 動的パーツカラーパラメータの解放
	/*!
	@param	なし

	@retval	なし

	AddtionalColorCreateで取得したパラメータを解放（破棄）します。
	子エンティティ群で同じパラメータを共用しているものも同時に
		解放します。
	必ず、AdditionalColorCreateを行ったエンティティから行ってくだ
		さい（エンティティが異なる場合、メモリの解放ミスを起こし
		ますので、最悪例外やハングアップなどの障害が出る可能性が
		あります）。

	AdditionalColorCreateで最初に作成したパラメータは、最終的に
		本関数で解放しないと、メモリリークを起こしますので注意して
		ください。
	※解放タイミングについては、原則的にPackSet(nullptr)の前です。
		ただし、AdditionalColorCreate(true)で取得した場合には、
		最親エンティティのShutDown()前に廃棄されていれば問題あり
		ません。
	*/
	void AdditionalColorRelease(void);

	/* ********************************************************* */
	//! TRSリフレッシュのキャンセル
	/*!
	@param	idParts
		パーツID
		-1 == 全パーツ対象
	@param	flagPosition
		true == 「X/Y/Z座標」のリフレッシュを行わない
		false == 通常動作（リフレッシュを行う）
	@param	flagRotation
		true == 「X/Y/Z軸回転」のリフレッシュを行わない
		false == 通常動作（リフレッシュを行う）
	@param	flagScaling
		true == 「X/Yスケール」のリフレッシュを行わない
		false == 通常動作（リフレッシュを行う）

	@retval	関数値
		true == 成功
		false == 失敗

	指定パーツのアニメーションが変更された場合などに起こる、下記の
		アトリビュートのリフレッシュを抑制する設定を行います。
	- X/Y/Z座標
	- X/Y/Z軸回転
	- X/Yスケール

	これらのリフレッシュ動作はアニメーションを安定動作させるために
		必要な処理になっています。
	ただし、アニメーションの再生状態によっては、それらのリフレッ
		シュが邪魔になる場合があります。
	例えば、本プレーヤでは、SpriteStudio6のデータ上で、上記のアト
		リビュートを一切設定していない（setupアニメーションでも通常
		アニメーションでもキーが打たれていない）場合などは、特段
		プレーヤは値を操作しない……という仕様があります。
	その特性を利用して何かしらパーツへの特殊な外部からの処理を行っ
		ている場合で・値がプレーヤから書き換えられると困るような
		時に本関数を使用してリフレッシュを抑制することができます
		（ただ、これらの運用は、かなり特殊な運用といえます）。
	*/
	bool RefreshCancelTransform(int idParts, bool flagPosition, bool flagRotation, bool flagScaling);

	/* ********************************************************* */
	//! インスタンスパーツの持つ子エンティティを取得
	/*!
	@param	idParts
		（インスタンスパーツの）パーツID

	@retval	関数値
		インスタンスパーツの持つ子エンティティ
		nullptr == エンティティを持っていない（もしくは失敗）

	インスタンスパーツが持っている子エンティティの制御クラス
		（Entity）のポインタを取得します。

	本関数がnullptrを返す場合、下記の主な理由が考えられます。
	- 指定パーツがインスタンスパーツでない
	- 子エンティティが作成されていない（データが不正）
	- 本クラスがBootUpとPackSetされていない
	*/
	Entity* InstanceGet(int idParts);

	/* ********************************************************* */
	//! インスタンスパーツが持つ子エンティティを変更（パック名版）
	/*!
	@param	idParts
		（インスタンスパーツの）パーツID
	@param	project
		設定するプロジェクト
		nullptr == 現在設定されているプロジェクトと同じ
	@param	namePack
		設定するアニメーションパック名

	@retval	関数値
		true == 成功
		false == 失敗

	インスタンスパーツが制御下に置いている子エンティティに対して
		再生を割り当てるアニメーションパックを変更します。
	本関数を使用する場合、本関数の後にAnimationChangeInstanceを使用
		する必要があります（本関数が使用された直後だと無造作に0番の
		アニメーションが割当されます）。

	本関数は非常に重い処理であり、内部的に使用している様々なワーク
		エリア（特に描画データ用バッファ）などを再構成する必要が
		出ることに注意してください。

	また、差し替える子アニメーションパックは（AnimationChangeInstance
		で設定するアニメーションも含めて）指定インスタンスパーツが指定
		している再生状態を満たすデータでなくてはなりません（本関数では
		それらの差替先のアニメーションパックのデータ内容の妥当性は判定
		しません）。
	*/
	bool InstanceChange(int idParts, Project* project, const char* namePack);

	/* ********************************************************* */
	//! インスタンスパーツが持つ子エンティティを変更（パック番号版）
	/*!
	@param	idParts
		（インスタンスパーツの）パーツID
	@param	project
		設定するプロジェクト
		nullptr == 現在設定されているプロジェクトと同じ
	@param	indexPack
		設定するアニメーションパック番号

	@retval	関数値
		true == 成功
		false == 失敗

	インスタンスパーツが制御下に置いている子エンティティに対して
		再生を割り当てるアニメーションパックを変更します。

	アニメーションパックの指定がインデックスであるか・名前であるか
		の違いなので、解説については同名関数（パック名版）を参照
		してください。

	※アニメーションパック番号をキャッシュしておける場合、文字列
		検索が入らない分だけ、若干だけ本関数の方が高速になります。
	*/
	bool InstanceChange(int idParts, Project* project, int indexPack);

	/* ********************************************************* */
	//! インスタンスパーツの持つ子エンティティに設定するアニメーションを指定（名前版）
	/*!
	@param	idParts
		（インスタンスパーツの）パーツID
	@param	nameAnimation
		アニメーション名
		空文字・nullptr == ignoreAttributeのみを変更する
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	ignoreAttribute
		NON == アニメーションデータ上で次の「インスタンス」キーが出現したら再生開始
		NOW_ANIMATION == 新しいアニメーションが変更されるまで変更が有効
		PERMANENT == 本クラスがShutDownされるまで有効
	@param	flagStartImmediate
		true == アニメーションを即時再生開始
		false == 次の「インスタンス」キーが検知された時点から再生開始
	@param	timesPlay
		0 == 無限ループ
		1 == ループしない（1回再生）
		2 <= 指定回数再生
		省略時: 1
	@param	rateTime
		再生速度（経過時間倍率）
		負数を与えると逆再生します。
		省略時: 1.0f
	@param	style
		再生方向
		Library::KindStylePlay::NOMAL == 通常再生
		Library::KindStylePlay::PINGPONG == ピンポン再生
		省略時: Library::KindStylePlay::NORMAL
	@param	labelRangeStart
		再生区間開始位置ラベル名
		nullptr・空文字列・"_start" == アニメーションの先頭（_startはSS6の予約ラベル名）
		省略時: nullptr
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	frameRangeOffsetStart
		再生区間開始ラベルからのオフセットフレーム値
		※実際の再生区間開始フレームはlabelRangeStart+frameRangeOffsetStartになります。
		省略時: 0
	@param	labelRangeEnd
		再生区間終了位置ラベル名
		nullptr・空文字列・"_end" == アニメーションの終端（_endはSS6の予約ラベル名）
		省略時: nullptr
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	frameRangeOffsetEnd
		再生区間終了ラベルからのオフセットフレーム値
		※実際の再生区間終端フレームはlabelRangeEnd+frameRangeOffsetEndになります。
		省略時: 0

	@retval	関数値
		true == 成功
		false == 失敗

	指定されたインスタンスパーツが制御下に置いている子エンティティ
		を変更します。

	子エンティティのアニメーションを変更するには、子エンティティの
		AnimationPlayを直接実行しないで、本関数を使用してください。
	※直接実行すると、親エンティティ側の持つ情報との不整合が起き、
		正常な実行結果が得られません。

	原則として、殆どの引数はAnimationPlayと同じ内容になりますが、
		「前回と同じ再生状態（再生パラメータを変更しない）」の指定
		がないことに注意してください。
	※ignoreAttributeのみを変更する場合が例外となりますが、その場合
		timesPlay以降の引数は省略記載をすることを推奨します。

	nameAttributeが設定されている場合、flagStartImmediateとの関係は
		下記になります。
	- flagStartImmediate == true
		子エンティティ（インスタンス）のアニメーションがすぐに再生
		されます。
		この場合、SpriteStudio6の「インスタンス」キーの「独立再生」
		がtrue（チェックされている）の時と同じ動作をします。
	- flagStartImmediate == false
		子エンティティのアニメーションは、アニメーションデータ上で
		「インスタンス」キーが発見された時に、キーの指定に従って
		再生を開始します。
		この場合に、timesPlay以降の引数は無視され・アニメーションの
		キーの内容を採用します（ので、flagStartImmediateがfalseの時
		には、timesPlay以降は省略記載してください）。
	*/
	bool AnimationChangeInstance(	int idParts,
									const char* nameAnimation,
									Library::KindIgnoreAttribute::Kind ignoreAttribute,
									bool flagStartImmediate=false,
									int timesPlay=1,
									float rateTime=1.0f,
									Library::KindStylePlay::Kind style=Library::KindStylePlay::NORMAL,
									const char* labelRangeStart=nullptr,
									int frameRangeOffsetStart=0,
									const char* labelRangeEnd=nullptr,
									int frameRangeOffsetEnd=0
							);

	/* ********************************************************* */
	//! インスタンスパーツの持つ子エンティティに設定するアニメーションを指定（番号版）
	/*!
	@param	idParts
		（インスタンスパーツの）パーツID
	@param	indexAnimation
		アニメーション番号
		-1 == ignoreAttributeのみを変更する
	@param	ignoreAttribute
		NON == アニメーションデータ上で次の「インスタンス」キーが出現したら再生開始
		NOW_ANIMATION == 新しいアニメーションが変更されるまで変更が有効
		PERMANENT == 本クラスがShutDownされるまで有効
	@param	flagStartImmediate
		true == アニメーションを即時再生開始
		false == 次の「インスタンス」キーが検知された時点から再生開始
	@param	timesPlay
		0 == 無限ループ
		1 == ループしない（1回再生）
		2 <= 指定回数再生
		省略時: 1
	@param	rateTime
		再生速度（経過時間倍率）
		負数を与えると逆再生します。
		省略時: 1.0f
	@param	style
		再生方向
		Library::KindStylePlay::NOMAL == 通常再生
		Library::KindStylePlay::PINGPONG == ピンポン再生
		省略時: Library::KindStylePlay::NORMAL
	@param	labelRangeStart
		再生区間開始位置ラベル名
		nullptr・空文字列・"_start" == アニメーションの先頭（_startはSS6の予約ラベル名）
		省略時: nullptr
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	frameRangeOffsetStart
		再生区間開始ラベルからのオフセットフレーム値
		※実際の再生区間開始フレームはlabelRangeStart+frameRangeOffsetStartになります。
		省略時: 0
	@param	labelRangeEnd
		再生区間終了位置ラベル名
		nullptr・空文字列・"_end" == アニメーションの終端（_endはSS6の予約ラベル名）
		省略時: nullptr
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	frameRangeOffsetEnd
		再生区間終了ラベルからのオフセットフレーム値
		※実際の再生区間終端フレームはlabelRangeEnd+frameRangeOffsetEndになります。
		省略時: 0

	@retval	関数値
		true == 成功
		false == 失敗

	指定されたインスタンスパーツが制御下に置いている子エンティティ
		を変更します。
	同名関数の名前版とはアニメーションの指定がnameAnimation（名前）か
		indexAnimation（番号）かの違い以外は同じですので、名前版の解
		説を参照してください。

	※アニメーション番号をキャッシュしておける場合、文字列検索が入ら
		ない分だけ、若干だけ本関数の方が高速になります。
	*/
	bool AnimationChangeInstance(	int idParts,
									int indexAnimation,
									Library::KindIgnoreAttribute::Kind ignoreAttribute,
									bool flagStartImmediate=false,
									int timesPlay=1,
									float rateTime=1.0f,
									Library::KindStylePlay::Kind style=Library::KindStylePlay::NORMAL,
									const char* labelRangeStart=nullptr,
									int frameRangeOffsetStart=0,
									const char* labelRangeEnd=nullptr,
									int frameRangeOffsetEnd=0
							);

	/* ********************************************************* */
	//! エフェクトパーツの持つ子エンティティを取得
	/*!
	@param	idParts
		（エフェクトパーツの）パーツID

	@retval	関数値
		エフェクトパーツの持つ子エンティティ
		nullptr == エンティティを持っていない（もしくは失敗）

	エフェクトパーツが持っている子エンティティの制御クラス
		（EntityEffect）のポインタを取得します。

	本関数がnullptrを返す場合、下記の主な理由が考えられます。
	- 指定パーツがエフェクトスパーツでない
	- 子エンティティが作成されていない（データが不正）
	- 本クラスがBootUpとDataSetされていない
	*/
	EntityEffect* EffectGet(int idParts);

// };	/* Entity */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */
