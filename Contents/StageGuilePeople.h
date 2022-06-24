#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageGuilePeople : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuilePeople(); // default constructer ����Ʈ ������
	~StageGuilePeople(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageGuilePeople(const StageGuilePeople& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageGuilePeople(StageGuilePeople&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageGuilePeople& operator=(const StageGuilePeople& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageGuilePeople& operator=(const StageGuilePeople&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

public :
	void AnimationInIt();

public:
	void ChangeAnimation(std::string _name);
};

