/**
	SS6Player for CPP(C++/ssbp2)
	Platform: 

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO: 【注意!!】                                                           */
/* 本ヘッダはentity.hの一部です。（パーツ操作関数群）                         */
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
	bool HideSetMain(int idParts, bool flagSwitch, bool flagInvolveChildren);
	bool DataGetColorLabel(Library::Data::Parts::Animation::ColorLabel* dataColorLabel, int idParts);
	bool FlipSetPartsMain(int idParts, Uint32 flags, bool flagInvolveChildren);

protected:
public:
	/* ********************************************************* */
	//! アニメーションデータ中のパーツ数の取得
	/*!
	@param	なし

	@retval	関数値
		アニメーションデータ中のパーツ数
		-1 == 失敗

	現在扱っているアニメーションに定義されているパーツ数を取得
		します。
	※パーツIDは「0 ～ 本関数の値-1」の範囲の値になります。
	*/
	int CountGetParts(void)
	{
		if(false == DataAnimation->IsValid())	{
			return(-1);
		}

		return(DataAnimation->CountTableParts());
	}

	/* ********************************************************* */
	//! パーツ種別の取得
	/*!
	@param	idParts
		パーツID

	@retval	関数値
		パーツ種別
		-1 == 失敗

	現在扱っているアニメーションに定義されているパーツの種別を
		取得します。
	*/
	Library::Data::Parts::Animation::KindFeature FeatureGetParts(int idParts)
	{	/* MEMO: 良く使うので関数化 */
		Library::Data::Parts::Animation dataPartsAnimation;
		DataAnimation->TableParts(&dataPartsAnimation, idParts);

		return(dataPartsAnimation.Feature());
	}

	/* ********************************************************* */
	//! パーツIDの取得
	/*!
	@param	name
		パーツ名
	@param	length
		パーツ名の長さ（合致最大長）

	@retval	関数値
		パーツID
		-1 == 失敗

	現在扱っているアニメーションに定義されているパーツの中から
		nameと同じ文字列長を持ったパーツのIDを返します。

	lengthに0より上の値を与えると、その長さ分しか照合しません
		（0はターミネータがくるまで照合を続けます）。
	*/
	int IDGetParts(const char* name, size_t length=0)
	{
		int count = CountGetParts();
		for(int i=0; i<count; i++)	{
			if(true == Library::Utility::Text::Compare(name, DataAnimation->NameGetParts(i), length))	{
				return(i);
			}
		}

		return(-1);
	}

	/* ********************************************************* */
	//! パーツ名の取得
	/*!
	@param	idParts
		パーツID

	@retval	関数値
		true == 成功
		false == 失敗

	現在扱っているアニメーションに定義されているパーツの名前を
		取得します。
	*/
	const char* NameGetParts(int idParts)
	{
		return(DataAnimation->NameGetParts(idParts));
	}

	/* ********************************************************* */
	//! パーツ制御の取得
	/*!
	@param	idParts
		パーツID

	@retval	関数値
		指定IDのパーツ制御
		nullptr == 失敗（エラー）

	指定パーツIDのパーツ制御を取得します。
	通常使用時は取得する必要はほとんどありません（本関数で取得
		したパーツ制御をアプリケーションから直接操作することは
		誤動作防止の観点から推奨しかねます）。
	※予備として公開してはいますが、原則としてプレーヤの内部処理
		のために設置されている関数です。
	*/
	Library::Control::Animation::Parts* ControlGetParts(int idParts)
	{
		int count = CountGetParts();
		if((0 > idParts) || (count <= idParts))	{
			return(nullptr);
		}
		if(nullptr == TableControlParts)	{
			return(nullptr);
		}

		return(&TableControlParts[idParts]);
	}

	/* ********************************************************* */
	//! パーツの強制非表示状態の設定
	/*!
	@param	idParts
		パーツID
		0 == 全パーツ（Entity全体への指定）
		-1 == 全パーツへの設定
	@param	flagSwitch
		非表示状態
		true == 強制非表示
		false == 強制非表示解除（アニメーションデータに準じる）
	@param	flagInvolveChildren
		true == 指定パーツの子パーツにも設定
		false == 指定パーツのみに設定
		省略時: false

	@retval	関数値
		true == 成功
		false == 失敗

	指定パーツの強制非表示状態を設定します。
	flagSwitchがtrueの場合は、指定パーツをアニメーションデータの
		状態を問わず表示しません。
	falseの場合の表示状態は、アニメーションデータの状態に依存します
		（強制的に表示する……ような指定はありません）。

	本関数は描画パーツでなくても設定はできますが、非描画に設定を行っ
		てもその結果は実際に効果することはありません。
	また、本関数で設定した非表示状態は（flagInvolveChildrenをtrueに
		設定しない限り）、子パーツ群への影響はありませんので注意して
		ください（SpriteStudio6上での「（アトリビュートの）継承」の
		効果はありません）。

	idPartsが0（SpriteStudio6のrootパーツ）の場合、パーツではなくアニ
		メーション全体に対する強制非表示の設定となり・パーツ毎の設定と
		は競合しない設定となります。
	このアニメーション全体への指定は、パーツ毎への指定とは競合しない
		他、trueである場合、パーツ毎への指定の上位にあります（つまり
		パーツがfalseの状態であっても、全体への指定がtrueであれば、
		そのエンティティが再生している全パーツが非表示状態になります）。
	※全体への指定ががfalseの場合は、各パーツの描画状態は各パーツへ
		の設定状態に従います。

	一方、idPartsに-1を指定した場合には、全てのパーツにflagSwitchの値
		を個別に設定していく指定となります（つまりidPartsが0の時に設定
		されるアニメーションエンティティ全体への操作ではなく、全ての
		パーツに対して、同じ値を一斉設定するための指定です）。

	idPartsが0もしくは-1の時、flagInvolveChildrenは無視されます。
	*/
	bool HideSet(int idParts, bool flagSwitch, bool flagInvolveChildren=false);

	/* ********************************************************* */
	//! パーツのカラーラベルの取得
	/*!
	@param	idParts
		パーツID

	@retval	関数値
		カラーラベル種別
		-1 == 失敗

	指定パーツのカラーラベル（SpriteStudio上でパーツ毎に設定できる
		カラーラベル）を取得します。

	本関数の返値がCUSTOMである場合、実際に指定されている色を取得
		するには、ColorGetColorLabel関数を使用する必要があります。
	*/
	Library::Data::Parts::Animation::ColorLabel::KindForm FormGetColorLabel(int idParts);

	/* ********************************************************* */
	//! パーツのカラーラベル色の取得
	/*!
	@param	color
		色情報の格納先
	@param	idParts
		パーツID

	@retval	関数値
		true == 成功
		false== 失敗（colorの中身は不定）
	@retval	color
		カラーラベルが示す実際の色

	指定パーツのカラーラベル（SpriteStudio上でパーツ毎に設定できる
		カラーラベル）の実際の色を取得します。

	本関数の返値がCUSTOMである場合、実際に指定されている色を取得
		するには、ColorGetColorLabel関数を使用する必要があります。
	パーツカラーが指定されていない（FormGetColorLabelがNONを返す）
		パーツに対して本関数を使用した場合、本関数はfalseを返し
		ます。
	*/
	bool ColorGetColorLabel(CPU::Color* color, int idParts);

	/* ********************************************************* */
	//! パーツの追加反転状態の設定
	/*!
	@param	idParts
		パーツID
	@param	flagFlipX
		true == 描画パーツそのものを横方向反転
		false == 描画パーツそのものを横方向反転しない
	@param	flagFlipY
		true == 描画パーツそのものを縦方向反転
		false == 描画パーツそのものを縦方向反転しない
	@param	flagFlipImageX
		true == 描画パーツの中のセル画像を横方向反転
		false == 描画パーツの中のセル画像を横方向反転しない
	@param	flagFlipImageY
		true == 描画パーツの中のセル画像を縦方向反転
		false == 描画パーツの中のセル画像を縦方向反転しない
	@param	flagInvolveChildren
		true == 指定パーツの子パーツにも設定
		false == 指定パーツのみに設定
		省略時: false

	@retval	関数値
		true == 成功
		false== 失敗

	指定パーツの追加反転表示状態を設定します。
	本設定は再生中のアニメーションに、追加で描画パーツの反転
		状態に影響を与えます（アニメーションデータ状態に対する
		上書ではありません）。

	それぞれがSpriteStudio6の各アトリビュートに対応しています。
	- flagFlipX: 左右反転
	- flagFlipY: 上下反転
	- flagFlipImageX: イメージ左右反転
	- flagFlipImageY: イメージ上下反転

	本関数は、「通常」「マスク」のどちらかのパーツにのみ使用可能
		です（非描画パーツ及び「メッシュ」「インスタンス」「エフェ
		クト」パーツには使用できません）。

	idPartsが-1の時、flagInvolveChildrenは無視されます。
	*/
	bool FlipSetParts(int idParts, bool flagFlipX, bool flagFlipY, bool flagFlipImageX, bool flagFlipImageY, bool flagInvolveChildren=false);

	/* ********************************************************* */
	//! パーツの追加反転状態の取得
	/*!
	@param	flagFlipX
		描画パーツの横方向反転状態の取得先
	@param	flagFlipY
		描画パーツの縦方向反転状態の取得先
	@param	flagFlipImageX
		描画パーツの中のセル画像の横方向反転状態の取得先
	@param	flagFlipImageY
		描画パーツの中のセル画像の縦方向反転状態の取得先
	@param	idParts
		パーツID

	@retval	関数値
		true == 成功
		false== 失敗
	@retval	flagFlipX
		描画パーツの横方向反転状態
	@retval	flagFlipY
		描画パーツの縦方向反転状態
	@retval	flagFlipImageX
		描画パーツの中のセル画像の横方向反転状態
	@retval	flagFlipImageY
		描画パーツの中のセル画像の縦方向反転状態

	指定パーツの追加反転表示状態を取得します。
	本関数はは再生中のアニメーションに対して、追加で与えている
		反転状態を取得するものですので、アニメーションデータ
		そのものの反転状態・実際に表示されている反転状態を取得
		する役割ではありません。

	本関数は、「通常」「マスク」のどちらかのパーツにのみ使用可能
		です（非描画パーツ及び「メッシュ」「インスタンス」「エフェ
		クト」パーツには使用できません）。
	*/
	bool FlipGetParts(bool* flagFlipX, bool* flagFlipY, bool* flagFlipImageX, bool* flagFlipImageY, int idParts);

// };	/* Entity */
// }	/* SpriteStudio6 */

#endif	/* defined(__SS6PLAYER_ENTITY_CLASS__) */
