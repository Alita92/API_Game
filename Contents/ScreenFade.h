#pragma once
#include <Object.h>

// �з� :
// �뵵 :
// ���� :
class ScreenFade : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;
	float alpha_;
	

	enum class FadeEnum
	{
		FADEIN, // ȭ���� ������ ��
		FADEOUT, // ȭ���� �������� ��
		MAX
	};

	FadeEnum fadeEnum_;

public:
	void FadeIn();
	void FadeOut();
	void ChangeState(FadeEnum _state);

public:
	void LastCheck() override;
	void Update() override;
	void Render() override;


public:		
	ScreenFade(); // default constructer ����Ʈ ������
	~ScreenFade(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ScreenFade(const ScreenFade& _other) = delete; // default Copy constructer ����Ʈ ���������
	ScreenFade(ScreenFade&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ScreenFade& operator=(const ScreenFade& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ScreenFade& operator=(const ScreenFade&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

};

