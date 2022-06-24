#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class FightScene;
class RefereeText : public Object
{
	friend FightScene;
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;



public:		
	RefereeText(); // default constructer ����Ʈ ������
	~RefereeText(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	RefereeText(const RefereeText& _other) = delete; // default Copy constructer ����Ʈ ���������
	RefereeText(RefereeText&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	RefereeText& operator=(const RefereeText& _other) = delete; // default Copy operator ����Ʈ ���� ������
	RefereeText& operator=(const RefereeText&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

public:
	void ChangeText(std::string _imageName);
	SpriteRenderer* GetMainSpriteRenderer()
	{
		return mainSpriteRenderer_;
	}
};

