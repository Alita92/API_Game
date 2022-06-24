#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :

class Cursor;
class SelectionPortrait : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	Cursor* parentCursor_;

public:
	void AnimationInit();
	void PlayerInit();

	void SetParentCursor(Cursor* _cursor)
	{
		parentCursor_ = _cursor;
	}

	void ChangePortrait(std::string _name);

public:		
	SelectionPortrait(); // default constructer ����Ʈ ������
	~SelectionPortrait(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SelectionPortrait(const SelectionPortrait& _other) = delete; // default Copy constructer ����Ʈ ���������
	SelectionPortrait(SelectionPortrait&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SelectionPortrait& operator=(const SelectionPortrait& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SelectionPortrait& operator=(const SelectionPortrait&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
};

