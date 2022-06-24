#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class ResultBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;



public:		
	ResultBackground(); // default constructer ����Ʈ ������
	~ResultBackground(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ResultBackground(const ResultBackground& _other) = delete; // default Copy constructer ����Ʈ ���������
	ResultBackground(ResultBackground&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ResultBackground& operator=(const ResultBackground& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ResultBackground& operator=(const ResultBackground&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void LastCheck() override;
};

