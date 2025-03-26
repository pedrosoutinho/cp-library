#include "../monoid/add.hpp"
#include "../monoid/min.hpp"

template <typename E>
struct ActedMonoid_Min_Add {
  using Monoid_X = Monoid_Min<E>;
  using Monoid_A = Monoid_Add<E>;
  using X = typename Monoid_X::value_type;
  using A = typename Monoid_A::value_type;
  static constexpr X act(const X &x, const A &a, const ll &size) {
    if (x == infty<E>) return x;
    return x + a;
  }
};
