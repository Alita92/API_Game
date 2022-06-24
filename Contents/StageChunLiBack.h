#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageChunLiBack : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	StageChunLiBack(); // default constructer ����Ʈ ������
	~StageChunLiBack(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageChunLiBack(const StageChunLiBack& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageChunLiBack(StageChunLiBack&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageChunLiBack& operator=(const StageChunLiBack& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageChunLiBack& operator=(const StageChunLiBack&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

