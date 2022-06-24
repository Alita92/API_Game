#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class TitleTypoSprites : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	TitleTypoSprites(); // default constructer ����Ʈ ������
	~TitleTypoSprites(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	TitleTypoSprites(const TitleTypoSprites& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleTypoSprites(TitleTypoSprites&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	TitleTypoSprites& operator=(const TitleTypoSprites& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleTypoSprites& operator=(const TitleTypoSprites&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
	void ChangeSprite();
};

