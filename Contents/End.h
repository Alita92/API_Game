#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class End : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	End(); // default constructer ����Ʈ ������
	~End(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	End(const End& _other) = delete; // default Copy constructer ����Ʈ ���������
	End(End&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	End& operator=(const End& _other) = delete; // default Copy operator ����Ʈ ���� ������
	End& operator=(const End&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
};

