#include "../include/gl_util.hpp"
#include "../include/shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace pr::gl {

  void init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4); // 4xMSAA
  }

  void on_window_resize(GLFWwindow* const window, width const w, height const h) {
    glViewport(0, 0, w.get(), h.get());
  }

  void process_input(GLFWwindow* const window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  void window_loop(
    input_action  const& input, 
    render_action const& render, 
    GLFWwindow*   const  window
  ) {
    auto _input  = input.get()  ? input.get()  : [](auto _){};
    auto _render = render.get() ? render.get() : [](auto _){};

    glfwSwapInterval(1); // vsync

    while (!glfwWindowShouldClose(window)) {
      _input(window);
      _render(window);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  void set_primary_texture(std::unique_ptr<unsigned char[]> data, std::pair<width, height> const& dim) {
    unsigned texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, 
      dim.first.get(), dim.second.get(), 
      0, GL_RGB, GL_UNSIGNED_BYTE, 
      data.get()
    );

    glGenerateMipmap(GL_TEXTURE_2D);
  }

}