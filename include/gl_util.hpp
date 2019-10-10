#pragma once

#include "strong_type.hpp"
#include "shared_types.hpp"

#include <cstddef>
#include <functional>
#include <vector>

struct GLFWwindow;

namespace pr::gl {

  void init();

  void on_window_resize(GLFWwindow* const window, width const w, height const h);

  void process_input(GLFWwindow* const window);

  using signature = std::function<void(GLFWwindow*)>;

  using input_action  = pr::strong_type<signature, struct input_action_type>;
  using render_action = pr::strong_type<signature, struct render_action_type>;

  void window_loop(
    input_action  const& input, 
    render_action const& render, 
    GLFWwindow*   const  window
  );

  void set_primary_texture(std::unique_ptr<unsigned char[]> data, std::pair<width, height> const& dim);

}