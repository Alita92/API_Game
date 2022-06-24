#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class ResultPortrait : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:
	bool isWin_;
public:		
	ResultPortrait(); // default constructer ����Ʈ ������
	~ResultPortrait(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ResultPortrait(const ResultPortrait& _other) = delete; // default Copy constructer ����Ʈ ���������
	ResultPortrait(ResultPortrait&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ResultPortrait& operator=(const ResultPortrait& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ResultPortrait& operator=(const ResultPortrait&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void portraitInit();

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void Collision() override;
};

