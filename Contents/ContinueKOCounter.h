#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class ContinueKOCounter : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	ContinueKOCounter(); // default constructer ����Ʈ ������
	~ContinueKOCounter(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ContinueKOCounter(const ContinueKOCounter& _other) = delete; // default Copy constructer ����Ʈ ���������
	ContinueKOCounter(ContinueKOCounter&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ContinueKOCounter& operator=(const ContinueKOCounter& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ContinueKOCounter& operator=(const ContinueKOCounter&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

	void AnimationInit();
	void ChangeNumber(std::string _number);
};

