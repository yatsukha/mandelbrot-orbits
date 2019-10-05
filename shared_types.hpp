#pragma once

#include "strong_type.hpp"
#include <utility>

namespace pr {

  using width  = pr::strong_type<std::size_t, struct width_tag>;
  using height = pr::strong_type<std::size_t, struct height_tag>;

}