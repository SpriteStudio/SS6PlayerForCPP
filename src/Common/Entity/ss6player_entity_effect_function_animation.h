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
// class EntityEffect	{
	/* ----------------------------------------------- Functions */
private:
protected:
public:
	/* ********************************************************* */
	//! アニメーションの再生開始
	/*!
	@param	timesPlay
		-1 == 前回指定と同じ
		0 == 無限ループ
		1 == ループしない（1回再生）
		2 <= 指定回数再生
		省略時: -1
	@param	rateTime
		再生速度（経過時間倍率）
		負数を与えると逆再生します。
		quiet_NaN == 前回指定と同じ
		省略時: quiet_NaN
	@param	framePerSecond
		Frame Per Second (In principle, omit specifying this value)<br>
		0 == Conforms to animation<br>
		default: 0

	@retval	関数値
		true == Success<br>
		false == Failure (Error)

	The playing of animation begins. <br>
	<br>
	The update speed of animation quickens when you give a value that is bigger than 1.0f to "RateTime".
	*/
	bool AnimationPlay(	int timesPlay=-1,
						float rateTime=std::numeric_limits<float>::quiet_NaN(),
						int framePerSecond=0
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

	@retval	なし

	現在再生中のアニメーションを停止します。
	*/
	void AnimationStop(bool flagJumpEnd=false);

	/* ********************************************************* */
	//! アニメーションの一時停止状態の設定
	/*!
	@param	flagSwitch
		true == 一時停止
		false == 一時停止解除

	@retval	Return-Value
		true == 成功
		false == 失敗

	アニメーションの一時停止状態を設定します。
	*/
	bool AnimationPause(bool flagSwitch);

// };	/* EntityEffect */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */
