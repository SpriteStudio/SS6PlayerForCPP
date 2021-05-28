/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO: 【注意!!】                                                           */
/* 本ヘッダはentity.hの一部です（アニメーション操作関数群）。                 */
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
	/* ----------------------------------------------- Functions */
private:
	bool AnimationStopMain(int indexTrack, bool flagJumpEnd, bool flagEngageTransition);
	bool AnimationPauseMain(int indexTrack, bool flagSwitch);

protected:
public:
	/* ********************************************************* */
	//! アニメーションの個数を取得
	/*!
	@param	なし

	@retval	関数値
		保持しているアニメーション数
		-1 == 失敗

	現在割り当たっているアニメーションパックが持つアニメーション
		個数を取得します。
	*/
	inline int CountGetAnimation(void)
	{
		return((int)(DataAnimation->CountTableAnimation()));
	}

	/* ********************************************************* */
	//! 名前からアニメーションのインデックスを取得
	/*!
	@param	name
		検索するアニメーション名

	@retval	関数値
		アニメーションのインデックス
		-1 == 見つからなかった

	現在割り当たっているアニメーションパックが持つアニメーション
		から指定の名前を持つアニメーションのインデックスを返します。
	*/
	int IndexGetAnimation(const char* name);

	/* ********************************************************* */
	//! アニメーション番号から名前を取得
	/*!
	@param	indexAnimation
		アニメーション番号

	@retval	関数値
		アニメーション名
		nullptr == 見つからなかった

	現在割り当たっているアニメーションパックが持つアニメーション
		から指定のアニメーション番号のアニメーション名を返します。

	返ったポインタはアニメーションデータ内の情報を直接指しています
		ので、変更などは行わないようにしてください。
	*/
	const char* NameGetAnimation(int indexAnimation);

	/* ********************************************************* */
	//! アニメーションの再生開始
	/*!
	@param	indexTrack
		使用する再生トラック番号
		-1 == 現在再生中の全てのトラックの再生を停止してトラック0
				で再生（全パーツの適用再生トラックがトラック0に
				再設定されます）
	@param	indexAnimation
		アニメーション番号
		-1 == 前回指定と同じ
		省略時: -1
	@param	timesPlay
		-1 == 前回指定と同じ
		0 == 無限ループ
		1 == ループしない（1回再生）
		2 <= 指定回数再生
		省略時: -1
	@param	frameOffset
		強制的にアニメーションの再生開始するフレーム数
		実際のアニメーション開始フレームは
		「labelRangeStart + frameOffsetStart+ frameOffset」
		になり、再生範囲外を指定することはできません（そのような
		場合、本値は0に調整されます）
		INT_MIN == 前回指定と同じ
		省略時: INT_MIN
	@param	rateTime
		再生速度（経過時間倍率）
		負数を与えると逆再生します。
		quiet_NaN == 前回指定と同じ
		省略時: quiet_NaN
	@param	style
		再生方向
		Library::KindStylePlay::NOMAL == 通常再生
		Library::KindStylePlay::PINGPONG == ピンポン再生
		Library::KindStylePlay::NO_CHANGE == 前回と同じ
		省略時: Library::KindStylePlay::NO_CHANGE
	@param	labelRangeStart
		再生区間開始位置ラベル名
		空文字列・"_start" == アニメーションの先頭（_startはSS6の予約ラベル名）
		nullptr == 前回と同じラベル名
		省略時: nullptr
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	frameRangeOffsetStart
		再生区間開始ラベルからのオフセットフレーム値
		※実際の再生区間開始フレームはlabelRangeStart+frameRangeOffsetStartになります。
		INT_MIN == 前回設定と同じ<br>
		省略時: INT_MIN
	@param	labelRangeEnd
		再生区間終了位置ラベル名
		空文字列・"_end" == アニメーションの終端（_endはSS6の予約ラベル名）
		nullptr == 前回と同じラベル名
		省略時: nullptr
		※空文字の指定にはSpriteStudio6::TextEmptyを使用してください。
	@param	frameRangeOffsetEnd
		再生区間終了ラベルからのオフセットフレーム値
		※実際の再生区間終端フレームはlabelRangeEnd+frameRangeOffsetEndになります。
		INT_MIN == 前回設定と同じ<br>
		省略時: INT_MIN
	@param	framePerSecond
		再生FPS（原則として、本値は指定しない方が良いです）
		0 == アニメーションデータに準ずる
		省略時: 0
	@param	dataAnimation
		別のアニメーション（パック）データに内包されるアニメーションを再生
		※本値は現在実効しません（予約引数です）
		nullptr == 現在設定されているパックのデータを使用
		省略時: nullptr

	@retval	関数値
		true == 成功
		false == 失敗

	アニメーションを再生開始します。

	indexAnimationは、IndexGetAnimation関数などで取得するようにしてください。
	もし存在しないアニメーション番号を指定した場合、本関数はfalseを返します。
	<br>
	アニメーションの速度を調整する際は、framePerSecondを使用するのではなく、
		rateTimeで調整することを推奨します。
	*/
	bool AnimationPlay(	int indexTrack,
						int indexAnimation=-1,
						int timesPlay=-1,
						int frameOffset=INT_MIN,
						float rateTime=std::numeric_limits<float>::quiet_NaN(),
						Library::KindStylePlay::Kind style=Library::KindStylePlay::NO_CHANGE,
						const char* labelRangeStart=nullptr,
						int frameRangeOffsetStart=INT_MIN,
						const char* labelRangeEnd=nullptr,
						int frameRangeOffsetEnd=INT_MIN,
						int framePerSecond=0,
						Library::Data::Project::Animation* dataAnimation=nullptr
					);

	/* ********************************************************* */
	//! アニメーションの停止
	/*!
	@param	indexTrack
		停止する再生トラック
		-1 == 全再生トラックを停止
	@param	flagJumpEnd
		false == 現在の再生位置で停止します
		true == 再生区間指定の末尾フレームまでジャンプします
		省略時: false
	@param	flagEngageTransition
		トランジションしていた時にトランジションも終了させるか
		false == 現状維持（トランジションは継続動作）
		true == トランジションも終了
		省略時: false

	@retval	なし

	現在再生中のアニメーションを停止します。

	「frameJumpEnd」がtrueの場合で、現在再生位置と末尾フレームとの
		間にユーザデータがあっても無視されます（ユーザデータ検知の
		コールバックは動作しません）。
	同様に、indexTrackで指定したトラックの再生終了コールバック及び
		エンティティの再生終了コールバックも発行されません。

	「flagEngageTransition」がtrueの場合にはトランジションは強制的
		にトランジション満了の状態になります。
	また、同時に「flagJumpEnd」がtrueに設定されていた場合、トランジ
		ションのスレーブ再生トラックの再生フレームは末尾にジャンプ
		しません。
	*/
	void AnimationStop(int indexTrack, bool flagJumpEnd=false, bool flagEngageTransition=false);

	/* ********************************************************* */
	//! アニメーションの一時停止状態の設定
	/*!
	@param	indexTrack
		一時停止設定する再生トラック
		-1 == 全再生トラックを停止
	@param	flagSwitch
		true == 一時停止
		false == 一時停止解除
	@retval	Return-Value
		true == 成功
		false == 失敗

	アニメーションの一時停止状態を設定します。
	indexTrackが-1の場合、全てのトラックで設定に成功すると、trueを
		返します。
	indexTrackが0以上の場合、指定のトラックが再生状態にない場合は
		本関数はfalseを返します。

	また、指定トラックが（トラックトランジション中で）スレーブト
		ラックを持つ場合、スレーブにも同じ一時停止状態が設定され
		ます。
	*/
	bool AnimationPause(int indexTrack, bool flagSwitch);
// };	/* Entity */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */
