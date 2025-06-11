namespace hive
{
	class Vector3i
	{
	public:
		union {
			struct {
				int32 x;
				int32 y;
				int32 z;
			};

			int32 coord[3] = { 0 };
		};
		constexpr Vector3i() : x(0), y(0), z(0) {}
		constexpr Vector3i(int32 x, int32 y, int32 z) : x(x), y(y), z(z) {}

		int32& operator[](int idx) { return coord[idx]; }
		const int32& operator[](int idx) const { return coord[idx]; }

		constexpr Vector3i& operator+=(const Vector3i& other)
		{ 
			x += other.x; 
			y += other.y; 
			z += other.z;
			return *this; 
		}
		constexpr Vector3i operator+(const Vector3i& other) const
		{ 
			Vector3i tmp(*this);
			tmp += other; 
			return tmp; 
		}
		constexpr Vector3i& operator-=(const Vector3i& other)
		{ 
			x -= other.x; 
			y -= other.y; 
			z -= other.z;
			return *this; 
		}
		constexpr Vector3i operator-(const Vector3i& other) const
		{ 
			Vector3i tmp(*this);
			tmp -= other; 
			return tmp; 
		}
		constexpr Vector3i& operator*=(int32 scalar)
		{ 
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		constexpr Vector3i operator*(int32 scalar) const
		{
			Vector3i tmp(*this);
			tmp *= scalar;
			return tmp;
		}
		constexpr Vector3i& operator/=(int32 scalar)
		{ 
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		constexpr Vector3i operator/(int32 scalar) const
		{ 
			Vector3i tmp(*this);
			tmp /= scalar; 
			return tmp; 
		}
		constexpr Vector3i& operator %=(int32 scalar)
		{
			x %= scalar;
			y %= scalar;
			z %= scalar;
			return *this;
		}
		constexpr Vector3i operator%(int32 scalar) const
		{
			Vector3i tmp(*this);
			tmp %= scalar;
			return tmp;
		}

		constexpr Vector3i operator-() const { return Vector3i(-x, -y, -z); }
		constexpr bool operator==(const Vector3i& other) const { return x == other.x && y == other.y && z == other.z; }
		constexpr bool operator!=(const Vector3i& other) const { return x != other.x || y != other.y || z != other.z; }

		float32 Length() const { return sqrt(LengthSq()); }
		int64 LengthSq() const { return static_cast<int64>(x) * x + static_cast<int64>(y) * y + static_cast<int64>(z) * z; }
	};

	constexpr Vector3i operator*(int32 scalar, const Vector3i& vector) { return vector * scalar; }
}