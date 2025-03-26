#pragma once

template <typename Monoid> struct Cumsum_2D {
    using MX = Monoid;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    int H, W;
    V<X> dat;

    Cumsum_2D() {}
    Cumsum_2D(V<V<X>>& A) { build(A); }

    void build(V<V<X>>& A) {
        H = sz(A);
        W = (H == 0 ? 0 : sz(A[0]));
        dat.assign(H * W, MX::unit());
        rep(x, H) rep(y, W) {
            int k = W * x + y;
            dat[k] = (y == 0 ? A[x][y] : MX::op(dat[k - 1], A[x][y]));
        }
        rep(i, W, H * W) dat[i] = MX::op(dat[i - W], dat[i]);
    }

    // [x1,x2) x [y1,y2)

    template <bool allow_out_of_range = false>
    X sum(int x1, int x2, int y1, int y2) {
        if constexpr (allow_out_of_range) {
            ckmax(x1, 0ll), ckmin(x2, H), ckmax(y1, 0ll), ckmin(y2, W);
            if (x1 >= x2 || y1 >= y2) return MX::unit();
        }
        if (x2 == 0 || y2 == 0) return MX::unit();
        assert(0 <= x1 && x1 <= x2 && x2 <= H);
        assert(0 <= y1 && y1 <= y2 && y2 <= W);
        --x1, --y1, --x2, --y2;
        X a = (x1 >= 0 && y1 >= 0 ? dat[W * x1 + y1] : MX::unit());
        X b = (x1 >= 0 && y2 >= 0 ? dat[W * x1 + y2] : MX::unit());
        X c = (x2 >= 0 && y1 >= 0 ? dat[W * x2 + y1] : MX::unit());
        X d = (x2 >= 0 && y2 >= 0 ? dat[W * x2 + y2] : MX::unit());
        return MX::op(MX::op(a, d), MX::inverse(MX::op(b, c)));
    }

    X prefix_sum(int x, int y) {
        return (x == 0 || y == 0) ? MX::unit() : dat[W * x + y - (W + 1)];
    }
};
