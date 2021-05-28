/**
	SS6Player for CPP(C++/ssbp2)

	Copyright(C) Web Technology Corp.
	All rights reserved.
*/
/* -------------------------------------------------------------------------- */
/*                                                 Includes / Compile-Options */
/* -------------------------------------------------------------------------- */
#include "ss6player_data.h"
#include "ss6player_utility.h"

namespace SpriteStudio6	{
namespace Library	{
namespace Data	{
/* -------------------------------------------------------------------------- */
/*                          [File-Scope internal] Defines (Value-Type Macros) */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       [File-Scope internal] Classes / Structures / Typedef */
/* -------------------------------------------------------------------------- */

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
/*                                                           Parts::Animation */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// const char* Animation::Name(void) const
// int Animation::ID(void) const
// int Animation::IDParent(void) const
// const Sint32* Animation::TableIDChild(void) const
// Sint32 Animation::IDGetChild(int index) const
// Animation::KindFeature Animation::Feature(void) const
// int Animation::CountMesh(void) const
// const Animation::BindMesh* Animation::Mesh(BindMesh* bindMesh) const
// const Animation::ColorLabel* Animation::LabelColor(ColorLabel* labelColor) const
// Library::KindOperationBlend::Kind Animation::OperationBlendTarget(void) const
// Animation::KindCollision Animation::ShapeCollision(void) const
// Float32 Animation::SizeCollisionZ(void) const
// int Animation::IndexAnimationPackUnderControl(void) const
// const char* Animation::NameAnimationUnderControl(void) const
// int Animation::IndexEffectDataUnderControl(void) const
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                               Parts::Animation::ColorLabel */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// void Animation::ColorLabel::CleanUp(void)
// Animation::ColorLabel::ColorLabel(void)
// Animation::ColorLabel::ColorLabel(KindForm valueForm, CPU::Color32& valueColor)
// Animation::ColorLabel::KindForm Animation::ColorLabel::Form(void)
// const CPU::Color32* Animation::ColorLabel::Color(CPU::Color32* color)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                 Parts::Animation::BindMesh */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// int Animation::BindMesh::CountVertex(void)
// int Animation::BindMesh::CountTableVertex(void)
// const Animation::BindMesh::Vertex* Animation::BindMesh::TableVertex(Vertex* vertex)
// int Animation::BindMesh::CountTableRateUV(void)
// const Animation::BindMesh::RateUV* Animation::BindMesh::TableRateUV(RateUV* vertex)
// int Animation::BindMesh::CountTableIndexVertex(void)
// const Sint32* Animation::BindMesh::TableIndexVertex(void)
// int Animation::BindMesh::CountVertexDeform(void)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                         Parts::Animation::BindMesh::Vertex */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// int Animation::BindMesh::Vertex::CountTableBone(int index)
// const Animation::BindMesh::Vertex::Bone* Animation::BindMesh::Vertex::TableBone(Bone* bone, int index)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                   Parts::Animation::BindMesh::Vertex::Bone */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// int Animation::BindMesh::Vertex::Bone::CountData(void)
// int Animation::BindMesh::Vertex::Bone::Index(int index)
// Float32 Animation::BindMesh::Vertex::Bone::Weight(int index)
// CPU::Vector3* Animation::BindMesh::Vertex::Bone::CoordinateOffset(CPU::Vector3* coordinateOffset, int index)
// FPU::Vector4* Animation::BindMesh::CoordinateOffset(FPU::Vector4* coordinateOffset, int index)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                         Parts::Animation::BindMesh::RateUV */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// CPU::Vector2* Parts::Animation::BindMesh::RateUV::UV(CPU::Vector2* rateUV, int index)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                  Parts::Animation::Catalog */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// int Animation::Catalog::CountTableIDPartsNULL(void)
// const Uint16* Animation::Catalog::TableIDPartsNULL(void)
// int Animation::Catalog::CountTableIDPartsNormal(void)
// const Uint16* Animation::Catalog::TableIDPartsNormal(void)
// int Animation::Catalog::CountTableIDPartsInstance(void)
// const Uint16* Animation::Catalog::TableIDPartsInstance(void)
// int Animation::Catalog::CountTableIDPartsEffect(void)
// const Uint16* Animation::Catalog::TableIDPartsEffect(void)
// int Animation::Catalog::CountTableIDPartsMask(void)
// const Uint16* Animation::Catalog::TableIDPartsMask(void)
// int Animation::Catalog::CountTableIDPartsJoint(void)
// const Uint16* Animation::Catalog::TableIDPartsJoint(void)
// int Animation::Catalog::CountTableIDPartsBone(void)
// const Uint16* Animation::Catalog::TableIDPartsBone(void)
// int Animation::Catalog::CountTableIDPartsMoveNode(void)
// const Uint16* Animation::Catalog::TableIDPartsMoveNode(void)
// int Animation::Catalog::CountTableIDPartsConstraint(void)
// const Uint16* Animation::Catalog::TableIDPartsConstraint(void)
// int Animation::Catalog::CountTableIDPartsBonePoint(void)
// const Uint16* Animation::Catalog::TableIDPartsBonePoint(void)
// int Animation::Catalog::CountTableIDPartsMesh(void)
// const Uint16* Animation::Catalog::TableIDPartsMesh(void)
// int Animation::Catalog::CountTableIDPartsTransformConstraint(void)
// const Uint16* Animation::Catalog::TableIDPartsTransformConstraint(void)
// int Animation::Catalog::CountTableIDPartsCamera(void)
// const Uint16* Animation::Catalog::TableIDPartsCamera(void)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                              Parts::Effect */
/* -------------------------------------------------------------------------- */
namespace Parts	{
// const char* Effect::Name(void)
// int Effect::ID(void)
// int Effect::IDParent(void)
// int Effect::CountTableIDChild(void)
// const int* Effect::TableIDChild(void)
// Effect::KindFeature Effect::Feature(void)
// int Effect::IndexEmitter(void)
}	/* Parts */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                              CellMap::Body */
/* -------------------------------------------------------------------------- */
namespace CellMap	{
// const char* Body::Name(void)
// CPU::Vector2*  Body::SizeOriginal(CPU::Vector2* sizeOriginal)
// int  Body::CountTableCell(void)
// Texture::KindInterpolation Body::Filter(void)
// Texture::KindWrap Body::Wrap(void)
Cell*  Body::TableCell(Cell* cell, int index)
{	/* MEMO: 宣言順序の問題でインラインで書けない（後から調整するかも）。 */
	cell->Value = (*(Value->table_cell()))[index];

	return(cell);
}
}	/* CellMap */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                              CellMap::Cell */
/* -------------------------------------------------------------------------- */
namespace CellMap	{
// const char* Cell::Name(void)
// CPU::Rect* Cell::Rectangle(CPU::Rect* rect)
// CPU::Vector2* Cell::Pivot(CPU::Vector2* pivot)
// bool Cell::IsMesh(void)
// Cell::DataMesh* Cell::Mesh(DataMesh* dataMesh)
}	/* CellMap */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                    CellMap::Cell::DataMesh */
/* -------------------------------------------------------------------------- */
namespace CellMap	{
// int Cell::DataMesh::CountTableCoordinate(void)
// CPU::Vector2* Cell::DataMesh::TableCoordinateModel(CPU::Vector2* coordinate, int index)
// int Cell::DataMesh::CountTableIndexVertex(void)
// const Uint16* Cell::DataMesh::TableIndexVertex(void)
}	/* CellMap */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            Animation::Body */
/* -------------------------------------------------------------------------- */
namespace Animation	{
// const char* Body::Name(void)
// int Body::FramePerSecond(void)
// int Body::CountFrame(void)
// int Body::SizeCanvasX(void)
// int Body::SizeCanvasY(void)
// int Body::FrameValidStart(void)
// int Body::FrameValidEnd(void)
// int Body::CountFrameValid(void)
// int Body::DepthIK(void)
// KindModeSort Body::ModeSort(void)
// int Body::CountTableLabel(void)
Label* Body::TableLabel(Label* label, int index)
{	/* MEMO: 宣言順序の問題でインラインで書けない（後から調整するかも）。 */
	label->Value = (*(Value->table_label()))[index];

	return(label);
}
// int Body::CountTableParts(void)
Parts* Body::TableParts(Parts* parts, int idParts)
{	/* MEMO: パーツ情報はポインタを取得してエリアスしてしまう。                   */
	/*       もしメモリ消費量とコストが高いようなら、キャッシュしないのも手です。 */
	const ss::ssfb2::DataAnimationParts* original = (*(Value->table_parts()))[idParts];

	parts->StatusParts = (Uint32)(original->status_parts());

	parts->Status = original->status();

	parts->Cell = original->cell();

	parts->Position = original->position();
	parts->Rotation = original->rotation();
	parts->Scaling = original->scaling();
	parts->ScalingLocal = original->scalingLocal();

	parts->RateOpacity = original->rateOpacity();
	parts->Priority = original->priority();
	parts->PartsColor = original->partsColor();
	parts->VertexCorrection = original->vertexCorrection();

	parts->OffsetPivot = original->offsetPivot();
	parts->PositionAnchor = original->positionAnchor();
	parts->SizeForce = original->sizeForce();
	parts->PositionTexture = original->positionTexture();
	parts->RotationTexture = original->rotationTexture();
	parts->ScalingTexture = original->scalingTexture();

	parts->RadiusCollision = original->radiusCollision();

	parts->UserData = original->user_data();
	parts->Instance = original->instance();
	parts->Effect = original->effect();
	parts->Deform = original->deform();
	parts->Shader = original->shader();
	parts->Signal = original->signal();

	return(parts);
}
void Body::FrameRangeGet(	int* frameRangeStart, int* frameRangeEnd,
							const char* labelStart, int frameOffsetStart,
							const char* labelEnd, int frameOffsetEnd
					)
{
	int frameStart = Value->frame_valid_start();	/* 0 */
	int frameEnd = Value->frame_valid_end();	/* CountFrame - 1 */
	int indexLabel;

	/* 開始フレームの算出 */
	indexLabel = -1;
	if(false == Library::Utility::Text::IsNullOrEmpty(labelStart))	{	/* 空文字でない */
		/* MEMO: 後から生かす */
//		indexLabel = IndexGetLabel(labelStart);
	}
	if(0 > indexLabel)	{	/* 見つからない */
		indexLabel = (int)(Library::Data::Animation::Label::KindLabelReserved::START | Library::Data::Animation::Label::KindLabelReserved::INDEX_RESERVED);
	}

	*frameRangeStart = FrameGetLabel(indexLabel);
	if(frameStart > *frameRangeStart)	{
		*frameRangeStart = frameStart;
	}
	*frameRangeStart += frameOffsetStart;
	if((frameStart > *frameRangeStart) || (frameEnd < *frameRangeStart))
	{
		*frameRangeStart = frameStart;
	}

	/* 終了フレームの算出 */
	indexLabel = -1;
	if(false == Library::Utility::Text::IsNullOrEmpty(labelEnd))	{	/* 空文字でない */
		/* MEMO: 後から生かす */
//		indexLabel = IndexGetLabel(labelEnd);
	}
	if(0 > indexLabel)	{	/* 見つからない */
		indexLabel = (int)(Library::Data::Animation::Label::KindLabelReserved::END | Library::Data::Animation::Label::KindLabelReserved::INDEX_RESERVED);
	}

	*frameRangeEnd = FrameGetLabel(indexLabel);
	if(frameStart > *frameRangeEnd)	{
		*frameRangeEnd = frameEnd;
	}
	*frameRangeEnd += frameOffsetEnd;
	if((frameStart > *frameRangeEnd) || (frameEnd < *frameRangeEnd))
	{
		*frameRangeEnd = frameEnd;
	}
}

int Body::FrameGetLabel(int index)
{
	if(Label::KindLabelReserved::INDEX_RESERVED <= index)	{	/* 予約ラベルインデックス */
		index -= Label::KindLabelReserved::INDEX_RESERVED;
		switch(index)	{
			case Label::KindLabelReserved::START:
				return(0);

			case Label::KindLabelReserved::END:
				return(Value->count_frame() - 1);

			default:
				break;
		}
		return(-1);
	}

	if((0 > index) || (CountTableLabel() <= index))	{	/* 不良インデックス */
		return(-1);
	}

	Label dataLabel;
	TableLabel(&dataLabel, index);

	return(dataLabel.Frame());
}
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                           Animation::Label */
/* -------------------------------------------------------------------------- */
namespace Animation	{
/* MEMO: 他のソースでは静的変数の設置は前方で行っていますが、 */
/*       このソースに限っては名前空間が複雑なので、ここで。   */
const char* const Label::TableNameLabelReserved[KindLabelReserved::TERMINATOR] = {
	"_start",
	"_end",
};

// const char* Label::Name(void)
// int Label::Frame(void)
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                               Animation::Attribute::Status */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// void Status::CleanUp(void)
// Status::Status(void)
// Status::Status(Uint32 flags)
// void Status::Duplicate(Status original)
// bool Status::IsValid(void)
// bool Status::IsHide(void)
// bool Status::IsFlipX(void)
// bool Status::IsFlipY(void)
// bool Status::IsTextureFlipX(void)
// bool Status::IsTextureFlipY(void)
// int Status::IDGetPartsNextDraw(void)
// int Status::IDGetPartsNextPreDraw(void)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                 Animation::Attribute::Cell */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// void Cell::CleanUp(void)
// Cell::Cell(void)
// Cell::Cell(int indexCellMap, int indexCell)
// Cell::~Cell(void)
// void Cell::Duplicate(const Cell& original)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                      Animation::PartsColor */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
PartsColor::PartsColor(void)
{
	CleanUp();
}
PartsColor::PartsColor(Library::KindBoundBlend::Kind bound, Library::KindOperationBlend::Kind operation, const CPU::Color* vertexColor, const float* rateAlpha)
{
	Bound = bound;
	Operation = operation;
	for(int i=0; i<Library::KindVertex::TERMINATOR2; i++)	{
		VertexColor[i].Set(vertexColor[i]);
		RateAlpha[i] = (Float32)(rateAlpha[i]);
	}
}

PartsColor::~PartsColor(void)
{
}

// void PartsColor::CleanUp(void)
// PartsColor::PartsColor(Library::KindBoundBlend::Kind bound, Library::KindOperationBlend::Kind operation, const CPU::Color* vertexColor, const float* rateAlpha)
// void PartsColor::BootUp(void)
// void PartsColor::Duplicate(const PartsColor& original)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                Animation::VertexCorrection */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
VertexCorrection::VertexCorrection(void)
{
	CleanUp();
}

VertexCorrection::~VertexCorrection(void)
{
}

// void VertexCorrection::CleanUp(void)
// VertexCorrection::VertexCorrection(const CPU::Vector2* coordinate)
// void VertexCorrection::BootUp(void)
// void VertexCorrection::Duplicate(const VertexCorrection& original)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                        Animation::UserData */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// void UserData::CleanUp(void)
// UserData::UserData(void)
// UserData::UserData(FlagBit flags, int numberInt, CPU::Rect& rectangle, CPU::Vector2& coordinate, const char* text)
// void UserData::Duplicate(const UserData& original)
// bool UserData::IsValid(void) const
// bool UserData::IsNumber(void) const
// bool UserData::IsRectangle(void) const
// bool UserData::IsCoordinate(void) const
// bool UserData::IsText(void) const
// Uint32 UserData::Number(void) const
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                        Animation::Instance */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// Instance::Instance(void)
// void Instance::CleanUp(void)
// Instance::Instance(FlagBit flags, int playCount, float rateTime, int offsetStart, int offsetEnd, const char* labelStart, const char* labelEnd)
// void Instance::Duplicate(const Instance& original)
// bool Instance::IsValid(void)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          Animation::Effect */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// Effect::Effect(void)
// void Effect::CleanUp(void)
// Effect::Effect(FlagBit flags, int frameStart, float rateTime)
// void Effect::Duplicate(const Effect& original)
// bool Effect::IsValid(void)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          Animation::Deform */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
Deform::Deform(void)
{
	CleanUp();
}

Deform::~Deform(void)
{
}

// Deform::Deform(const CPU::Vector2* tableVertex, int countVertex)
// void Deform::CleanUp(void)
// bool Deform::BootUp(int countVertex)
// void Deform::Duplicate(const Deform& original)
// bool Deform::IsValid(void)
// void Deform::CoordinateReset(void)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          Animation::Shader */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
Shader::Shader(void)
{
	CleanUp();
}
Shader::~Shader(void)
{
}
// void Shader::CleanUp(void)
bool Shader::BootUp(void)
{
	CleanUp();

	ID = TextEmpty;

	return(true);
}
void Shader::Duplicate(const Shader& original)
{
	ID = original.ID;
	for(int i=0; i<Constant::COUNT_PARAMETER; i++)	{
		Parameter[i] = original.Parameter[i];
	}
}
// bool Shader::IsValid(void)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                          Animation::Signal::Command::Value */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// Signal::Command::Parameter::Parameter(void)
// Signal::Command::Parameter::~Parameter(void)
// void Signal::Command::Parameter::CleanUp(void)
// bool Signal::Command::Parameter::BootUp(void)
// void Signal::Command::Parameter::Duplicate(const Parameter& original)
// bool Signal::Command::Parameter::IsValid(void) const
// const char* Signal::Command::Parameter::IDGet(void) const
// KindType Signal::Command::Parameter::TypeGet(void) const
// const char* Signal::Command::Parameter::NameGet(void) const
// bool Signal::Command::Parameter::ValueGetBool(void) const
// int Signal::Command::Parameter::ValueGetIndex(void) const
// int Signal::Command::Parameter::ValueGetInteger(void) const
// float Signal::Command::Parameter::ValueGetFloating(void) const
// const char* Signal::Command::Parameter::ValueGetText(void) const
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                 Animation::Signal::Command */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// Signal::Command::Command(void)
// Signal::Command::~Command(void)
// void Signal::Command::CleanUp(void)
// bool Signal::Command::BootUp(void)
// void Signal::Command::Duplicate(const Command& original)
// bool Signal::Command::IsValid(void) const
// bool Signal::Command::IsActive(void) const
// const char* Signal::Command::IDGet(void) const
// const char* Signal::Command::NameGet(void) const
// const char* Signal::Command::NoteGet(void) const
// int Signal::Command::CountGetParameter(void) const
#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
int Signal::Command::IndexGetParameter(const char* id) const
{
	int count = CountGetParameter();
	if(0 > count)	{
		return(-1);
	}

	Parameter dataParameter;
	for(int i=0; i<count; i++)	{
		ParameterGet(&dataParameter, i);
		if(true == Library::Utility::Text::Compare(id, dataParameter.IDGet()))	{
			return(i);
		}
	}

	return(-1);
}
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
int Signal::Command::IndexGetParameter(int id) const
{
	int count = CountGetParameter();
	if(0 > count)	{
		return(-1);
	}

	Parameter dataParameter;
	for(int i=0; i<count; i++)	{
		ParameterGet(&dataParameter, i);
		if(dataParameter.IDGet() == id)	{
			return(i);
		}
	}

	return(-1);
}
#endif
// Parameter* Signal::Command::ParameterGet(Parameter* parameter, int index)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                          Animation::Signal */
/* -------------------------------------------------------------------------- */
namespace Animation	{
namespace Attribute	{
// Signal::Signal(void)
// Signal::~Signal(void)
// void Signal::CleanUp(void)
// bool Signal::BootUp(void)
// void Signal::Duplicate(const Signal& original)
// bool Signal::IsValid(void) const
// int Signal::CountGetCommand(void) const
#if 0	/* MEMO: ConverterVer2 1.0.0までの仕様 */
int Signal::IndexGetCommand(const char* id) const
{
	int count = CountGetCommand();
	if(0 > count)	{
		return(-1);
	}

	Command dataCommand;
	for(int i=0; i<count; i++)	{
		CommandGet(&dataCommand, i);
		if(true == Library::Utility::Text::Compare(id, dataCommand.IDGet()))	{
			return(i);
		}
	}

	return(-1);
}
#else	/* MEMO: ConverterVer2 1.1.0以降の仕様 */
int Signal::IndexGetCommand(int id) const
{
	int count = CountGetCommand();
	if(0 > count)	{
		return(-1);
	}

	Command dataCommand;
	for(int i=0; i<count; i++)	{
		CommandGet(&dataCommand, i);
		if(dataCommand.IDGet() == id)	{
			return(i);
		}
	}

	return(-1);
}
#endif
// Command* CommandGet(Command* command, int index)
}	/* Attribute */
}	/* Animation */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                            Effect::Emitter */
/* -------------------------------------------------------------------------- */
namespace Effect	{
// Emitter::FlagBit FlagData(void) const
// Library::KindOperationBlendEffect::Kind Emitter::OperationBlendTarget(void) const
// int Emitter::IndexCellMap(void) const
// int Emitter::IndexCell(void) const
// Emitter::RangeFloat* Emitter::Angle(RangeFloat* angle) const
// CPU::Vector2* Emitter::GravityDirectional(CPU::Vector2* gravityDirectional) const
// CPU::Vector2* Emitter::GravityPointPosition(CPU::Vector2* gravityPointPosition) const
// Float32 Emitter::GravityPointPower(void) const
// Emitter::RangeVector2* Emitter::Position(RangeVector2* position) const
// Emitter::RangeFloat* Emitter::Rotation(RangeFloat* rotation) const
// Emitter::RangeFloat* Emitter::RotationFluctuation(RangeFloat* rotationFluctuation) const
// Float32 Emitter::RotationFluctuationRate(void) const
// Float32 Emitter::RotationFluctuationRateTime(void) const
// Emitter::RangeFloat* Emitter::RateTangentialAcceleration(RangeFloat* rateTangentialAcceleration) const
// Emitter::RangeVector2* Emitter::ScaleStart(RangeVector2* scaleStart) const
// Emitter::RangeFloat* Emitter::ScaleRateStart(RangeFloat* scaleRateStart) const
// Emitter::RangeVector2* Emitter::ScaleEnd(RangeVector2* scaleEnd) const
// Emitter::RangeFloat* Emitter::ScaleRateEnd(RangeFloat* scaleRateEnd) const
// int Emitter::Delay(void) const
// Emitter::RangeColor* Emitter::ColorVertex(RangeColor* colorVertex) const
// Emitter::RangeColor* Emitter::ColorVertexFluctuation(RangeColor* colorVertexFluctuation) const
// Float32 Emitter::AlphaFadeStart(void) const
// Float32 Emitter::AlphaFadeEnd(void) const
// Emitter::RangeFloat* Emitter::Speed(RangeFloat* speed) const
// Emitter::RangeFloat* Emitter::SpeedFluctuation(RangeFloat* speedFluctuation) const
// Float32 Emitter::TurnDirectionFluctuation(void) const
// Sint64 Emitter::SeedRandom(void) const
// int Emitter::DurationEmitter(void) const
// int Emitter::Interval(void) const
// Emitter::RangeFloat* Emitter::DurationParticle(RangeFloat* durationParticle) const
// int Emitter::CountParticleMax(void) const
// int Emitter::CountParticleEmit(void) const
// int Emitter::CountPartsMaximum(void) const
// int Emitter::CountTablePatternEmit(void) const
// Emitter::PatternEmit* Emitter::TablePatternEmit(PatternEmit* patternEmit, int index) const
// int Emitter::CountTablePatternOffset(void) const
// const int* Emitter::TablePatternOffset(void) const
// int Emitter::CountTableSeedParticle(void) const
// const Sint64* Emitter::TableSeedParticle(void) const

/* ********************************************************* */
//! エミットパターンテーブル用ワークエリア長を取得
/* --------------------------------------------------------- */
/* MEMO:
 *
 */
size_t Effect::Emitter::SizeGetTablePatternEmit(int* countPatternEmit, int* countSeedRandom, int* countEmitMax, int* countEmit)
{
	int count;
	*countEmitMax = CountParticleMax();
	*countEmit = CountParticleEmit();
	*countEmit = (1 > *countEmit) ? 1 : *countEmit;

	/* エミットパターンテーブル数を算出 */
	count = (*countEmitMax) * Constant::LIFE_EXTEND_SCALE;
	if(Constant::LIFE_EXTEND_MIN > count)	{
		count = Constant::LIFE_EXTEND_MIN;
	}
	*countPatternEmit = count;

	/* 乱数種テーブルを算出 */
	*countSeedRandom = *countEmitMax * 3;

	/* ワークエリア長を算出 */
	size_t sizeWorkArea =	Memory::Alignment(sizeof(PatternEmit) * (*countPatternEmit))
							+ Memory::Alignment(sizeof(Sint64) * (*countSeedRandom));
	return(sizeWorkArea);
}

/* ********************************************************* */
//! エミットパターンテーブル群を作成
/* --------------------------------------------------------- */
/* MEMO:
 * 乱数が固定されている場合はSS6CaonverterVer2が事前計算で作成
 * したテーブルを使用するので、本関数は乱数が固定されていない
 * 時の動的生成でのみ使用します。
 * TablePatternOffsetは本関数の前に作成しておいてください。
 * 
 * workAreaの実体はSizeGetTablePatternEmit関数の関数値で得ら
 * れたバイトサイズが静的に確保されている必要があります。
 * 本関数のworkArea以降の引数は、必ずSizeGetTablePatternEmitで
 * 取得した値を与えるようにしてください。
 */
void Effect::Emitter::TableGetPatternEmit(	PatternEmit*& tablePatternEmit,
											Sint64*& tableSeedParticle,
											Library::Utility::Random::Generator& random,
											Uint32 seedRandom,
											void* workArea,
											int countPatternEmit,
											int countSeedRandom,
											int countEmitMax,
											int countEmit
										)
{
	Uint8* workAreaNow = reinterpret_cast<Uint8*>(workArea);

	/* パーティクル生成パターンテーブルを作成 */
	tablePatternEmit = reinterpret_cast<PatternEmit*>(workAreaNow);

	PatternEmit* patternEmit;
	int cycle = (int)(((float)(countEmitMax * Interval()) / (float)countEmit) + 0.5f);
	int duration;
	RangeFloat rangeDurationParticle;
	DurationParticle(&rangeDurationParticle);
	for(int i=0; i<countPatternEmit; i++)	{
		
		patternEmit = &tablePatternEmit[i];
		patternEmit->IndexGenerate = i;
		duration = (int)((float)rangeDurationParticle.Main + random.RandomFloat((float)rangeDurationParticle.Sub));
		patternEmit->Duration = duration;
		patternEmit->Cycle = (duration > cycle) ? duration : cycle;
	}
	workAreaNow += Memory::Alignment(sizeof(PatternEmit) * countPatternEmit);

	/* 乱数種テーブルを作成 */
	tableSeedParticle = reinterpret_cast<Sint64*>(workAreaNow);

	random.InitSeed(seedRandom);
	for(int i=0; i<countSeedRandom; i++)	{
		tableSeedParticle[i] = (Sint64)((Uint64)random.RandomUint32());
	}
	workAreaNow += Memory::Alignment(sizeof(Sint64) * countSeedRandom);
}

}	/* Effect */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                               Effect::Emitter::PatternEmit */
/* -------------------------------------------------------------------------- */
namespace Effect	{
// Emitter::RangeFloat* Emitter::RangeFloat::Set(const ss::ssfb2::RangeFloat* original)
}	/* Effect */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                Effect::Emitter::RangeFloat */
/* -------------------------------------------------------------------------- */
namespace Effect	{
// Emitter::RangeVector2* Emitter::RangeVector2::Set(const ss::ssfb2::RangeVector2* original)
}	/* Effect */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                              Effect::Emitter::RangeVector2 */
/* -------------------------------------------------------------------------- */
namespace Effect	{
// Emitter::RangeColor* Emitter::RangeColor::Set(const ss::ssfb2::RangeColor* original)
}	/* Effect */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                Effect::Emitter::RangeColor */
/* -------------------------------------------------------------------------- */
namespace Effect	{
}	/* Effect */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                             Sequence::Step */
/* -------------------------------------------------------------------------- */
namespace Sequence	{
// const char* Step::NameAnimationPack(void)
// const char* Step::NameAnimation(void)
// int Step::CountRepeat(void)
}	/* Sequence */

/* -------------------------------------------------------------------------- */
/*                                                                  Functions */
/*                                                             Sequence::Body */
/* -------------------------------------------------------------------------- */
namespace Sequence	{
// const char* Body::Name(void)
// int Body::Index(void)
// Body::KindTypePlayEed Body::TypeGetPlayEnd(void)
// int Body::CountGetStep(void)
// Step* Body::Step(Step* step, int index)
}	/* Sequence */

}	/* Data */
}	/* Library */
}	/* SpriteStudio6 */
