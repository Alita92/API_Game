#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :

class CharacterHandle;
class HPbar : public Object
{
private:
	friend CharacterHandle;

private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	CharacterHandle* parentCharacter_;

	int barLength_;
	float barMoveSpeed_;
	float p2debugFloat_;
	float p2HpDecliner_;
	int damagedBar_;

	float interTime_;

	float originalPosX_;
	float originalPosY_;

public:
	void SetOriginalPos(float _x, float _y)
	{
		originalPosX_ = _x;
		originalPosY_ = _y;
	}

public:		
	HPbar(); // default constructer ����Ʈ ������
	~HPbar(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	HPbar(const HPbar& _other) = delete; // default Copy constructer ����Ʈ ���������
	HPbar(HPbar&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	HPbar& operator=(const HPbar& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HPbar& operator=(const HPbar&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
public:
	void HealthCheckUpdate();
	void SetParentCharacter(CharacterHandle* _parent)
	{
		parentCharacter_ = _parent;
	}
};

