#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class WinnerTexth : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;

public:		
	WinnerTexth(); // default constructer ����Ʈ ������
	~WinnerTexth(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	WinnerTexth(const WinnerTexth& _other) = delete; // default Copy constructer ����Ʈ ���������
	WinnerTexth(WinnerTexth&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	WinnerTexth& operator=(const WinnerTexth& _other) = delete; // default Copy operator ����Ʈ ���� ������
	WinnerTexth& operator=(const WinnerTexth&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void LastCheck() override;
};

