namespace hive
{
	class Vector4i
	{
	public:
		union {
			struct {
				int32 x;
				int32 y;
				int32 z;
				int32 w;
			};

			int32 coord[4] = { 0 };
		};
		constexpr Vector4i() : x(0), y(0), z(0), w(0) {}
		constexpr Vector4i(int32 x, int32 y, int32 z, int32 w) : x(x), y(y), z(z), w(w) {}

		int32& operator[](int idx) { return coord[idx]; }
		const int32& operator[](int idx) const { return coord[idx]; }

		constexpr Vector4i& operator+=(const Vector4i& other)
		{ 
			x += other.x; 
			y += other.y; 
			z += other.z;
			w += other.w;
			return *this; 
		}
		constexpr Vector4i operator+(const Vector4i& other) const
		{ 
			Vector4i tmp(*this);
			tmp += other; 
			return tmp; 
		}
		constexpr Vector4i& operator-=(const Vector4i& other)
		{ 
			x -= other.x; 
			y -= other.y; 
			z -= other.z;
			w -= other.w;
			return *this; 
		}
		constexpr Vector4i operator-(const Vector4i& other) const
		{ 
			Vector4i tmp(*this);
			tmp -= other; 
			return tmp; 
		}
		constexpr Vector4i& operator*=(int32 scalar)
		{ 
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}
		constexpr Vector4i operator*(int32 scalar) const
		{
			Vector4i tmp(*this);
			tmp *= scalar;
			return tmp;
		}
		constexpr Vector4i& operator/=(int32 scalar)
		{ 
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}
		constexpr Vector4i operator/(int32 scalar) const
		{ 
			Vector4i tmp(*this);
			tmp /= scalar; 
			return tmp; 
		}
		constexpr Vector4i& operator %=(int32 scalar)
		{
			x %= scalar;
			y %= scalar;
			z %= scalar;
			w %= scalar;
			return *this;
		}
		constexpr Vector4i operator%(int32 scalar) const
		{
			Vector4i tmp(*this);
			tmp %= scalar;
			return tmp;
		}

		constexpr Vector4i operator-() const { return Vector4i(-x, -y, -z, -w); }
		constexpr bool operator==(const Vector4i& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		constexpr bool operator!=(const Vector4i& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		float32 Length() const { return sqrt(LengthSq()); }
		int64 LengthSq() const {
			return static_cast<int64>(x) * x +
				static_cast<int64>(y) * y +
				static_cast<int64>(z) * z +
				static_cast<int64>(w) * w;
		}

		constexpr explicit operator std::string() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
		}
	};

	constexpr Vector4i operator*(int32 scalar, const Vector4i& vector) { return vector * scalar; }
}