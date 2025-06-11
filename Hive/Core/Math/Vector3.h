namespace std
{
	class string;
}

namespace hive
{
	class Vector3
	{
	public:
		union {
			struct {
				float32 x, y, z;
			};
			float32 coord[3] = { 0 };
		};
		constexpr Vector3() : x(0), y(0), z(0) {}
		constexpr Vector3(float32 x, float32 y, float32 z) : x(x), y(y), z(z) {}

		float32& operator[](int idx) { return coord[idx]; }
		const float32& operator[](int idx) const { return coord[idx]; }

		constexpr Vector3& operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		constexpr Vector3 operator+(const Vector3& other) const
		{
			Vector3 tmp(*this);
			tmp += other;
			return tmp;
		}
		constexpr Vector3& operator-=(const Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		constexpr Vector3 operator-(const Vector3& other) const
		{
			Vector3 tmp(*this);
			tmp -= other;
			return tmp;
		}
		constexpr Vector3& operator*=(float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		constexpr Vector3 operator*(float32 scalar) const
		{
			Vector3 tmp(*this);
			tmp *= scalar;
			return tmp;
		}
		constexpr Vector3& operator/=(float32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		constexpr Vector3 operator/(float32 scalar) const
		{
			Vector3 tmp(*this);
			tmp /= scalar;
			return tmp;
		}
		constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }
		constexpr bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
		constexpr bool operator!=(const Vector3& other) const { return x != other.x || y != other.y || z != other.z; }

		float32 Length() const { return sqrt(x * x + y * y + z * z); }
		float32 LengthSq() const { return x * x + y * y + z * z; }
		Vector3 Normalized() const
		{
			float32 length = Length();
			if (length > 0.0f)
			{
				return Vector3(x / length, y / length, z / length);
			}
			return Vector3(0.0f, 0.0f, 0.0f);
		}
		void Normalize() { *this = Normalized(); }
		float32 Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
		Vector3 Cross(const Vector3& other) const 
		{ 
			return Vector3(
				(y * other.z) - (z * other.y),
				(z * other.x) - (x * other.z),
				(x * other.y) - (y * other.x)
			);
		}
		float32 AngleBetween(const Vector3& other) const
		{
			float32 lengths = sqrt(LengthSq() * other.LengthSq());
			if (lengths > 0.0f)
			{
				float32 dot = Dot(other);
				return acos(dot / lengths);
			}
			return 0.0f;
		}
	};

	constexpr Vector3 operator*(float32 scalar, const Vector3& vector) { return vector * scalar; }
}