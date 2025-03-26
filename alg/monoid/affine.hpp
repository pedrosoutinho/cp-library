#pragma once

// op(F, G) = comp(G,F), F のあとで G
template <typename K>
struct Monoid_Affine {
  using F = pair<K, K>;
  using value_type = F;
  using X = value_type;
  static constexpr F op(const F &x, const F &y) noexcept {
    return F({x.first * y.first, x.second * y.first + y.second});
  }
  static constexpr F inverse(const F &x) {
    auto [a, b] = x;
    a = K(1) / a;
    return {a, a * (-b)};
  }
  static constexpr K eval(const F &f, K x) noexcept {
    return f.first * x + f.second;
  }
  static constexpr F unit() { return {K(1), K(0)}; }
  static constexpr bool commute = false;
};
