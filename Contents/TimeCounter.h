#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class TimeCounter : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;
	float unitTime_;
	int roundTime_;

	enum class DIGITSTYLE
	{
		TENDIGIT,
		ONEDIGIT,
		MAX
	};

	std::string digitsTag_;
	DIGITSTYLE friendDigits_;

public:		
	TimeCounter(); // default constructer ����Ʈ ������
	~TimeCounter(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	TimeCounter(const TimeCounter& _other) = delete; // default Copy constructer ����Ʈ ���������
	TimeCounter(TimeCounter&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	TimeCounter& operator=(const TimeCounter& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TimeCounter& operator=(const TimeCounter&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

public:
	void spriteInit();
	void timeInit();
};

