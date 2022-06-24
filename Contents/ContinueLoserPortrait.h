#pragma once
#include <Object.h>

// �з� :
// �뵵 :
// ���� :
class ContinueLoserPortrait : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	ContinueLoserPortrait(); // default constructer ����Ʈ ������
	~ContinueLoserPortrait(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ContinueLoserPortrait(const ContinueLoserPortrait& _other) = delete; // default Copy constructer ����Ʈ ���������
	ContinueLoserPortrait(ContinueLoserPortrait&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ContinueLoserPortrait& operator=(const ContinueLoserPortrait& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ContinueLoserPortrait& operator=(const ContinueLoserPortrait&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

public:
	void AnimationInit();
	void ChangePortrait(std::string _name);
};

