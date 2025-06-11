namespace hive
{
	class Vector2i
	{
	public:
		union {
			struct {
				int32 x;
				int32 y;
			};

			int32 coord[2] = { 0 };
		};
		constexpr Vector2i() : x(0), y(0) {}
		constexpr Vector2i(int32 x, int32 y) : x(x), y(y) {}

		int32& operator[](int idx) { return coord[idx]; }
		const int32& operator[](int idx) const { return coord[idx]; }

		constexpr Vector2i& operator+=(const Vector2i& other) 
		{ 
			x += other.x; 
			y += other.y; 
			return *this; 
		}
		constexpr Vector2i operator+(const Vector2i& other) const 
		{ 
			Vector2i tmp(*this); 
			tmp += other; 
			return tmp; 
		}
		constexpr Vector2i& operator-=(const Vector2i& other) 
		{ 
			x -= other.x; 
			y -= other.y; 
			return *this; 
		}
		constexpr Vector2i operator-(const Vector2i& other) const 
		{ 
			Vector2i tmp(*this); 
			tmp -= other; 
			return tmp; 
		}
		constexpr Vector2i& operator*=(int32 scalar)
		{ 
			x *= scalar;
			y *= scalar;
			return *this;
		}
		constexpr Vector2i operator*(int32 scalar) const
		{
			Vector2i tmp(*this);
			tmp *= scalar;
			return tmp;
		}
		constexpr Vector2i& operator/=(int32 scalar)
		{ 
			x /= scalar;
			y /= scalar;
			return *this;
		}
		constexpr Vector2i operator/(int32 scalar) const
		{ 
			Vector2i tmp(*this); 
			tmp /= scalar; 
			return tmp; 
		}
		constexpr Vector2i& operator %=(int32 scalar)
		{
			x %= scalar;
			y %= scalar;
			return *this;
		}
		constexpr Vector2i operator%(int32 scalar) const
		{
			Vector2i tmp(*this);
			tmp %= scalar;
			return tmp;
		}

		constexpr Vector2i operator-() const { return Vector2i(-x, -y); }
		constexpr bool operator==(const Vector2i& other) const { return x == other.x && y == other.y; }
		constexpr bool operator!=(const Vector2i& other) const { return x != other.x || y != other.y; }

		float32 Length() const { return sqrt(LengthSq()); }
		int64 LengthSq() const { return static_cast<int64>(x) * x + static_cast<int64>(y) * y; }

		constexpr explicit operator std::string() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		}
	};

	constexpr Vector2i operator*(int32 scalar, const Vector2i& vector) { return vector * scalar; }
}