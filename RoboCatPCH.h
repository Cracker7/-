#include "RoboCatShared.h"

// ������ Ÿ���� ��Ÿ���� ������ Ŭ����
enum EPrimitiveType
{
	EPT_Int,
	EPT_String,
	EPT_Float,
};

// RoboCat ��ü�� ��� ������ �����ϴ� Ŭ����. ��� ���� �̸�, ������ Ÿ��, ��ü ���� offset ���� ���� ����
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

// ����ȭ�Ϸ��� RoboCat ��ü�� ������ Ÿ���� �����ϴ� Ŭ����
// std::vector<MemberVariable> ���·� RoboCat ��ü ���� ��� ���� ����Ʈ�� ����
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

// ��ü ���� Ư�� ��� ������ offset ���� ����ϴ� ��ũ�� �Լ�
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