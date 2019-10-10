#include <iostream>
#include <future>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../include/gl_util.hpp"
#include "../include/common.hpp"
#include "../include/mandelbrot.hpp"
#include "../include/shader.hpp"
#include "../include/graphics_primitives.hpp"
#include "../include/mouse_events.hpp"
#include "../include/keyboard_events.hpp"

int main() {
  std::pair<pr::width, pr::height> dimensions{pr::width{1280}, pr::height{720}};

  pr::gl::keyboard_events::mandelbrot_future_data = 
    std::async(
      std::launch::async, 
      pr::mandelbrot, 
      std::size_t{pr::zoom_level}, dimensions.first, dimensions.second
    );

  // gl

  pr::gl::init();

  auto* const w = glfwCreateWindow(
    dimensions.first.get(), dimensions.second.get(), 
    "Mandelbrot orbits", nullptr, nullptr
  );

  if (!w)
    pr::terminate(std::cerr, "Error while creating a new window.");

  glfwMakeContextCurrent(w);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    pr::terminate(std::cerr, "Error while loading GLAD.");

  glViewport(0, 0, dimensions.first.get(), dimensions.second.get());
  glfwSetFramebufferSizeCallback(w, [](GLFWwindow* window, int w, int h) { 
    pr::gl::on_window_resize(
      window, 
      pr::width{static_cast<std::size_t>(w)}, 
      pr::height{static_cast<std::size_t>(h)}); 
  });

  // primitives

  pr::gl::rectangle rect{
    pr::gl::shader{"data/vertex_shader.glsl", "data/fragment_shader.glsl"},
    std::vector<float>{
       1.0f,  1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f 
    }
  };

  std::size_t const iterations = 50;

  pr::gl::connected_lines lines{
    pr::gl::shader{"data/vertex_line_shader.glsl", "data/fragment_line_shader.glsl"},
    std::vector<float>(6 * (iterations + 1), 0.0f)
  };

  // callbacks

  glfwSetMouseButtonCallback(w, pr::gl::mouse_events::on_mouse_click);
  glfwSetKeyCallback(w, pr::gl::keyboard_events::on_key_pressed);

  // textures

  pr::gl::set_primary_texture(
    pr::gl::keyboard_events::mandelbrot_future_data.get(), 
    dimensions
  );

  // main loop

  pr::gl::window_loop(
    pr::gl::input_action{pr::gl::process_input},
    pr::gl::render_action{[&](auto window) {
      rect.draw();

      if (!pr::gl::mouse_events::click_count)
        return;

      int w, h;
      glfwGetWindowSize(window, &w, &h);

      if (pr::gl::mouse_events::click_count % 2) {
        double xpos, ypos; // TODO: code dup
        glfwGetCursorPos(window, &xpos, &ypos);

        std::vector<float> new_lines = pr::iterate(
          iterations,
          pr::strong_x{static_cast<float>(xpos / w *  2 - 1)}, 
          pr::strong_y{static_cast<float>(ypos / h * -2 + 1)}
        );

        lines.update(new_lines);
      }

      lines.draw();

      if (pr::ready(pr::gl::keyboard_events::mandelbrot_future_data))
        pr::gl::set_primary_texture(
          pr::gl::keyboard_events::mandelbrot_future_data.get(), 
          {pr::width{static_cast<std::size_t>(w)}, pr::height{static_cast<std::size_t>(h)}}
      );
    }},
    w
  );
}