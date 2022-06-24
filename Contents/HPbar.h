#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :

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
	HPbar(); // default constructer 디폴트 생성자
	~HPbar(); // default destructer 디폴트 소멸자

public:		// delete constructer
	HPbar(const HPbar& _other) = delete; // default Copy constructer 디폴트 복사생성자
	HPbar(HPbar&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	HPbar& operator=(const HPbar& _other) = delete; // default Copy operator 디폴트 대입 연산자
	HPbar& operator=(const HPbar&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

