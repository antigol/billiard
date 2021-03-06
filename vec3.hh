#ifndef VEC3_HH
#define VEC3_HH

#include <ostream>

class Vec3
{
public:
	Vec3();
	Vec3(double x, double y, double z);

	double &operator[](int i);
	const double &operator[](int i) const;

	Vec3 &operator+=(const Vec3& a);
	Vec3 &operator-=(const Vec3& a);
	Vec3 &operator*=(double a);
	Vec3 &operator/=(double a);

    Vec3 operator+();
    Vec3 operator-();

	void setNull();
	bool isNull() const;

	static double dot(const Vec3& a, const Vec3& b);
    static Vec3 cross(const Vec3& a, const Vec3& b);
    static double norm(const Vec3& a);
    static double length(const Vec3& a);
	static Vec3 random(double L = 1.0);
private:
	double _d[3];
};

inline Vec3 operator+(Vec3 a, const Vec3& b) { return a += b; }
inline Vec3 operator-(Vec3 a, const Vec3& b) { return a -= b; }
inline Vec3 operator*(Vec3 a, double b) { return a *= b; }
inline Vec3 operator*(double a, Vec3 b) { return b *= a; }
inline Vec3 operator/(Vec3 a, double b) { return a /= b; }

std::ostream& operator<<(std::ostream& out, const Vec3& a);

#endif // VEC3_HH
