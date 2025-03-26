#pragma once

constexpr db EPS = 1e-10;
constexpr db Pi = 3.141592653589793238462643383279L;
bool equals(db a, db b) { return fabs(b - a) < EPS; }
int sign(db a) { return equals(a, 0) ? 0 : a > 0 ? 1 : -1; }

template <typename R> struct Point {
    using P = Point;
    R x, y;
    Point() : x(0), y(0) {}
    Point(R _x, R _y) : x(_x), y(_y) {}
    template <typename T, typename U>
    Point(const pair<T, U>& p) : x(p.first), y(p.second) {}

    P operator+(const P& r) const { return {x + r.x, y + r.y}; }
    P operator-(const P& r) const { return {x - r.x, y - r.y}; }
    P operator*(R r) const { return {x * r, y * r}; }
    P operator/(R r) const { return {x / r, y / r}; }

    P& operator+=(const P& r) { return (*this) = (*this) + r; }
    P& operator-=(const P& r) { return (*this) = (*this) - r; }
    P& operator*=(R r) { return (*this) = (*this) * r; }
    P& operator/=(R r) { return (*this) = (*this) / r; }

    bool operator<(const P& r) const { return x != r.x ? x < r.x : y < r.y; }
    bool operator==(const P& r) const { return x == r.x and y == r.y; }
    bool operator!=(const P& r) const { return !((*this) == r); }

    P rotate(R rad) const {
        return {x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)};
    }

    R real() const { return x; }
    R imag() const { return y; }
    friend R real(const P& p) { return p.x; }
    friend R imag(const P& p) { return p.y; }
    friend R dot(const P& l, const P& r) { return l.x * r.x + l.y * r.y; }
    R dot(P other) { return x * other.x + y * other.y; }
    friend R cross(const P& l, const P& r) { return l.x * r.y - l.y * r.x; }
    friend R abs(const P& p) { return sqrt(p.x * p.x + p.y * p.y); }
    friend R norm(const P& p) { return p.x * p.x + p.y * p.y; }
    friend R arg(const P& p) { return atan2(p.y, p.x); }

    friend istream& operator>>(istream& is, P& p) {
        R a, b;
        is >> a >> b;
        p = P{a, b};
        return is;
    }
    friend ostream& operator<<(ostream& os, const P& p) {
        return os << p.x << " " << p.y;
    }
};

template <typename T> bool equals(const Point<T>& a, const Point<T>& b) {
    return equals(a.x, b.x) and equals(a.y, b.y);
}

// ccw, 点の進行方向
template <typename T>
int ccw(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
    Point x = b - a, y = c - a;
    if (cross(x, y) > EPS) return +1;                 // 反時計回り
    if (cross(x, y) < -EPS) return -1;                // 時計回り
    if (min(norm(x), norm(y)) < EPS * EPS) return 0;  // c=a または c=b
    if (dot(x, y) < EPS) return +2;    // c-a-b の順で一直線
    if (norm(x) < norm(y)) return -2;  // a-b-c の順で一直線
    return 0;                          // a-c-b の順で一直線
}

// ax+by+c
template <typename T> struct Line {
    T a, b, c;

    Line(T a, T b, T c) : a(a), b(b), c(c) {}
    Line(Point<T> A, Point<T> B) {
        a = A.y - B.y, b = B.x - A.x, c = A.x * B.y - A.y * B.x;
    }
    Line(T x1, T y1, T x2, T y2) : Line(Point<T>(x1, y1), Point<T>(x2, y2)) {}

    template <typename U> U eval(Point<U> P) { return a * P.x + b * P.y + c; }

    template <typename U> T eval(U x, U y) { return a * x + b * y + c; }

    // 同じ直線が同じ a,b,c で表現されるようにする
    void normalize() {
        static_assert(is_same_v<T, int> || is_same_v<T, long long>);
        T g = gcd(gcd(abs(a), abs(b)), abs(c));
        a /= g, b /= g, c /= g;
        if (b < 0) {
            a = -a, b = -b, c = -c;
        }
        if (b == 0 && a < 0) {
            a = -a, b = -b, c = -c;
        }
    }

    bool is_parallel(Line other) { return a * other.b - b * other.a == 0; }
    bool is_orthogonal(Line other) { return a * other.a + b * other.b == 0; }
};

template <typename T> struct Segment {
    Point<T> A, B;

    Segment(Point<T> A, Point<T> B) : A(A), B(B) {}
    Segment(T x1, T y1, T x2, T y2)
        : Segment(Point<T>(x1, y1), Point<T>(x2, y2)) {}

    bool contain(Point<T> C) {
        T det = (C - A).det(B - A);
        if (det != 0) return 0;
        return (C - A).dot(B - A) >= 0 && (C - B).dot(A - B) >= 0;
    }

    Line<T> to_Line() { return Line(A, B); }
};

template <typename REAL> struct Circle {
    Point<REAL> O;
    REAL r;
    Circle(Point<REAL> O, REAL r) : O(O), r(r) {}
    Circle(REAL x, REAL y, REAL r) : O(x, y), r(r) {}
    template <typename T> bool contain(Point<T> p) {
        REAL dx = p.x - O.x, dy = p.y - O.y;
        return dx * dx + dy * dy <= r * r;
    }
};
