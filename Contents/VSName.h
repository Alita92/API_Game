#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class VSName : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	VSName(); // default constructer ����Ʈ ������
	~VSName(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	VSName(const VSName& _other) = delete; // default Copy constructer ����Ʈ ���������
	VSName(VSName&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	VSName& operator=(const VSName& _other) = delete; // default Copy operator ����Ʈ ���� ������
	VSName& operator=(const VSName&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;

public:
	void AnimationInit();
	void SetVSName(std::string _name);
};

