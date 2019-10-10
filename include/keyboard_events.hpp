#pragma once

#include <future>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mandelbrot.hpp"

namespace pr::gl {

  class keyboard_events {
    public:
      static std::future<std::unique_ptr<unsigned char[]>>  mandelbrot_future_data;
      static void on_key_pressed(GLFWwindow* w, int key, int, int action, int);
  };

}