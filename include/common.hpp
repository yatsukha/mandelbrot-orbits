#pragma once

#include "strong_type.hpp"

#include <ostream>
#include <future>
#include <chrono>

namespace pr {

  void terminate(std::ostream& out, const char* msg);

  using lower_bound = pr::strong_type<float, struct lower_bound_tag>;
  using upper_bound = pr::strong_type<float, struct upper_bound_tag>;

  using start_bounds = pr::strong_type<std::pair<lower_bound, upper_bound>, struct start_bounds_tag>;
  using end_bounds   = pr::strong_type<std::pair<lower_bound, upper_bound>, struct end_bounds_tag>;

  inline float scale(float const value, start_bounds const& sb, end_bounds const& eb) noexcept {
    return 
      (value - sb.get().first.get()) / (sb.get().second.get() - sb.get().first.get())
        * (eb.get().second.get() - eb.get().first.get())
        + eb.get().first.get();
  }

  template<typename T>
  bool ready(std::future<T>& f) {
    return f.valid() && std::future_status::ready == f.wait_for(std::chrono::nanoseconds{0});
  }

}