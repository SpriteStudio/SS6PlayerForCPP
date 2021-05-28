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
	bool CellChangePartsMain(int idParts, int indexCellMap, int indexCell, Library::KindIgnoreAttribute::Kind ignoreAttribute);

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
		パーツID
	@param	indexCellMap
		セルマップ番号
		-1 == アニメーションデータでの参照セルに戻す
	@param	indexCell
		（セルマップ内）セル番号
	@param	ignoreAttribute
		変更有効期間
		Library::KindIgnoreAttribute::NON == 次に更新されるまで
		Library::KindIgnoreAttribute::NOW_ANIMATION == 現在の
				アニメーションが変更されるまで
		Library::KindIgnoreAttribute::PERMANENT == 恒久的

	@retval	関数値
		true == 成功
		false == 失敗

	現在再生中のアニメーションでパーツが描画に使用しているセルを
		強制変更します。

	ignoreAttributeがNONの場合、本関数実行直後から最初にパーツの
		アニメーションデータ上で参照セルが更新されると、本関数での
		設定はアニメーションデータで上書きされます。
	PERMANENTの場合、アニメーションを変更しても本関数での指定が
		持ち越されますが、本クラスをShutDownしたり・PackSetしたり
		した場合には、解除されます。
	本関数を実行した場合、その前に本関数を実行した時に指定した
		ignoreAttributeの値に関わらず、状態を上書きできます。

	本関数でのセル変更を元に戻す（アニメーションデータでの参照
		セルを適用する）には、indexCellMapに-1を指定してください。
	*/
	bool CellChangeParts(int idParts, int indexCellMap, int indexCell, Library::KindIgnoreAttribute::Kind ignoreAttribute);
// };	/* Entity */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */
