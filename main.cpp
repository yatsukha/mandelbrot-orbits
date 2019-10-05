#include <iostream>
#include <future>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "gl_util.hpp"
#include "common.hpp"
#include "mandelbrot.hpp"
#include "shader.hpp"

namespace pr {
  std::size_t click_count = 0;
  std::size_t const iterations = 50;

  float mouse_line[(iterations + 1) * 6] = {0};
  unsigned line_VBO;

  std::size_t zoom_level = 0;
  std::future<std::unique_ptr<unsigned char[]>> mandelbrot_future_data;
}

int main() {
  std::pair<pr::width, pr::height> dimensions{pr::width{1280}, pr::height{720}};

  pr::mandelbrot_future_data = std::async(
    std::launch::async, pr::mandelbrot, std::size_t{pr::zoom_level}, dimensions.first, dimensions.second
  );

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

  float vertices[] = {
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f 
  };

  unsigned int indices[] = {  
    0, 1, 3,
    1, 2, 3 
  };

  unsigned VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
    
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned l_VBO, l_VAO;
  glGenVertexArrays(1, &l_VAO);
  glGenBuffers(1, &l_VBO);

  glBindVertexArray(l_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, l_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pr::mouse_line), pr::mouse_line, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  pr::line_VBO = l_VBO;

  pr::gl::shader shader{"vertex_shader.glsl", "fragment_shader.glsl"};
  pr::gl::shader line_shader{"vertex_line_shader.glsl", "fragment_line_shader.glsl"};

  pr::gl::set_primary_texture(pr::mandelbrot_future_data.get(), dimensions);

  glfwSetMouseButtonCallback(w, [](auto window, auto button, auto action, auto) {
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
      return;

    ++pr::click_count;
  });

  glfwSetKeyCallback(w, [](auto window, auto key, auto sc, auto action, auto) -> void {
    if (key != GLFW_KEY_P || action != GLFW_PRESS)
      return;

    if (!pr::zoom_level)
      pr::zoom_level = 1;
    else
      pr::zoom_level *= 4;

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    auto ww = pr::width {static_cast<std::size_t>(w)};
    auto hh = pr::height{static_cast<std::size_t>(h)};

    pr::mandelbrot_future_data = std::async(
      std::launch::async, pr::mandelbrot, std::size_t{pr::zoom_level}, ww, hh
    );

  });

  pr::gl::window_loop(
    pr::gl::input_action{pr::gl::process_input},
    pr::gl::render_action{[&](auto window) {
      shader.activate();

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      if (!pr::click_count)
        return;

      int w, h;
      glfwGetWindowSize(window, &w, &h);

      if (pr::click_count % 2) {
        double xpos, ypos; // TODO: code dup
        glfwGetCursorPos(window, &xpos, &ypos);
        
        pr::iterate(
          pr::mouse_line,
          pr::iterations, 
          pr::strong_x{static_cast<float>(xpos / w *  2 - 1)}, 
          pr::strong_y{static_cast<float>(ypos / h * -2 + 1)}
        );

        glBindBuffer(GL_ARRAY_BUFFER, pr::line_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pr::mouse_line), &pr::mouse_line);
      }

      line_shader.activate();

      glBindVertexArray(l_VAO);
      glDrawArrays(GL_LINES, 0, 2 * (pr::iterations + 1));

      if (pr::ready(pr::mandelbrot_future_data))
        pr::gl::set_primary_texture(pr::mandelbrot_future_data.get(), {pr::width{static_cast<std::size_t>(w)}, pr::height{static_cast<std::size_t>(h)}});
    }},
    w
  );
}