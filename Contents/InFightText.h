#pragma once
#include <Object.h>

class InFightText : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

	float interTime_;
	bool isTextShown_;

public:
	InFightText(); // default constructer ����Ʈ ������
	~InFightText(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	InFightText(const InFightText& _other) = delete; // default Copy constructer ����Ʈ ���������
	InFightText(InFightText&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	InFightText& operator=(const InFightText& _other) = delete; // default Copy operator ����Ʈ ���� ������
	InFightText& operator=(const InFightText&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetText(std::string _name);

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
};