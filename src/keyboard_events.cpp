#include "../include/keyboard_events.hpp"

namespace pr::gl {

  std::future<std::unique_ptr<unsigned char[]>>  keyboard_events::mandelbrot_future_data;

  void keyboard_events::on_key_pressed(GLFWwindow* window, int key, int, int action, int) {
    if (key != GLFW_KEY_P || action != GLFW_PRESS)
      return;

    if (!pr::zoom_level)
      pr::zoom_level = 1;
    else
      pr::zoom_level *= 4;

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    mandelbrot_future_data = std::async(
      std::launch::async, 
      pr::mandelbrot, 
      std::size_t{pr::zoom_level}, 
      pr::width {static_cast<std::size_t>(w)}, 
      pr::height{static_cast<std::size_t>(h)}
    );
  }

}
