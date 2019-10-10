#pragma once

#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace pr::gl {

  class mouse_events {
    public:
      static std::size_t click_count;
      static void on_mouse_click(GLFWwindow* w, int button, int action, int);
  };

}