#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class Winpoint : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	Winpoint(); // default constructer ����Ʈ ������
	~Winpoint(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Winpoint(const Winpoint& _other) = delete; // default Copy constructer ����Ʈ ���������
	Winpoint(Winpoint&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Winpoint& operator=(const Winpoint& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Winpoint& operator=(const Winpoint&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

};

