namespace hive
{
	class Vector2
	{
	public:
		union {
			struct {
				float32 x;
				float32 y;
			};

			float32 coord[2] = { 0 };
		};
		constexpr Vector2() : x(0), y(0) {}
		constexpr Vector2(float32 x, float32 y) : x(x), y(y) {}

		float32& operator[](int idx) { return coord[idx]; }
		const float32& operator[](int idx) const { return coord[idx]; }

		constexpr Vector2& operator+=(const Vector2& other) 
		{ 
			x += other.x; 
			y += other.y; 
			return *this; 
		}
		constexpr Vector2 operator+(const Vector2& other) const 
		{ 
			Vector2 tmp(*this); 
			tmp += other; 
			return tmp; 
		}
		constexpr Vector2& operator-=(const Vector2& other) 
		{ 
			x -= other.x; 
			y -= other.y; 
			return *this; 
		}
		constexpr Vector2 operator-(const Vector2& other) const 
		{ 
			Vector2 tmp(*this); 
			tmp -= other; 
			return tmp; 
		}
		constexpr Vector2& operator*=(float32 scalar) 
		{ 
			x *= scalar;
			y *= scalar;
			return *this;
		}
		constexpr Vector2 operator*(float32 scalar) const 
		{
			Vector2 tmp(*this);
			tmp *= scalar;
			return tmp;
		}
		constexpr Vector2& operator/=(float32 scalar) 
		{ 
			x /= scalar;
			y /= scalar;
			return *this;
		}
		constexpr Vector2 operator/(float32 scalar) const 
		{ 
			Vector2 tmp(*this); 
			tmp /= scalar; 
			return tmp; 
		}
		constexpr Vector2 operator-() const { return Vector2(-x, -y); }
		constexpr bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		constexpr bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }

		float32 Length() const { return sqrt(x * x + y * y); }
		float32 LengthSq() const { return x * x + y * y; }
		Vector2 Normalized() const
		{
			float32 length = Length();
			if (length > 0.0f)
			{
				return Vector2(x / length, y / length);
			}
			return Vector2(0.0f, 0.0f);
		}
		void Normalize() { *this = Normalized(); }
		float32 Dot(const Vector2& other) const { return x * other.x + y * other.y; }
		float32 Cross(const Vector2& other) const { return x * other.y - y * other.x; }
		float32 AngleBetween(const Vector2& other) const
		{
			float32 lengths = sqrt(LengthSq() * other.LengthSq());
			if (lengths > 0.0f)
			{
				float32 dot = Dot(other);
				return acos(dot / lengths);
			}
			return 0.0f;
		}

		constexpr explicit operator std::string() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		}
	};

	constexpr Vector2 operator*(float32 scalar, const Vector2& vector) { return vector * scalar; }
}