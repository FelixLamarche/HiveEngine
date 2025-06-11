namespace hive
{
	class Vector4
	{
	public:
		union {
			struct {
				float32 x, y, z, w;
			};
			float32 coord[4] = { 0 };
		};
		constexpr Vector4() : x(0), y(0), z(0), w(0) {}
		constexpr Vector4(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

		float32& operator[](int idx) { return coord[idx]; }
		const float32& operator[](int idx) const { return coord[idx]; }

		constexpr Vector4& operator+=(const Vector4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		constexpr Vector4 operator+(const Vector4& other) const
		{
			Vector4 tmp(*this);
			tmp += other;
			return tmp;
		}
		constexpr Vector4& operator-=(const Vector4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		constexpr Vector4 operator-(const Vector4& other) const
		{
			Vector4 tmp(*this);
			tmp -= other;
			return tmp;
		}
		constexpr Vector4& operator*=(float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}
		constexpr Vector4 operator*(float32 scalar) const
		{
			Vector4 tmp(*this);
			tmp *= scalar;
			return tmp;
		}
		constexpr Vector4& operator/=(float32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}
		constexpr Vector4 operator/(float32 scalar) const
		{
			Vector4 tmp(*this);
			tmp /= scalar;
			return tmp;
		}
		constexpr Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
		constexpr bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		constexpr bool operator!=(const Vector4& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		float32 Length() const { return sqrt(x * x + y * y + z * z + w * w); }
		float32 LengthSq() const { return x * x + y * y + z * z + w * w; }
		Vector4 Normalized() const
		{
			float32 length = Length();
			if (length > 0.0f)
			{
				return Vector4(x / length, y / length, z / length, w / length);
			}
			return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		void Normalize() { *this = Normalized(); }
		float32 Dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
	};

	constexpr Vector4 operator*(float32 scalar, const Vector4& vector) { return vector * scalar; }
}