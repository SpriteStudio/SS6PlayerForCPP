/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO: 【注意!!】                                                           */
/* 本ヘッダはentity.hの一部です（セル操作関数群）。                           */
/* 単独でincludeしないでください（一応コンパイルエラー防護はしているものの、  */
/* 内容が正常に適用されません）。                                             */
/* ※EntityEffectクラスの機能が大きい巨大なため、各区分の機能を見通しやすくす */
/*   るために（C#のpartial的に）ファイルを分割してあるだけです。              */
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
	bool CellChangePartsMain(int indexData, int idParts, int indexCellMap, int indexCell);

protected:
public:
	/* MEMO: 下記の役割関数は、Library::Control::Rootクラスに基底機能として実装しています。 */
	/*       - セルマップ数の取得 : CountGetCellMap                                         */
	/*       - セルマップのインデックスを取得 : IndexGetCellMap                             */
	/*       - セルマップ番号から名前を取得 : NameGetCellMap                                */
	/*       - セルマップデータを取得 : DataGetCellMap                                      */
	/*       - セルマップデータを設定 : DataSetCellMap                                      */
	/*       - セル数の取得 : CountGetCell                                                  */
	/*       - セル情報の取得 : DataGetCell                                                 */

	/* ********************************************************* */
	//! パーツで描画使用中のセルを変更
	/*!
	@param	idParts
		エフェクトのパーツID
		-1 == 全パーツ対象
	@param	indexCellMap
		セルマップ番号
		-1 == 元の（SSFB2に記録されている）セルに戻す
		省略時: -1
	@param	indexCell
		セルマップ中にセル番号
		-1 == 元の（SSFB2に記録されている）セルに戻す
		省略時: -1

	@retval	関数値
		true == 成功
		false == 失敗

	エフェクトのパーツ（の持つエミッタ）で使用するセルを変更します。

	indexCellMapかindexCellに-1を指定すると元のSSFB2に格納されて
		いるセルに戻します。
	その指定の場合、indexCellMap・indexCellを省略記載するように
		してください。
	*/
	bool CellChangeParts(int idParts, int indexCellMap=-1, int indexCell=-1);

// };	/* EntityEffect */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */
