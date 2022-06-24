#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class VSPortrait : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	VSPortrait(); // default constructer ����Ʈ ������
	~VSPortrait(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	VSPortrait(const VSPortrait& _other) = delete; // default Copy constructer ����Ʈ ���������
	VSPortrait(VSPortrait&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	VSPortrait& operator=(const VSPortrait& _other) = delete; // default Copy operator ����Ʈ ���� ������
	VSPortrait& operator=(const VSPortrait&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	
public:
	void AnimationInit();
	void SetPortrait(std::string _name);
};

