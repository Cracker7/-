#include "RoboCatShared.h"

// 데이터 타입을 나타내는 열거형 클래스
enum EPrimitiveType
{
	EPT_Int,
	EPT_String,
	EPT_Float,
};

// RoboCat 객체의 멤버 변수를 저장하는 클래스. 멤버 변수 이름, 데이터 타입, 객체 내의 offset 값을 갖고 있음
class MemberVariable
{
public:
	MemberVariable(const char* inName, EPrimitiveType inPrimitiveType, uint32_t inOffset) :
		mName(inName), mPrimitiveType(inPrimitiveType), mOffset(inOffset) {}

	EPrimitiveType GetPrimitiveType() const { return mPrimitiveType; }
	uint32_t GetOffset() const { return mOffset; }
	//private:
	std::string mName;
	EPrimitiveType mPrimitiveType;
	uint32_t mOffset;
};

// 직렬화하려는 RoboCat 객체의 데이터 타입을 저장하는 클래스
// std::vector<MemberVariable> 형태로 RoboCat 객체 내의 멤버 변수 리스트를 저장
class DataType {
public:
	DataType(std::initializer_list<MemberVariable> inMVs) :
		mMemberVariables(inMVs) {}
	const std::vector<MemberVariable>& GetMemberVariables() const
	{
		return mMemberVariables;
	}
private:
	std::vector<MemberVariable> mMemberVariables;
};

// 객체 내의 특정 멤버 변수의 offset 값을 계산하는 매크로 함수
#define OffsetOf(c, mv) ((size_t)&(((c*)0)->mv))

class RoboCat {
public:
	RoboCat() : mHealth(10), mMeowCount(3) {}
	void Serialize(MemoryStream* stream) const
	{
		stream->Serialize(mHealth);
		stream->Serialize(mMeowCount);
	}
	static DataType* sDataType;
	static void InitDataType() {
		sDataType = new DataType({
			MemberVariable("mHealth", EPT_Int, OffsetOf(RoboCat, mHealth)),
			MemberVariable("mMeowCount", EPT_Int, OffsetOf(RoboCat, mMeowCount))
			});
	}
private:
	uint32_t mHealth;
	uint32_t mMeowCount;
};

DataType* RoboCat::sDataType = nullptr;