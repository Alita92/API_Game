#pragma once

// 분류 :
// 용도 :
// 설명 :
class ResultPortraitP1
{
private:	// member Var

public:		
	ResultPortraitP1(); // default constructer 디폴트 생성자
	~ResultPortraitP1(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ResultPortraitP1(const ResultPortraitP1& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ResultPortraitP1(ResultPortraitP1&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ResultPortraitP1& operator=(const ResultPortraitP1& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ResultPortraitP1& operator=(const ResultPortraitP1&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

