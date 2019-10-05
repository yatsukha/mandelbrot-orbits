#pragma once

#include <memory>
#include "shared_types.hpp"

namespace pr {
  
  std::unique_ptr<unsigned char[]> mandelbrot(std::size_t iterations, width const w, height const h);

  using strong_x = pr::strong_type<float, struct strong_x_tag>;
  using strong_y = pr::strong_type<float, struct strong_y_tag>;

  void iterate(float* data, std::size_t iterations, strong_x x, strong_y y);

}