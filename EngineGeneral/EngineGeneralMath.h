#pragma once
#include <math.h>
#include <random>
#include <string>

	class Random
	{
		// 시드역할을 담당하는 클래스
	private:
		std::mt19937_64 mt_;

	public:
		int RandomInt(int _min, int _max)
		{
			// 보통 대부분의 난수 알고리즘이
			// 마지막 제한을 할때 % 사용하므로
			// 보통 _max -1까지 나온다는것을 항상 염두해두자...
			std::uniform_int_distribution<int> Dis(_min, _max);
			return Dis(mt_);
		}

		float RandomFloat(float _min, float _max)
		{
			std::uniform_real_distribution<float> Dis(_min, _max);
			return Dis(mt_);
		}

	public:
		Random()
			: mt_()
		{
			std::random_device rng_;
			mt_.seed(rng_());
		}


		Random(__int64 _Seed)
			: mt_(_Seed)
		{

		}
		~Random()
		{

		}
	};

class float4
{
public:
	static const float4 ZERO;
	static const float4 LEFT;
	static const float4 RIGHT;
	static const float4 RIGHTUP;
	static const float4 UP;
	static const float4 DOWN;

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};

	float4 operator+(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x + _other.x;
		ReturnValue.y = this->y + _other.y;
		ReturnValue.z = this->z + _other.z;
		ReturnValue.w = this->w + _other.w;
		return ReturnValue;
	}

	float4 operator-(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x - _other.x;
		ReturnValue.y = this->y - _other.y;
		ReturnValue.z = this->z - _other.z;
		ReturnValue.w = this->w - _other.w;
		return ReturnValue;
	}

	float4 operator*(const float _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x * _other;
		ReturnValue.y = this->y * _other;
		ReturnValue.z = this->z * _other;
		ReturnValue.w = this->w * _other;
		return ReturnValue;
	}



	float4 operator*(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x * _other.x;
		ReturnValue.y = this->y * _other.y;
		ReturnValue.z = this->z * _other.z;
		ReturnValue.w = this->w * _other.w;
		return ReturnValue;
	}

	float4 operator/(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x / _other.x;
		ReturnValue.y = this->y / _other.y;
		ReturnValue.z = this->z / _other.z;
		ReturnValue.w = this->w / _other.w;
		return ReturnValue;
	}


	float4& operator+=(const float4 _other)
	{
		this->x += _other.x;
		this->y += _other.y;
		this->z += _other.z;
		this->w += _other.w;
		return *this;
	}

	float4& operator-=(const float4 _other)
	{
		this->x -= _other.x;
		this->y -= _other.y;
		this->z -= _other.z;
		this->w -= _other.w;
		return *this;
	}

	float4& operator*=(const float4 _other)
	{
		this->x *= _other.x;
		this->y *= _other.y;
		this->z *= _other.z;
		this->w *= _other.w;
		return *this;
	}

	float4& operator/=(const float4 _other)
	{
		this->x /= _other.x;
		this->y /= _other.y;
		this->z /= _other.z;
		this->w /= _other.w;
		return *this;
	}

	// 대입연산자
	float4& operator=(const float4& _other)
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;
		w = _other.w;

		return *this;
	}

public:
	int ix() const
	{
		return static_cast<int>(x);
	}

	int iy() const
	{
		return static_cast<int>(y);
	}

	int iz() const
	{
		return static_cast<int>(z);
	}


	float hx() const
	{
		return x * 0.5f;
	}

	float hy() const
	{
		return y * 0.5f;
	}

	float hz() const
	{
		return z * 0.5f;
	}

	float4 halffloat4() const
	{
		return { hx(), hy(), hz() };
	}


	int ihx() const
	{
		return static_cast<int>(hx());
	}

	int ihy() const
	{
		return static_cast<int>(hy());
	}

	int ihz() const
	{
		return static_cast<int>(hz());
	}



public:
	float4()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	float4(float _x, float _y)
		: x(_x), y(_y), z(0.0f), w(1.0f)
	{
	}

	float4(float _x, float _y, float _z, float _w = 1.0f)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	~float4() {

	}

public:		// delete constructer
	float4(const float4& _other)
		: x(_other.x), y(_other.y), z(_other.z), w(_other.w)
	{

	}

};

class int4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			__int64 HighValue;
			__int64 LowValue;
		};
	};
};

class Figure // 사각형의 형태  class
{
public:
	float4 pos_;  // 좌표 (이자 좌상단 끝)
	float4 size_; // 크기 (이자 우하단 끝)

public:
	int iLeft()
	{
		return pos_.ix() - size_.ihx();
	}

	int iRight()
	{
		return pos_.ix() + size_.ihx();
	}

	int iTop()
	{
		return pos_.iy() - size_.ihy();
	}

	int iBot()
	{
		return pos_.iy() + size_.ihy();
	}

	int Left()
	{
		return pos_.ix();
	}

	int Right()
	{
		return size_.ix();
	}
	
	int Top()
	{
		return pos_.iy();
	}

	int Bottom()
	{
		return size_.iy();
	}

public:
	Figure(float4 _Pos, float4 _Size)
		: pos_(_Pos), size_(_Size)
	{

	}
};