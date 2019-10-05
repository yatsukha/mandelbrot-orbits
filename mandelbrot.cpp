#include "mandelbrot.hpp"
#include "common.hpp"

// TODO: delete
#include <iostream>

namespace pr {
  
  std::unique_ptr<unsigned char[]> mandelbrot(std::size_t const iterations, width const w, height const h) {
    std::unique_ptr<unsigned char[]> ret(new unsigned char[w.get() * h.get() * 3]);

    auto const h_ = h.get(),
               w_ = w.get();

    start_bounds x_bounds({lower_bound{0.0f}, upper_bound{static_cast<float>(w_)}}),
                 y_bounds({lower_bound{0.0f}, upper_bound{static_cast<float>(h_)}});

    int a = 0, aa = 0;

    for (std::size_t i = 0; i < h_; ++i)
      for (std::size_t j = 0; j < w_; ++j) {

        std::pair<float, float> c{0.f, 0.f}, p{
          scale(
            static_cast<float>(j), 
            x_bounds, 
            end_bounds({lower_bound{-2.5}, upper_bound{1.0}})
          ),
          scale(
            static_cast<float>(i), 
            y_bounds, 
            end_bounds({lower_bound{-1.0}, upper_bound{1.0}})
          )
        };

        std::size_t iteration = 0;
        
        while (c.first * c.first + c.second * c.second < 4.0f && iteration++ < iterations) {
          auto _ = c.first * c.first - c.second * c.second + p.first;

          c.second = 2 * c.first * c.second + p.second;
          c.first  = _;
        }

        unsigned char _ = 0xFF - 0x33 * (iteration > iterations);

        ret[j * 3     + i * w_ * 3] = _;
        ret[j * 3 + 1 + i * w_ * 3] = _;
        ret[j * 3 + 2 + i * w_ * 3] = _;
      }

    return ret;
  }

  void iterate(float* data, std::size_t iterations, strong_x x, strong_y y) {
    std::pair<float, float> c{
      scale(
        x.get(), 
        start_bounds({lower_bound{-1.0}, upper_bound{1.0}}), 
        end_bounds({lower_bound{-2.5}, upper_bound{1.0}})
      ),
      y.get()
    };

    std::pair<float, float> xy{0.0f, 0.0f};

    for (std::size_t i = 1; i <= 2 * iterations; i += 2) {
      auto _ = xy.first * xy.first - xy.second * xy.second + c.first;

      xy.second = 2 * xy.first * xy.second + c.second;
      xy.first  = _;

      data[i * 3] = data[(i + 1) * 3] =
        scale(
          xy.first, 
          start_bounds({lower_bound{-2.5}, upper_bound{1.0}}), 
          end_bounds({lower_bound{-1.0}, upper_bound{1.0}})
        );
      data[i * 3 + 1] = data[(i + 1) * 3 + 1] = xy.second;
    }

    data[6 * iterations + 3] = 
      scale(
          xy.first, 
          start_bounds({lower_bound{-2.5}, upper_bound{1.0}}), 
          end_bounds({lower_bound{-1.0}, upper_bound{1.0}})
      );
    data[6 * iterations + 4] = xy.second;
  }

}