#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class SelectionFlag : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	SelectionFlag(); // default constructer ����Ʈ ������
	~SelectionFlag(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SelectionFlag(const SelectionFlag& _other) = delete; // default Copy constructer ����Ʈ ���������
	SelectionFlag(SelectionFlag&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SelectionFlag& operator=(const SelectionFlag& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SelectionFlag& operator=(const SelectionFlag&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	
public:
	void AnimationInit();
	void PlayerInit();

public:
	void ChangeFlagAndPos(std::string _flag, float4 _pos);
};

