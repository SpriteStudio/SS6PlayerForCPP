/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* ========================================================================== */
/* MEMO:                                                                      */
/* SS6Playerの内部データ構造の定義群です。                                    */
/* ※定義順や役割の関係から、プロジェクト周り（プロジェクト・アニメーション・ */
/*   エフェクト・セルマップ）の大区分データ構造については、data_project.h/cpp */
/*   に飛び地定義されています。                                               */
/* ========================================================================== */
#pragma once

/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include <ss6player_platform.h>
#include "../ssfb2_generated.h"
#include "../Library/ss6player_utility.h"

#include <vector>

/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

namespace SpriteStudio6	{
namespace Library	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */
/* Library全域固定値関係 */
/* MEMO: 外部から本値を使って何かを設定する場合に必要になる定数関係は、記述の簡便さから */
/*       ここに露出する形で定義してあります。                                           */
/* MEMO: MIX～ADDの並びがSpriteStudio6の並びと異なるのですが……SpriteStudio5からの */
/*       並びを（連番として）受け継いでいる関係から、本順序になっています。         */
namespace KindOperationBlend	{
enum Kind	{
	MASK_PRE = Renderer::SectionBlend::KindType::MASK_PRE,
	MASK = Renderer::SectionBlend::KindType::MASK,
	NON = Renderer::SectionBlend::KindType::NONE,	/* for Control.AdditionalColor */

	MIX = Renderer::SectionBlend::KindType::MIX,
	ADD = Renderer::SectionBlend::KindType::ADD,
	SUB = Renderer::SectionBlend::KindType::SUB,
	MUL = Renderer::SectionBlend::KindType::MUL,

	MUL_NA = Renderer::SectionBlend::KindType::MUL_NA,
	SCR = Renderer::SectionBlend::KindType::SCR,
	EXC = Renderer::SectionBlend::KindType::EXC,
	INV = Renderer::SectionBlend::KindType::INV,

	TERMINATOR = Renderer::SectionBlend::KindType::TERMINATOR,

	TERMINATOR_PARTSCOLOR = MUL_NA,
	INITIATOR = MASK_PRE,
	TERMINATOR_TABLEMATERIAL = TERMINATOR - INITIATOR,	/* - (-x) = +(x) */
};
}	/* KindOperationBlend */
namespace KindOperationBlendEffect	{
enum Kind	{
	NON = -1,

	MIX = 0,
	ADD,

	TERMINATOR,

	INITIATOR = MIX,
	TERMINATOR_TABLEMATERIAL = TERMINATOR - INITIATOR,	/* - (-x) = +(x) */
};
}	/* KindOperationBlendEffect */
namespace KindBoundBlend	{
enum Kind	{
	NON = 0,
	OVERALL,
	VERTEX
};
}	/* KindBoundBlend */
namespace KindVertex	{
enum Kind	{
	LU = 0,	/* Left-Up (TRIANGLE2 & TRIANGLE4) */
	RU,	/* Right-Up (TRIANGLE2 & TRIANGLE4) */
	RD,	/* Right-Down (TRIANGLE2 & TRIANGLE4) */
	LD,	/* Left-Down (TRIANGLE2 & TRIANGLE4) */
	C,	/* Center (TRIANGLE4) */

	TERMINATOR,
	TERMINATOR4 = TERMINATOR,	/* for "Normal(Sprite)" */
	TERMINATOR3 = LD,			/* for "Mesh" */
	TERMINATOR2 = C				/* for "Effect" */
};
} /* KindVertex */
namespace KindStylePlay	{
enum Kind	{
	NO_CHANGE = -1,
	NORMAL = 0,
	PINGPONG = 1,
};
}	/* KindStylePlay */
namespace KindIgnoreAttribute	{
enum Kind	{
	NON = 0,
	NOW_ANIMATION,
	PERMANENT,
};
}	/* KindIgnoreAttribute */
namespace KindMasking	{
enum Kind	{
	THROUGH = 0,
	MASK,

	TERMINATOR,
	FOLLOW_DATA = TERMINATOR,
};
}	/* KindMasking */

namespace Data	{
/* -------------------------------------------------------------------------- */
/*                                                Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                            Classes / Structures / Typedefs */
/* -------------------------------------------------------------------------- */
/* パーツ関係 */
/* MEMO: 本名前空間内にAnimation・Effectが存在し、同時にAnimation名前空間内に  */
/*       Partsがあるために混乱する場合がありますが、下記の役割になっています。 */
/*       - Parts::Animation : アニメーション内に設置されているパーツの情報     */
/*       - Parts::Effect : エフェクト内に設置されているパーツの情報            */
/*       - Animation::Parts : アニメーションが持っている各アニメーションのパー */
/*                            ツ毎のアニメーションデータ                       */
/*      ※Effetにもパーツは存在しますがクラス名としては存在していません。      */
namespace Parts	{
	/* アニメーション内モデルパーツ情報 */
	class Animation
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		/* カラーラベル情報 */
		class ColorLabel
		{
			/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
		private:
		protected:
		public:

			/* ----------------------------------------------- Enums & Constants */
		private:
		protected:
		public:
			enum KindForm	{
				NON = 0,

				RED,
				ORANGE,
				YELLOW,
				GREEN,
				BLUE,
				VIOLET,
				GRAY,

				TERMINATOR,
				CUSTOM = TERMINATOR,
			};

			/* MEMO: SSデフォルトのカラーラベル */
			static const ColorLabel TableDefault[KindForm::TERMINATOR];

			/* ----------------------------------------------- Variables */
		private:
		protected:
		public:
			/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
			const ss::ssfb2::ColorLabel* Value;				/* 情報へのポインタ */

			/* MEMO: 動的もしくはコード内定数として値がある場合の格納先です。 */
			/*       Valueがnullptrの場合のみ適用されます。                   */
			KindForm ValueForm;
			CPU::Color32 ValueColor;

			/* ----------------------------------------------- Functions */
		private:
		protected:
		public:
			inline void CleanUp(void)
			{
				Value = nullptr;

				ValueForm = KindForm::NON;
				ValueColor.r = ValueColor.g = ValueColor.b = ValueColor.a = 0;
			}

			ColorLabel(void)
			{
				CleanUp();
			}

			ColorLabel(KindForm valueForm, CPU::Color32& valueColor)
			{
				Value = nullptr;
				ValueForm = valueForm;
				ValueColor = valueColor;
			}

			inline KindForm Form(void)
			{
				if(nullptr == Value)	{
					return(ValueForm);
				}

				return((KindForm)(Value->form()));
			}
			inline const CPU::Color32& Color(CPU::Color32& color)
			{
				if(nullptr == Value)	{
					color = ValueColor;

					return(color);
				}
				color.r = Value->color()->r();
				color.g = Value->color()->g();
				color.b = Value->color()->b();
				color.a = Value->color()->a();

				return(color);
			}

			/* ----------------------------------------------- Operators */

			/* ----------------------------------------------- Friends */
		};

		/* メッシュバインド情報 */
		class BindMesh
		{
			/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
		private:
		protected:
		public:
			/* 頂点情報 */
			class Vertex
			{
				/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
			private:
			protected:
			public:
				/*ボーン情報 */
				class Bone
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
				public:
					/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
					const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Bone>>* Value;	/* 情報へのポインタ */

					/* ----------------------------------------------- Functions */
				private:
				protected:
				public:
					inline int CountData(void)
					{
						return((int)(Value->size()));
					}
					inline int Index(int index)
					{
						return((int)((*Value)[index]->index()));
					}
					inline Float32 Weight(int index)
					{
						/* MEMO: Float32はfloatのエリアスなのでそのままキャストしています。 */
						return((Float32)(*Value)[index]->weight());
					}
					inline CPU::Vector3* CoordinateOffset(CPU::Vector3* coordinateOffset, int index)
					{
						const ss::ssfb2::Vector3* original = (*Value)[index]->coordinate_offset();
						coordinateOffset->Set(	original->x(),
												original->y(),
												original->z()
											);

						return(coordinateOffset);
					}
					inline FPU::Vector4* CoordinateOffset(FPU::Vector4* coordinateOffset, int index)
					{	/* MEMO: FPUの場合は事後の計算をしやすくするため、Vector4で受け取っています。 */
						const ss::ssfb2::Vector3* original = (*Value)[index]->coordinate_offset();
						coordinateOffset->Set(	original->x(),
												original->y(),
												original->z(),
												1.0f			/* 頂点座標なので1.0fですが……0.0fでも良いのかも */
											);

						return(coordinateOffset);
					}

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
				/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
				/* MEMO: ss::ssfb2::Vertex* Valueで持とうとすると実装が迂遠になるので……。 */
				const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vertex>>* Value;	/* 情報へのポインタ */

				/* ----------------------------------------------- Functions */
			private:
			protected:
			public:
				inline int CountTableBone(int index)
				{
					return((int)((*Value)[index]->table_bone()->size()));
				}
				inline const Bone* TableBone(Bone* bone, int index)
				{
					bone->Value = (*Value)[index]->table_bone();

					return(bone);
				}

				/* ----------------------------------------------- Operators */

				/* ----------------------------------------------- Friends */
			};

			/* UV情報 */
			class RateUV
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
			public:
				/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
				/* MEMO: ss::ssfb2::Vertex* Valueで持とうとすると実装が迂遠になるので……。 */
				const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::Vector2>>* Value;	/* 情報へのポインタ */

				/* ----------------------------------------------- Functions */
			private:
			protected:
			public:
				inline CPU::Vector2* UV(CPU::Vector2* rateUV, int index)
				{
					rateUV->Set(	(*Value)[index]->x(),
									(*Value)[index]->y()
							);

					return(rateUV);
				}

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
			/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
			const ss::ssfb2::BindMesh* Value;				/* 情報へのポインタ */

			/* ----------------------------------------------- Functions */
		private:
		protected:
		public:
			inline int CountVertex(void)
			{
				return((int)Value->count_vertex());
			}

			inline int CountTableVertex(void)
			{
				return((int)(Value->table_vertex()->size()));
			}
			inline const Vertex* TableVertex(Vertex* vertex)
			{
				vertex->Value = Value->table_vertex();

				return(vertex);
			}
			inline int CountTableRateUV(void)
			{
				return((int)(Value->table_rate_uv()->size()));
			}
			inline const RateUV* TableRateUV(RateUV* rateUV)
			{
				rateUV->Value = Value->table_rate_uv();

				return(rateUV);
			}
			inline int CountTableIndexVertex(void)
			{
				return((int)(Value->table_index_vertex()->size()));
			}
			inline const Sint16* TableIndexVertex(void)
			{
				/* MEMO: Sint32はint32_tのエリアスなので、素直にキャストしています。 */
				return((const Sint16*)(Value->table_index_vertex()->data()));
			}
			inline int CountVertexDeform(void)
			{
				return((int)(Value->count_vertex_deform()));
			}

			/* ----------------------------------------------- Operators */

			/* ----------------------------------------------- Friends */
		};

		/* パーツカタログ */
		class Catalog
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
		public:
			/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
			const ss::ssfb2::Catalog* Value;				/* 情報へのポインタ */

			/* ----------------------------------------------- Functions */
		private:
		protected:
		public:
			inline int CountTableIDPartsNULL(void)
			{
				return((int)(Value->table_id_parts_null()->size()));
			}
			inline const Uint16* TableIDPartsNULL(void)
			{
				return((const Uint16*)(Value->table_id_parts_null()->data()));
			}

			inline int CountTableIDPartsNormal(void)
			{
				return((int)(Value->table_id_parts_normal()->size()));
			}
			inline const Uint16* TableIDPartsNormal(void)
			{
				return((const Uint16*)(Value->table_id_parts_normal()->data()));
			}

			inline int CountTableIDPartsInstance(void)
			{
				return((int)(Value->table_id_parts_instance()->size()));
			}
			inline const Uint16* TableIDPartsInstance(void)
			{
				return((const Uint16*)(Value->table_id_parts_instance()->data()));
			}

			inline int CountTableIDPartsEffect(void)
			{
				return((int)(Value->table_id_parts_effect()->size()));
			}
			inline const Uint16* TableIDPartsEffect(void)
			{
				return((const Uint16*)(Value->table_id_parts_effect()->data()));
			}

			inline int CountTableIDPartsMask(void)
			{
				return((int)(Value->table_id_parts_mask()->size()));
			}
			inline const Uint16* TableIDPartsMask(void)
			{
				return((const Uint16*)(Value->table_id_parts_mask()->data()));
			}

			inline int CountTableIDPartsJoint(void)
			{
				return((int)(Value->table_id_parts_joint()->size()));
			}
			inline const Uint16* TableIDPartsJoint(void)
			{
				return((const Uint16*)(Value->table_id_parts_joint()->data()));
			}

			inline int CountTableIDPartsBone(void)
			{
				return((int)(Value->table_id_parts_bone()->size()));
			}
			inline const Uint16* TableIDPartsBone(void)
			{
				return((const Uint16*)(Value->table_id_parts_bone()->data()));
			}

			inline int CountTableIDPartsMoveNode(void)
			{
				return((int)(Value->table_id_parts_move_node()->size()));
			}
			inline const Uint16* TableIDPartsMoveNode(void)
			{
				return((const Uint16*)(Value->table_id_parts_move_node()->data()));
			}

			inline int CountTableIDPartsConstraint(void)
			{
				return((int)(Value->table_id_parts_constraint()->size()));
			}
			inline const Uint16* TableIDPartsConstraint(void)
			{
				return((const Uint16*)(Value->table_id_parts_constraint()->data()));
			}

			inline int CountTableIDPartsBonePoint(void)
			{
				return((int)(Value->table_id_parts_bone_point()->size()));
			}
			inline const Uint16* TableIDPartsBonePoint(void)
			{
				return((const Uint16*)(Value->table_id_parts_bone_point()->data()));
			}

			inline int CountTableIDPartsMesh(void)
			{
				return((int)(Value->table_id_parts_mesh()->size()));
			}
			inline const Uint16* TableIDPartsMesh(void)
			{
				return((const Uint16*)(Value->table_id_parts_mesh()->data()));
			}

			inline int CountTableIDPartsTransformConstraint(void)
			{
				return((int)(Value->table_id_parts_transform_constraint()->size()));
			}
			inline const Uint16* TableIDPartsTransformConstraint(void)
			{
				return((const Uint16*)(Value->table_id_parts_transform_constraint()->data()));
			}

			inline int CountTableIDPartsCamera(void)
			{
				return((int)(Value->table_id_parts_camera()->size()));
			}
			inline const Uint16* TableIDPartsCamera(void)
			{
				return((const Uint16*)(Value->table_id_parts_camera()->data()));
			}

			/* ----------------------------------------------- Operators */

			/* ----------------------------------------------- Friends */
		};

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		/* パーツ種別 */
		enum KindFeature	{
			/* ERROR/NON: -1 */

			ROOT = 0,	/* Root-Parts (Subspecies of "NULL"-Parts) */
			NULL_PARTS,	/* ※マクロのNULLと衝突するので、これだけ修飾がついています。 */
			NORMAL,

			INSTANCE,
			EFFECT,

			MASK,

			JOINT,
			BONE,
			MOVENODE,
			CONSTRAINT,
			BONEPOINT,
			MESH,

			TRANSFORM_CONSTRAINT,
			CAMERA,

			TERMINATOR,
		};

		/* コリジョン形状 */
		/* MEMO: C++版はコリジョンをサポートしていませんが、取得できるようにはしておきます。 */
		enum KindCollision	{
			COLLISION_NON = 0,
			COLLISION_SQUARE,
			COLLISION_AABB,
			COLLISION_CIRCLE,
			COLLISION_CIRCLE_SCALEMINIMUM,
			COLLISION_CIRCLE_SCALEMAXIMUM
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
		const ss::ssfb2::DataModelParts* Value;			/* 情報へのポインタ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline const char* Name(void) const
		{
			return(Value->name()->c_str());
		}
		inline int ID(void) const
		{
			return((int)(Value->id()));
		}
		inline int IDParent(void) const
		{
			return((int)(Value->id_parent()));
		}
		inline int CountTableIDChild(void) const
		{
			return((int)(Value->table_id_children()->size()));
		}
		inline const Sint32* TableIDChild(void) const
		{
			/* MEMO: Sint32はint32_tのエリアスなので、素直にキャストしています。 */
			return((Sint32*)(Value->table_id_children()->data()));
		}
		inline Sint32 IDGetChild(int index) const
		{	/* MEMO: TableIDChildを経由しない方が速い（取得テーブルが一時実体は困るため複製 */
			/*       しなくてはならないような）場合に使用する目的で設置しています。         */
			return((Sint32)((*(Value->table_id_children()))[index]));
		}
		inline KindFeature Feature(void) const
		{
			/* MEMO: Enumの並びは同じなので、そのままキャストしています。 */
			return((KindFeature)(Value->feature()));
		}
		inline int CountMesh(void) const
		{
			return((int)(Value->count_mesh()));
		}
		inline const BindMesh* Mesh(BindMesh* bindMesh) const
		{
			bindMesh->Value = Value->mesh();

			return(bindMesh);
		}
		inline const ColorLabel* LabelColor(ColorLabel* labelColor) const
		{
			labelColor->Value = Value->label_color();

			return(labelColor);
		}
		inline Library::KindOperationBlend::Kind OperationBlendTarget(void) const
		{
			/* MEMO: Enumの並びは同じなので、そのままキャストしています。 */
			return((Library::KindOperationBlend::Kind)(Value->operation_blend_target()));
		}

		inline KindCollision ShapeCollision(void) const
		{
			/* MEMO: Enumの並びは同じなので、そのままキャストしています。 */
			return((KindCollision)(Value->shape_collision()));
		}
		inline Float32 SizeCollisionZ(void) const
		{
			/* MEMO: Float32はfloatのエリアスなので、素直にキャストしています。 */
			return((Float32)(Value->size_collision()));
		}

		inline int IndexAnimationPackUnderControl(void) const
		{
			/* MEMO: 基本的に「return((int)((Sint16)Value->index_animation_pack_under_control()));」で記載できるのですが…… */
			/*       エラー値の明示のためと・符号判定をケチってUint16→Sint16→intのキャスト連段は実装系依存と解釈されること */
			/*       も多いので……明示的記載しています。                                                                    */
			Uint16 index = Value->index_animation_pack_under_control();
			return(((Uint16)0xffff == index) ? -1 : (int)index);
		}
		inline const char* NameAnimationUnderControl(void) const
		{
			return(Value->name_animation_under_control()->c_str());
		}

		inline int IndexEffectDataUnderControl(void) const
		{
			/* MEMO: 基本的に「return((int)((Sint16)Value->index_effect_under_control()));」で記載できるのですが…… */
			/*       エラー値の明示のためと・符号判定をケチってUint16→Sint16→intのキャスト連段は実装系依存と解釈さ */
			/*       れることも多いので……明示的に記載しています。                                                  */
			Uint16 index = Value->index_effect_under_control();
			return(((Uint16)0xffff == index) ? -1 : (int)index);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* エフェクト内モデルパーツ情報 */
	class Effect
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum KindFeature	{
			/* ERROR: -1 */
			ROOT = 0,									/* Root-Parts (Subspecies of "Particle"-Parts) */
			EMITTER,									/* Emitter */
			PARTICLE,									/* Particle *//* ※現時点未使用。実際にはSS6ConverterVer2の時点でEmitterに情報を混ぜ込まれて最適化削除されています。 */

			TERMINATOR
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
		const ss::ssfb2::DataEffectParts* Value;		/* 情報へのポインタ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline const char* Name(void)
		{
			return(Value->name()->c_str());
		}
		inline int ID(void)
		{
			return((int)(Value->id()));
		}
		inline int IDParent(void)
		{
			return((int)(Value->id_parent()));
		}
		inline int CountTableIDChild(void)
		{
			/* MEMO: ひょっとすると負の判定がいるかも……。 */
			return((int)(Value->table_id_child()->size()));
		}
		inline const int* TableIDChild(void)
		{
			/* MEMO: table_id_childはint32_tの配列なので注意。 */
			return((const int*)(Value->table_id_child()->data()));
		}
		inline KindFeature Feature(void)
		{
			/* MEMO: Enumの並びは同じなので、そのままキャストしています。 */
			return((KindFeature)(Value->feature()));
		}
		inline int IndexEmitter(void)
		{
			/* MEMO: ひょっとすると負の判定がいるかも……。 */
			return((int)(Value->index_emitter()));
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};
}	/* Parts */

/* セルマップ関係 */
namespace CellMap	{
/* 前方宣言 */
class Cell;

/* セルマップ情報 */
/* MEMO: 本来はCellMapをクラス名にするのが命名としてはスマートではあるのですが…… */
/*       記述としてクラス階層が深くなりすぎるので、こういう構造になっています。    */
class Body
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
public:
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::LibraryDataCellMap* Value;		/* 情報へのポインタ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline const char* Name(void)
	{
		return(Value->name()->c_str());
	}

	inline CPU::Vector2* SizeOriginal(CPU::Vector2* sizeOriginal)
	{
		sizeOriginal->Set(	Value->size_original()->x(),
							Value->size_original()->y()
						);

		return(sizeOriginal);
	}
	inline Texture::KindInterpolation Filter(void)
	{
		return((Texture::KindInterpolation)(Value->filter()));
	}
	inline Texture::KindWrap Wrap(void)
	{
		return((Texture::KindWrap)(Value->wrap()));
	}
	inline int CountTableCell(void)
	{
		return((int)(Value->table_cell()->size()));
	}
	Cell* TableCell(Cell* cell, int index);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* セル情報 */
class Cell
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* メッシュ情報 */
	class DataMesh
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum Constants	{
			COUNT_VERTEX_SURFACE = 3,					/* 平面を構成する頂点数: 三角 *//* ※現在は三角形のみ対応 */
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
		const ss::ssfb2::DataMesh* Value;					/* 情報へのポインタ */

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline int CountTableCoordinate(void)
		{
			return((int)(Value->table_coordinate()->size()));
		}
		inline CPU::Vector2* TableCoordinate(CPU::Vector2* coordinate, int index)
		{
			const ss::ssfb2::Vector2* original = (*(Value->table_coordinate()))[index];
			coordinate->Set(original->x(), original->y());

			return(coordinate);
		}

		inline int CountTableIndexVertex(void)
		{	/* MEMO: 必ずCountTableCoordinate*3になるのだが……念のため配列そのもののサイズを取る（乗算も絡まないし）。 */
			return((int)(Value->table_index_vertex()->size()));
		}
		inline const Uint16* TableIndexVertex(void)
		{
			/* MEMO: Uint16はuint16_tのエリアスなのでそのままキャストしています。 */
			/* MEMO: 乱暴にテーブルそのものを引っ張っているのは、使用機会として各頂点・各面をバラバラに参照することは殆どないので……。 */
			return((const Uint16*)Value->table_index_vertex()->data());
		}

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
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::Cell* Value;					/* 情報へのポインタ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline const char* Name(void)
	{
		return(Value->name()->c_str());
	}
	inline CPU::Rect* Rectangle(CPU::Rect* rect)
	{
		const ss::ssfb2::Rect* original = Value->rectangle();
		rect->SetLURD(	original->x_min(),
						original->y_min(),
						original->x_max(),
						original->y_max()
				);

		return(rect);
	}
	inline CPU::Vector2* Pivot(CPU::Vector2* pivot)
	{
		const ss::ssfb2::Vector2* original = Value->pivot();
		pivot->Set(	original->x(),
					original->y()
				);

		return(pivot);
	}

	inline bool IsMesh(void)
	{
		return(0 < Value->mesh()->table_coordinate()->size());	/* ? true : false */
	}
	inline DataMesh* Mesh(DataMesh* dataMesh)
	{
		dataMesh->Value = Value->mesh();

		return(dataMesh);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};
}	/* CellMap */

/* アニメーション関係 */
namespace Animation	{
/* 前方宣言 */
/* MEMO: 全部宣言すると煩雑になるので、本当に必要なものだけ行っています。 */
class Label;
class Parts;

/* アニメーションのソートモード */
/* MEMO: クラス的にはBodyの中で宣言されるのが構造上正しいのですが、 */
/*       使用する上でここにある方が望ましいため……。               */
enum KindModeSort	{
	PRIORITY = 0,				/* Attribute "Priority" */
	POSITION_Z,					/* Transformed Z-Position */
};

/* アニメーション情報 */
/* MEMO: 本来はAnimationをクラス名にするのが命名としてはスマートではあるのですが…… */
/*       記述としてクラス階層が深くなりすぎるので、こういう構造になっています。      */
class Body
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
public:
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::LibraryDataAnimation* Value;	/* 情報へのポインタ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline const char* Name(void)
	{
		return(Value->name()->c_str());
	}
	inline int FramePerSecond(void)
	{
		return((int)(Value->frame_per_second()));
	}
	inline int CountFrame(void)
	{
		return((int)(Value->count_frame()));
	}

	inline int SizeCanvasX(void)
	{
		return((int)(Value->size_canvas_x()));
	}
	inline int SizeCanvasY(void)
	{
		return((int)(Value->size_canvas_y()));
	}

	inline int FrameValidStart(void)
	{
		return((int)(Value->frame_valid_start()));
	}
	inline int FrameValidEnd(void)
	{
		return((int)(Value->frame_valid_end()));
	}
	inline int CountFrameValid(void)
	{
		return((int)(Value->count_frame_valid()));
	}

	inline int DepthIK(void)
	{
		return((int)(Value->depth_ik()));
	}
	inline KindModeSort ModeSort(void)
	{
		return((KindModeSort)(Value->mode_sort()));
	}

	inline int CountTableLabel(void)
	{
		return((int)(Value->table_label()->size()));
	}
	Label* TableLabel(Label* label, int index);

	inline int CountTableParts(void)
	{
		return((int)(Value->table_parts()->size()));
	}
	Parts* TableParts(Parts* parts, int idParts);

	void FrameRangeGet(	int* frameRangeStart, int* frameRangeEnd,
						const char* labelStart, int frameOffsetStart,
						const char* labelEnd, int frameOffsetEnd
					);
	int FrameGetLabel(int index);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* アニメーションラベル */
class Label
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	enum KindLabelReserved	{
		START = 0,	/* "_start" *//* START + INDEX_RESERVED */
		END,	/* "_end" *//* END + INDEX_RESERVED */

		TERMINATOR,
		INDEX_RESERVED = 0x10000000,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::Label* Value;					/* 情報へのポインタ */

	/* 静的変数 */
	/* MEMO: 直接参照しているコード箇所があるので……。 */
	static const char* const TableNameLabelReserved[KindLabelReserved::TERMINATOR];	/* 定義済みラベル */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline const char* Name(void)
	{
		return(Value->name()->c_str());
	}
	inline int Frame(void)
	{
		return((int)Value->frame());
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* アトリビュート関係 */
/* MEMO: アトリビュート関係は、デコードをキャッシュしたり・補間を計算で行うことがある関係もあって、 */
/*       FlatBuffersのバッファを直接覗いていません。                                                */
/*       FlatBuffersのデータと本クラス群の値との間を取り持つのは、基本的にLibary::Data::Pack空間内  */
/*       「AttributeExpand～」内に設定されている関数ポインタ（の実体）群になります。                */
/*       ※ただ、一部例外的にそれ以外の処理でデータをFlatBuffersから取得している処理があるので注意  */
/*         してください（主にはData::Animation::Body内のSetupアニメーションの取得など）。           */
namespace Attribute	{
	/* アトリビュート: ステータス（true/falseのビットで制御されるアトリビュートの集合体） */
	class Status
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum FlagBit	{
			VALID = 0x40000000,
			HIDE = 0x20000000,	/* Store as assistant data for skip useless processing. */

			FLIP_X = 0x08000000,
			FLIP_Y = 0x04000000,
			FLIP_TEXTURE_X = 0x02000000,
			FLIP_TEXTURE_Y = 0x01000000,

			ID_PARTS_NEXTDRAW = 0x00000fff,
			ID_PARTS_NEXTPREDRAW = 0x00fff000,

			CLEAR = 0x00000000,
			MASK = (VALID | HIDE | FLIP_X | FLIP_Y | FLIP_TEXTURE_X | FLIP_TEXTURE_Y | ID_PARTS_NEXTPREDRAW | ID_PARTS_NEXTDRAW),
			INITIAL = (VALID | ID_PARTS_NEXTDRAW | ID_PARTS_NEXTPREDRAW),
		};

		enum ShiftFlagBit	{
			SHIFT_ID_PARTS_NEXTDRAW = 0,
			SHIFT_ID_PARTS_NEXTPREDRAW = 12,
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
		Uint32 /* FlagBit */ Flags;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			Flags = FlagBit::CLEAR;
		}

		Status(void)
		{
			CleanUp();
		}
		Status(Uint32 flags)
		{
			Flags = flags;
		}

		~Status(void)
		{
		}

		inline void Duplicate(const Status& original)
		{
			Flags = original.Flags;
		}

		inline bool IsValid(void)
		{
			return(0 != (Flags & FlagBit::VALID));
		}
		inline bool IsHide(void)
		{
			return(0 != (Flags & FlagBit::HIDE));
		}
		inline bool IsFlipX(void)
		{
			return(0 != (Flags & FlagBit::FLIP_X));
		}
		inline bool IsFlipY(void)
		{
			return(0 != (Flags & FlagBit::FLIP_Y));
		}
		inline bool IsTextureFlipX(void)
		{
			return(0 != (Flags & FlagBit::FLIP_TEXTURE_X));
		}
		inline bool IsTextureFlipY(void)
		{
			return(0 != (Flags & FlagBit::FLIP_TEXTURE_Y));
		}
		inline int IDGetPartsNextDraw(void)
		{
			Uint32 data = Flags & FlagBit::ID_PARTS_NEXTDRAW;

			return((FlagBit::ID_PARTS_NEXTDRAW == data) ? (-1) : (int)data);
		}
		inline int IDGetPartsNextPreDraw(void)
		{
			Uint32 data = Flags & FlagBit::ID_PARTS_NEXTPREDRAW;

			return((FlagBit::ID_PARTS_NEXTPREDRAW == data) ? (-1) : (int)data >> (int)ShiftFlagBit::SHIFT_ID_PARTS_NEXTPREDRAW);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: セル */
	class Cell
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
	public:
		int IndexCellMap;
		int IndexCell;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			IndexCellMap = -1;
			IndexCell = -1;
		}

		Cell(void)
		{
			CleanUp();
		}
		Cell(int indexCellMap, int indexCell)
		{
			IndexCellMap = indexCellMap;
			IndexCell = indexCell;
		}

		~Cell(void)
		{
		}

		inline void Duplicate(const Cell& original)
		{
			IndexCellMap = original.IndexCellMap;
			IndexCell = original.IndexCell;
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: パーツカラー */
	class PartsColor
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
	public:
		/* MEMO: パーツカラーの頂点数はSS6PUではBootUp時に与えていたのですが…… */
		/*       実際上TERMINATOR2でしか使用しないのと・C#の都合的な面が大きかっ */
		/*       たので、C++版では固定で取っています。                           */
		Library::KindBoundBlend::Kind Bound;
		Library::KindOperationBlend::Kind Operation;
		CPU::Color VertexColor[Library::KindVertex::TERMINATOR2];
		Float32 RateAlpha[Library::KindVertex::TERMINATOR2];

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		PartsColor(void);
		PartsColor(Library::KindBoundBlend::Kind bound, Library::KindOperationBlend::Kind operation, const CPU::Color* vertexColor, const float* rateAlpha);

		~PartsColor(void);

		inline void CleanUp(void)
		{
			Bound = Library::KindBoundBlend::NON;
			Operation = Library::KindOperationBlend::MIX;
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
//				VertexColor[i].Set(CPU::Color::White);
				VertexColor[i].Set(CPU::Color::Clear);
				RateAlpha[i] = 1.0f;
			}
		}

		inline void BootUp(void)
		{
			Bound = Library::KindBoundBlend::NON;
			Operation = Library::KindOperationBlend::MIX;
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
//				VertexColor[i].Set(CPU::Color::White);
				VertexColor[i].Set(CPU::Color::Clear);
				RateAlpha[i] = 1.0f;
			}
		}

		inline void Duplicate(const PartsColor& original)
		{
			Bound = original.Bound;
			Operation = original.Operation;
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
				VertexColor[i].Set(original.VertexColor[i]);
				RateAlpha[i] = original.RateAlpha[i];
			}
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: 頂点変形 */
	class VertexCorrection
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
	public:
		/* MEMO: パーツカラーの頂点数はSS6PUではBootUp時に与えていたのですが…… */
		/*       実際上TERMINATOR2でしか使用しないのと・C#の都合的な面が大きかっ */
		/*       たので、C++版では固定で取っています。                           */
		CPU::Vector2 Coordinate[Library::KindVertex::TERMINATOR2];

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		VertexCorrection(void);

		~VertexCorrection(void);

		inline void CleanUp(void)
		{
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
				Coordinate[i].Set(CPU::Vector2::Zero);
			}
		}

		VertexCorrection(const CPU::Vector2* coordinate)
		{
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
				Coordinate[i].Set(coordinate[i]);
			}
		}

		inline void BootUp(void)
		{
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)
			{
				Coordinate[i].Set(CPU::Vector2::Zero);
			}
		}

		inline void Duplicate(const VertexCorrection& original)
		{
			for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)
			{
				Coordinate[i] = original.Coordinate[i];
			}
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: ユーザデータ */
	class UserData
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum FlagBit	{
			VALID = 0x40000000,

			COORDINATE = 0x00000004,
			TEXT = 0x00000008,
			RECTANGLE = 0x00000002,
			NUMBER = 0x00000001,

			CLEAR = 0x00000000,
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		/* MEMO: TextはUTF-8（BOMなし）型式（予定）です。                 */
		/*       ※FBの関係で型式が違ってしまった場合、ちょっと考えます。 */
		Uint32 /* FlagBit */ Flags;
		Sint32 NumberInt;
		CPU::Rect Rectangle;
		CPU::Vector2 Coordinate;
		const char* Text;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			Flags = FlagBit::CLEAR;
			NumberInt = 0;
			Rectangle.Set(CPU::Rect::Zero);
			Coordinate.Set(CPU::Vector2::Zero);
			Text = TextEmpty;
		}

		UserData(void)
		{
			CleanUp();
		}

		UserData(FlagBit flags, int numberInt, CPU::Rect& rectangle, CPU::Vector2& coordinate, const char* text)
		{
			Flags = flags;
			NumberInt = numberInt;
			Rectangle.Set(rectangle);
			Coordinate.Set(coordinate);
			Text = text;
		}

		inline void Duplicate(const UserData& original)
		{
			/* MEMO: Deep copy */
			Flags = original.Flags;
			NumberInt = original.NumberInt;
			Rectangle.Set(original.Rectangle);
			Coordinate.Set(original.Coordinate);
			/* MEMO: 文字列は（現時点）シャローコピーにしています。 */
			Text = original.Text;
		}

		inline bool IsValid(void) const
		{
			return(0 != (Flags & FlagBit::VALID));	/* ? true : false */
		}
		inline bool IsNumber(void) const
		{
			return(0 != (Flags & FlagBit::NUMBER));	/* ? true : false */
		}
		inline bool IsRectangle(void) const
		{
			return(0 != (Flags & FlagBit::RECTANGLE));	/* ? true : false */
		}
		inline bool IsCoordinate(void) const
		{
			return(0 != (Flags & FlagBit::COORDINATE));	/* ? true : false */
		}
		inline bool IsText(void) const
		{
			return(0 != (Flags & FlagBit::TEXT));	/* ? true : false */
		}
		inline Uint32 Number(void) const
		{
			return((Uint32)NumberInt);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: インスタンス再生 */
	class Instance
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum FlagBit	{
			VALID = 0x40000000,

			INDEPENDENT = 0x00000002,
			PINGPONG = 0x00000001,

			CLEAR = 0x00000000,
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		Uint32 /* FlagBit */ Flags;
		int PlayCount;
		Float32 RateTime;
		int OffsetStart;
		int OffsetEnd;
		const char* LabelStart;
		const char* LabelEnd;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			Flags = FlagBit::CLEAR;
			PlayCount = 1;
			RateTime = 1.0f;
			OffsetStart = 0;
			OffsetEnd = 0;
			LabelStart = TextEmpty;
			LabelEnd = TextEmpty;
		}

		Instance(FlagBit flags, int playCount, float rateTime, int offsetStart, int offsetEnd, const char* labelStart, const char* labelEnd)
		{
			Flags = flags;
			PlayCount = playCount;
			RateTime = (Float32)rateTime;
			OffsetStart = offsetStart;
			OffsetEnd = offsetEnd;
			LabelStart = labelStart;
			LabelEnd = labelEnd;
		}

		inline void Duplicate(const Instance& original)
		{
			/* MEMO: Deep copy */
			Flags = original.Flags;
			PlayCount = original.PlayCount;
			RateTime = original.RateTime;
			OffsetStart = original.OffsetStart;
			OffsetEnd = original.OffsetEnd;
			/* MEMO: 文字列は（現時点）シャローコピーにしています。 */
			LabelStart = original.LabelStart;
			LabelEnd = original.LabelEnd;
		}

		inline bool IsValid(void)
		{
			return(0 != (Flags & FlagBit::VALID));	/* ? true : false */
		}

		Instance(void)
		{
			CleanUp();
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: エフェクト再生 */
	class Effect
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum FlagBit	{
			VALID = 0x40000000,

			INDEPENDENT = 0x00000002,
			PINGPONG = 0x00000001,	/* (Reserved) */

			CLEAR = 0x00000000,
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		Uint32 /* FlagBit */ Flags;
		int FrameStart;
		Float32 RateTime;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			Flags = FlagBit::CLEAR;
			FrameStart = 0;
			RateTime = 1.0f;
		}

		Effect(FlagBit flags, int frameStart, float rateTime)
		{
			Flags = flags;
			FrameStart = frameStart;
			RateTime = (Float32)rateTime;
		}

		inline void Duplicate(const Effect& original)
		{
			Flags = original.Flags;
			FrameStart = original.FrameStart;
			RateTime = original.RateTime;
		}

		inline bool IsValid(void)
		{
			return(0 != (Flags & FlagBit::VALID));	/* ? true : false */
		}

		Effect(void)
		{
			CleanUp();
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: デフォーム */
	class Deform
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
	public:
		/* MEMO: デフォームは不定長なのでstd::vectorで持っています。                       */
		/*       ※現在コストが高いので、もう少しデータの持ち方を工夫するかもしれません。  */
		std::vector<CPU::Vector2> TableCoordinate;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		Deform(void);

		~Deform(void);

		Deform(const CPU::Vector2* tableVertex, int countVertex)
		{
			TableCoordinate.clear();
			if(TableCoordinate.capacity() < countVertex)	{
				TableCoordinate.resize(countVertex);
			}
			for(int i=0; i<countVertex; i++)	{
				TableCoordinate.push_back(tableVertex[i]);
			}
		}

		inline void CleanUp(void)
		{
			TableCoordinate.clear();
		}

		bool BootUp(int countVertex)
		{
			if(0 > countVertex)	{
				CleanUp();
			} else {
				if(TableCoordinate.capacity() < countVertex)	{
					TableCoordinate.resize(countVertex);
				}
				for(int i=0; i<countVertex; i++)	{
					TableCoordinate.push_back(CPU::Vector2::Zero);
				}
			}

			return(true);
		}

		inline void Duplicate(const Deform& original)
		{
			/* ディープコピーしています */
			/* MEMO: Deep copy */
			int countVertex = (int)(original.TableCoordinate.size());
			if(TableCoordinate.capacity() < countVertex)	{
				TableCoordinate.resize(countVertex);
			}
			for(int i=0; i<countVertex; i++)	{
				TableCoordinate.push_back(original.TableCoordinate[i]);
			}
		}

		bool IsValid(void)
		{
			return(0 <= TableCoordinate.size());	/* ? true : false */
		}

		void CoordinateReset(void)
		{
			int count = (int)(TableCoordinate.size());
			for(int i=0; i<count; i++)
			{
				TableCoordinate[i].Set(CPU::Vector2::Zero);
			}
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: Shader */
	class Shader
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:

		/* ----------------------------------------------- Enums & Constants */
	private:
	protected:
	public:
		enum Constant	{
			COUNT_PARAMETER = 4,						/* Parameterの配列長 */
		};

		/* ----------------------------------------------- Variables */
	private:
	protected:
	public:
		const char* ID;
		Float32 Parameter[Constant::COUNT_PARAMETER];

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		Shader(void);

		~Shader(void);

		inline void CleanUp(void)
		{
			ID = nullptr;
			for(int i=0; i<Constant::COUNT_PARAMETER; i++)	{
				Parameter[i] = 0.0f;
			}
		}
		bool BootUp(void);
		void Duplicate(const Shader& original);
		inline bool IsValid(void)
		{
			/* MEMO: 現在は名前があるか否かで簡略判定しています。 */
			return(nullptr != ID);	/* ? true : false */
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* アトリビュート: Signal */
	/* MEMO: シグナルはデータが大きいこともあるのと、扱いがアプリケーションに任されているため、 */
	/*       内容の複製は取らずに、そのまま情報を渡して・メンバ関数経由で内容を取得してもらう仕 */
	/*       様なので、他のアトリビュートクラスと実装形態が異なります。                         */
	class Signal
	{
		/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
	private:
	protected:
	public:
		class Command
		{
			/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
		private:
		protected:
		public:
			class Parameter
			{
				/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
			private:
			protected:
			public:

				/* ----------------------------------------------- Enums & Constants */
			private:
			protected:
			public:
				enum KindType	{
					_ERROR = -1,

					BOOL = 0,	/* Reserved. */
					INDEX,
					INTEGER,
					FLOATING,
					TEXT,	/* Reserved. */

					TERMINATOR
				};

				/* ----------------------------------------------- Variables */
			private:
			protected:
			public:
				const ss::ssfb2::AnimationAttributeSignalCommandParameter* Value;

				/* ----------------------------------------------- Functions */
			private:
			protected:
				inline void CleanUp(void)
				{
					Value = nullptr;
				}

				inline bool BootUp(void)
				{
					CleanUp();

					return(true);
				}

			public:
				Parameter(void)
				{
					CleanUp();
				}

				~Parameter(void)
				{
					CleanUp();
				}

				inline void Duplicate(const Parameter& original)
				{
					Value = original.Value;
				}

				inline bool IsValid(void) const
				{
					return(nullptr != Value);	/* ? true : false */
				}

#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
				inline const char* IDGet(void) const
				{
					if(false == IsValid())	{
						return(nullptr);
					}

					/* MEMO: ここ、最終的には文字列になる。 */
					return((Value->ID())->c_str());
				}
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
				inline int IDGet(void) const
				{
					if(false == IsValid())	{
						return(-1);
					}

					return(Value->ID());
				}
#endif

				inline KindType TypeGet(void) const
				{
					if(false == IsValid())	{
						return(KindType::_ERROR);
					}

					return((KindType)(Value->Type()));
				}

#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
				inline const char* NameGet(void) const
				{
					if(false == IsValid())	{
						return(nullptr);
					}

					return((Value->Name())->c_str());
				}
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
#endif

				inline bool ValueGetBool(void) const
				{
					if(KindType::BOOL != TypeGet())	{
						return(false);
					}

					const char* valueText = ValueGetText();
					if(nullptr == valueText)	{
						return(false);
					}

					int valueInt = atoi(valueText);
					return(0 != valueInt);	/* ? true  : false */
				}

				inline int ValueGetIndex(void) const
				{
					if(KindType::INDEX != TypeGet())	{
						return(-1);
					}

					const char* valueText = ValueGetText();
					if(nullptr == valueText)	{
						return(-1);
					}

					return(atoi(valueText));
				}

				inline int ValueGetInteger(void) const
				{
					if(KindType::INTEGER != TypeGet())	{
						return(0);
					}

					const char* valueText = ValueGetText();
					if(nullptr == valueText)	{
						return(0);
					}

					return(atoi(valueText));
				}

				inline float ValueGetFloating(void) const
				{
					if(KindType::FLOATING != TypeGet())	{
						return(0);
					}

					const char* valueText = ValueGetText();
					if(nullptr == valueText)	{
						return(0);
					}

					return((float)(atof(valueText)));
				}

				inline const char* ValueGetText(void) const
				{
					if(false == IsValid())	{
						return(nullptr);
					}

					return((Value->Data())->c_str());
				}

				/* ----------------------------------------------- Operators */

				/* ----------------------------------------------- Friends */
			};

			/* ----------------------------------------------- Enums & Constants */
		private:
		protected:
		public:
			enum FlagBit	{
				VALID = 0x40000000,
				ACTIVE= 0x20000000,

				CLEAR = 0x00000000,
			};

			/* ----------------------------------------------- Variables */
		private:
		protected:
		public:
			const ss::ssfb2::AnimationAttributeSignalCommand* Value;

			/* ----------------------------------------------- Functions */
		private:
		protected:
			inline void CleanUp(void)
			{
				Value = nullptr;
			}

			inline bool BootUp(void)
			{
				CleanUp();

				return(true);
			}

		public:
			Command(void)
			{
				CleanUp();
			}

			~Command(void)
			{
				CleanUp();
			}

			inline void Duplicate(const Command& original)
			{
				Value = original.Value;
			}

			inline bool IsValid(void) const
			{
				return(nullptr != Value);	/* ? true : false */
			}
			inline bool IsActive(void) const
			{
				if(false == IsValid())	{
					return(false);
				}

				return((FlagBit::VALID | FlagBit::ACTIVE) == (Value->Flags() & (FlagBit::VALID | FlagBit::ACTIVE)));	/* ? true : false */
			}

#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
			inline const char* IDGet(void) const
			{
				if(false == IsValid())	{
					return(nullptr);
				}

				return((Value->ID())->c_str());
			}
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
			inline int IDGet(void) const
			{
				if(false == IsValid())	{
					return(-1);
				}

				return(Value->ID());
			}
#endif
			inline const char* NoteGet(void) const
			{
				if(false == IsValid())	{
					return(nullptr);
				}

				return((Value->Note())->c_str());
			}

			inline int CountGetParameter(void) const
			{
				if(false == IsValid())	{
					return(-1);
				}

				return((int)((Value->TableParameter())->size()));
			}
#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
			inline int IndexGetParameter(const char* id) const;
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
			inline int IndexGetParameter(int id) const;
#endif
			inline Parameter* ParameterGet(Parameter* parameter, int index) const
			{
				if(nullptr == parameter)	{
					return(nullptr);
				}
				if((0 > index) || (CountGetParameter() <= index))	{
					return(nullptr);
				}

				parameter->Value = (*(Value->TableParameter()))[index];

				return(parameter);
			}

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
		const ss::ssfb2::AnimationAttributeSignal* Value;

		/* ----------------------------------------------- Functions */
	private:
	protected:
		inline void CleanUp(void)
		{
			Value = nullptr;
		}

		inline bool BootUp(void)
		{
			CleanUp();

			return(true);
		}

	public:
		Signal(void)
		{
			CleanUp();
		}

		~Signal(void)
		{
			CleanUp();
		}

		inline void Duplicate(const Signal& original)
		{
			Value = original.Value;
		}

		inline bool IsValid(void) const
		{
			return(nullptr != Value);	/* ? true : false */
		}

		inline int CountGetCommand(void) const
		{
			if(false == IsValid())	{
				return(-1);
			}

			return((int)((Value->TableCommand())->size()));
		}
#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
		inline int IndexGetCommand(const char* id) const;
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
		inline int IndexGetCommand(int id) const;
#endif
		inline Command* CommandGet(Command* command, int index) const
		{
			if(nullptr == command)	{
				return(nullptr);
			}
			if((0 > index) || (CountGetCommand() <= index))	{
				return(nullptr);
			}

			command->Value = (*(Value->TableCommand()))[index];

			return(command);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};
}	/* Attribute */

/* パーツ関係 */
class Parts
{
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
		NOT_USED = 0x20000000,
		HIDE_FULL = 0x10000000,

		NOT_MASKING = 0x08000000,

		NO_POSITION = 0x00800000,
		NO_ROTATION = 0x00400000,
		NO_SCALING = 0x00200000,
		NO_TRANSFORMATION_TEXTURE = 0x00100000,

		NO_USERDATA = 0x00080000,
		NO_PARTSCOLOR = 0x00040000,
		NO_INSTANCE = 0x00020000,
		NO_EFFECT = 0x00010000,

		NO_SIGNAL = 0x00008000,

		CLEAR = 0x00000000
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	Uint32 /* FlagBitStatus */ StatusParts;

	const ss::ssfb2::ContainerStatus* Status;
	const ss::ssfb2::ContainerCell* Cell;

	const ss::ssfb2::ContainerVector3* Position;
	const ss::ssfb2::ContainerVector3* Rotation;
	const ss::ssfb2::ContainerVector2* Scaling;
	const ss::ssfb2::ContainerVector2* ScalingLocal;

	const ss::ssfb2::ContainerFloat* RateOpacity;
	const ss::ssfb2::ContainerInt* Priority;
	const ss::ssfb2::ContainerPartsColor* PartsColor;
	const ss::ssfb2::ContainerVertexCorrection* VertexCorrection;

	const ss::ssfb2::ContainerVector2* OffsetPivot;
	const ss::ssfb2::ContainerVector2* PositionAnchor;
	const ss::ssfb2::ContainerVector2* SizeForce;
	const ss::ssfb2::ContainerVector2* PositionTexture;
	const ss::ssfb2::ContainerFloat* RotationTexture;
	const ss::ssfb2::ContainerVector2* ScalingTexture;

	const ss::ssfb2::ContainerFloat* RadiusCollision;

	const ss::ssfb2::ContainerUserData* UserData;
	const ss::ssfb2::ContainerInstance* Instance;
	const ss::ssfb2::ContainerEffect* Effect;
	const ss::ssfb2::ContainerDeform* Deform;
	const ss::ssfb2::ContainerShader* Shader;
	const ss::ssfb2::ContainerSignal* Signal;

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline void CleanUp(void)
	{
		StatusParts = FlagBitStatus::CLEAR;

		Status = nullptr;

		Cell = nullptr;

		Position = nullptr;
		Rotation = nullptr;
		Scaling = nullptr;
		ScalingLocal = nullptr;

		RateOpacity = nullptr;
		Priority = nullptr;
		PartsColor = nullptr;
		VertexCorrection = nullptr;

		OffsetPivot = nullptr;
		PositionAnchor = nullptr;
		SizeForce = nullptr;
		PositionTexture = nullptr;
		RotationTexture = nullptr;
		ScalingTexture = nullptr;

		RadiusCollision = nullptr;

		UserData = nullptr;
		Instance = nullptr;
		Effect = nullptr;
		Deform = nullptr;
		Shader = nullptr;
		Signal = nullptr;
	}

	Parts(void)
	{
		CleanUp();
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};
}	/* Animation */

/* エフェクト関係 */
namespace Effect	{
/* エフェクト情報 */
/* MEMO: 本来はEffectをクラス名にするのが命名としてはスマートではあるのですが…… */
/*       Animation・CellMapと併せた構造になっています。                           */
class Emitter
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:
	/* MEMO: 発生パターンと範囲データは現在値をキャッシュしていますが……後に変更するかもしれません。 */
	/* パーティクル発生パターン */
	class PatternEmit
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
	public:
		int IndexGenerate;
//		int Offset;
		int Duration;
		int Cycle;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline void CleanUp(void)
		{
			IndexGenerate = -1;
//			Offset = -1;
			Duration = -1;
			Cycle = -1;
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* 範囲情報: Float */
	class RangeFloat
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
	public:
		Float32 Main;
		Float32 Sub;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline RangeFloat* Set(const ss::ssfb2::RangeFloat* original)
		{
			Main = original->main();
			Sub = original->sub();

			return(this);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* 範囲情報: Vector2 */
	class RangeVector2
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
	public:
		CPU::Vector2 Main;
		CPU::Vector2 Sub;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline RangeVector2* Set(const ss::ssfb2::RangeVector2* original)
		{
			const ss::ssfb2::Vector2* originalVector2 = original->main();
			Main.Set(	originalVector2->x(),
						originalVector2->y()
				);

			originalVector2 = original->sub();
			Sub.Set(	originalVector2->x(),
						originalVector2->y()
				);

			return(this);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* 範囲情報: Color */
	class RangeColor
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
	public:
		CPU::Color Main;
		CPU::Color Sub;

		/* ----------------------------------------------- Functions */
	private:
	protected:
	public:
		inline RangeColor* Set(const ss::ssfb2::RangeColor* original)
		{
			const ss::ssfb2::Color* originalColor = original->main();
			Main.Set(	originalColor->r(),
						originalColor->g(),
						originalColor->b(),
						originalColor->a()
				); 

			originalColor = original->sub();
			Sub.Set(	originalColor->r(),
						originalColor->g(),
						originalColor->b(),
						originalColor->a()
				); 

			return(this);
		}

		/* ----------------------------------------------- Operators */

		/* ----------------------------------------------- Friends */
	};

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 定数群 */
	enum Constant	{
		SEED_MAGIC = 7573,

		LIFE_EXTEND_SCALE = 8,
		LIFE_EXTEND_MIN = 64,
	};

	/* 所持情報フラグ */
	enum FlagBit : Uint32	{
		/* for Particle */
		BASIC = 0x00000001,	/* (Reserved) */
		TANGENTIALACCELATION = 0x00000002,
		TURNDIRECTION = 0x00000004,
		SEEDRANDOM = 0x00000008,
		DELAY = 0x00000010,

		POSITION = 0x00000100,
		POSITION_FLUCTUATION = 0x00000200,	/* (Reserved) */
		ROTATION = 0x00000400,
		ROTATION_FLUCTUATION = 0x00000800,
		SCALE_START = 0x00001000,
		SCALE_END = 0x00002000,

		SPEED = 0x00010000,	/* (Reserved) */
		SPEED_FLUCTUATION = 0x00020000,
		GRAVITY_DIRECTION = 0x00040000,
		GRAVITY_POINT = 0x00080000,

		COLORVERTEX = 0x00100000,
		COLORVERTEX_FLUCTUATION = 0x00200000,
		FADEALPHA = 0x00400000,

		/* for Emitter */
		EMIT_INFINITE = 0x01000000,

		/* Mask-Bit and etc. */
		CLEAR = 0x00000000,
		MASK_EMITTER = 0x7f000000,
		MASK_PARTICLE = 0x00ffffff,
		MASK_VALID = 0x7fffffff,
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::DataEffectEmitter* Value;		/* 情報へのポインタ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline FlagBit FlagData(void) const
	{
		return((FlagBit)(Value->flag_data()));
	}
	inline Library::KindOperationBlendEffect::Kind OperationBlendTarget(void)
	{
		/* MEMO: Enumの並びは同じなので、そのままキャストしています。 */
		return((Library::KindOperationBlendEffect::Kind)(Value->operation_blend_target()));
	}
	inline int IndexCellMap(void) const
	{
		/* MEMO: ひょっとすると負の判定がいるかも……。 */
		return((int)(Value->index_cell_map()));
	}
	inline int IndexCell(void) const
	{
		/* MEMO: ひょっとすると負の判定がいるかも……。 */
		return((int)(Value->index_cell()));
	}
	inline RangeFloat* Angle(RangeFloat* angle) const
	{
		return(angle->Set(Value->angle()));
	}
	inline CPU::Vector2* GravityDirectional(CPU::Vector2* gravityDirectional) const
	{
		const ss::ssfb2::Vector2* original = Value->gravity_directional();
		gravityDirectional->Set(	original->x(),
									original->y()
							);

		return(gravityDirectional);
	}
	inline CPU::Vector2* GravityPointPosition(CPU::Vector2* gravityPointPosition) const
	{
		const ss::ssfb2::Vector2* original = Value->gravity_point_position();
		gravityPointPosition->Set(	original->x(),
									original->y()
								);

		return(gravityPointPosition);
	}
	inline Float32 GravityPointPower(void) const
	{
		return((Float32)(Value->gravity_point_power()));
	}
	inline RangeVector2* Position(RangeVector2* position) const
	{
		return(position->Set(Value->position()));
	}
	inline RangeFloat* Rotation(RangeFloat* rotation) const
	{
		return(rotation->Set(Value->rotation()));
	}
	inline RangeFloat* RotationFluctuation(RangeFloat* rotationFluctuation) const
	{
		return(rotationFluctuation->Set(Value->rotation_fluctuation()));
	}
	inline Float32 RotationFluctuationRate(void) const
	{
		return((Float32)(Value->rotation_fluctuation_rate()));
	}
	inline Float32 RotationFluctuationRateTime(void) const
	{
		return((Float32)(Value->rotation_fluctuation_rate_time()));
	}
	inline RangeFloat* RateTangentialAcceleration(RangeFloat* rateTangentialAcceleration) const
	{
		return(rateTangentialAcceleration->Set(Value->rate_tangential_acceleration()));
	}
	inline RangeVector2* ScaleStart(RangeVector2* scaleStart) const
	{
		return(scaleStart->Set(Value->scale_start()));
	}
	inline RangeFloat* ScaleRateStart(RangeFloat* scaleRateStart) const
	{
		return(scaleRateStart->Set(Value->scale_rate_start()));
	}
	inline RangeVector2* ScaleEnd(RangeVector2* scaleEnd) const
	{
		return(scaleEnd->Set(Value->scale_end()));
	}
	inline RangeFloat* ScaleRateEnd(RangeFloat* scaleRateEnd) const
	{
		return(scaleRateEnd->Set(Value->scale_rate_end()));
	}
	inline int Delay(void) const
	{
		return((int)(Value->delay()));
	}
	inline RangeColor* ColorVertex(RangeColor* colorVertex) const
	{
		return(colorVertex->Set(Value->color_vertex()));
	}
	inline RangeColor* ColorVertexFluctuation(RangeColor* colorVertexFluctuation) const
	{
		return(colorVertexFluctuation->Set(Value->color_vertex_fluctuation()));
	}
	inline Float32 AlphaFadeStart(void) const
	{
		return((Float32)(Value->alpha_fade_start()));
	}
	inline Float32 AlphaFadeEnd(void) const
	{
		return((Float32)(Value->alpha_fade_end()));
	}
	inline RangeFloat* Speed(RangeFloat* speed) const
	{
		return(speed->Set(Value->speed()));
	}
	inline RangeFloat* SpeedFluctuation(RangeFloat* speedFluctuation) const
	{
		return(speedFluctuation->Set(Value->speed_fluctuation()));
	}
	inline Float32 TurnDirectionFluctuation(void) const
	{
		return((Float32)(Value->turn_direction_fluctuation()));
	}
	inline Sint64 SeedRandom(void) const
	{
		return((Sint64)(Value->seed_random()));
	}
	inline int DurationEmitter(void) const
	{
		return((int)(Value->duration_emitter()));
	}
	inline int Interval(void) const
	{
		return((int)(Value->interval()));
	}
	inline RangeFloat* DurationParticle(RangeFloat* durationParticle) const
	{
#if 0	/* MEMO: Value->duration_particle()はnullptrの値を取り得るので、これはダメ（注意のために残してあります）。 */
		return(durationParticle->Set(Value->duration_particle()));
#else	/* MEMO: nullptrの時は内容を0で返す。 */
		const ss::ssfb2::RangeFloat* dataDurationParticle = Value->duration_particle();
		if(nullptr == dataDurationParticle)	{
			durationParticle->Main = 0.0f;
			durationParticle->Sub = 0.0f;

			return(durationParticle);
		}

		return(durationParticle->Set(dataDurationParticle));
#endif
	}
	inline int CountParticleMax(void) const
	{
		return((int)(Value->count_particle_max()));
	}
	inline int CountParticleEmit(void) const
	{
		return((int)(Value->count_particle_emit()));
	}
	inline int CountPartsMaximum(void) const
	{
		return((int)(Value->count_parts_maximum()));
	}
	inline int CountTablePatternEmit(void) const
	{
#if 0	/* MEMO: Value->table_pattern_emit()はnullptrの値を取り得るので、これはダメ（注意のために残してあります）。 */
		return((int)(Value->table_pattern_emit()->size()));
#else	/* MEMO: nullptrの時は0と返す。 */
		const flatbuffers::Vector<flatbuffers::Offset<ss::ssfb2::PatternEmit>>* tablePatternEmit = Value->table_pattern_emit();
		return((nullptr == tablePatternEmit) ? 0 : (int)tablePatternEmit->size());
#endif
	}
	inline PatternEmit* TablePatternEmit(PatternEmit* patternEmit, int index) const
	{	/* MEMO: この関数、「0 == CountTablePatternEmit()」の時は呼んではいけません。 */
		const ss::ssfb2::PatternEmit* original = (*(Value->table_pattern_emit()))[index];
		patternEmit->IndexGenerate = original->index_generate();
// 		patternEmit->Offset;
		patternEmit->Duration = original->duration();
		patternEmit->Cycle = original->cycle();

		return(patternEmit);
	}
	inline int CountTablePatternOffset(void) const
	{
#if 0	/* MEMO: Value->table_pattern_offset()はnullptrの値を取り得るので、これはダメ（注意のために残してあります）。 */
		return((int)(Value->table_pattern_offset()->size()));
#else	/* MEMO: nullptrの時は0と返す。 */
		const flatbuffers::Vector<int32_t>* tablePatternOffset = Value->table_pattern_offset();
		return((nullptr == tablePatternOffset) ? 0 : (int)tablePatternOffset->size());
#endif
	}
	inline const Sint32* TablePatternOffset(void) const
	{	/* MEMO: この関数、「0 == CountTablePatternOffset()」の時は呼んではいけません。 */
		return((const Sint32*)(Value->table_pattern_offset()->data()));
	}
	inline int CountTableSeedParticle(void) const
	{
#if 0	/* MEMO: Value->table_seed_particle()はnullptrの値を取り得るので、これはダメ（注意のために残してあります）。 */
		return((int)(Value->table_seed_particle()->size()));
#else	/* MEMO: nullptrの時は0と返す。 */
		const flatbuffers::Vector<int64_t>* tableSeedParticle = Value->table_seed_particle();
		return((nullptr == tableSeedParticle) ? 0 : (int)tableSeedParticle->size());
#endif
	}
	inline const Sint64* TableSeedParticle(void) const
	{	/* MEMO: この関数、「0 == CountTableSeedParticle()」の時は呼んではいけません。 */
		return((const Sint64*)(Value->table_seed_particle()->data()));
	}

	size_t SizeGetTablePatternEmit(int* countPatternEmit, int* countSeedRandom, int* countEmitMax, int* countEmit);
	void TableGetPatternEmit(	PatternEmit*& tablePatternEmit,
								Sint64*& tableSeedParticle,
								Library::Utility::Random::Generator& random,
								Uint32 seedRandom,
								void* workArea,
								int countPatternEmit,
								int countSeedRandom,
								int countEmitMax,
								int countEmit
							);

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};
}	/* Effect */

/* シーケンス関係 */
namespace Sequence	{
/* ステップ情報 */
class Step
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
public:
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::DataSequenceDetails* Value;	/* 情報へのポインタ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline const char* NameAnimationPack(void)
	{
		return((const char*)((Value->refAnimePack())->c_str()));
	}

	inline const char* NameAnimation(void)
	{
		return((const char*)((Value->refAnime())->c_str()));
	}

	inline int CountRepeat(void)
	{
		return((int)(Value->repeatCount()));
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

/* シーケンス情報 */
class Body
{
	/* ----------------------------------------------- Classes, Structs,Interfaces & Typedefs */
private:
protected:
public:

	/* ----------------------------------------------- Enums & Constants */
private:
protected:
public:
	/* 末尾での挙動 */
	enum KindTypePlayEed	{
		INVALID = -1,
		LAST,										/* 0: 最後のアイテムを繰り返し再生 */
		KEEP,										/* 1: 最終フレームを維持 */
		TOP,										/* 2: 全体を繰り返し再生 */
		TERMINATOR
	};

	/* ----------------------------------------------- Variables */
private:
protected:
public:
	/* MEMO: FlatBuffersの構造体群はfinalなので継承できないので包含しています。 */
	const ss::ssfb2::DataSequence* Value;			/* 情報へのポインタ */

	/* ----------------------------------------------- Functions */
private:
protected:
public:
	inline const char* Name(void)
	{
		return(Value->name()->c_str());
	}

	inline int Index(void)
	{
		return((int)(Value->index()));
	}

	inline KindTypePlayEed TypeGetPlayEnd(void)
	{
		return((KindTypePlayEed)(Value->type()));
	}

	inline int CountGetStep(void)
	{
		return((int)((Value->valueList())->size()));
	}
	inline Step* Step(Step* step, int index)
	{
		step->Value = (*(Value->valueList()))[index];

		return(step);
	}

	/* ----------------------------------------------- Operators */

	/* ----------------------------------------------- Friends */
};

}	/* Sequence */

/* -------------------------------------------------------------------------- */
/*                                                                  Variables */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                    Inline-Functions / Function-Type Macros */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                        Function Prototypes */
/* -------------------------------------------------------------------------- */

}	/* Data */
}	/* Library */
}	/* SpriteStudio6 */
