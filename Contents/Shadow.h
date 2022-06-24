#pragma once
#include <Object.h>
#include "CharacterHandle.h"
// �з� :
// �뵵 :
// ���� :
class Shadow : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	CharacterHandle* parentCharacter_;

public:
	void SetParentCharacter(CharacterHandle* _parent)
	{
		parentCharacter_ = _parent;
	}

	void PosInit();

public:		
	Shadow(); // default constructer ����Ʈ ������
	~Shadow(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Shadow(const Shadow& _other) = delete; // default Copy constructer ����Ʈ ���������
	Shadow(Shadow&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Shadow& operator=(const Shadow& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Shadow& operator=(const Shadow&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
};

