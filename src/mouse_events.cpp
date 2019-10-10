#include "../include/mouse_events.hpp"

namespace pr::gl {

  std::size_t mouse_events::click_count = 0;

  void mouse_events::on_mouse_click(GLFWwindow* w, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
      ++click_count;
  }

}
