#pragma once

#include "shader.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>

namespace pr::gl {

  template<typename Derived>
  class primitive;

  class rectangle : public primitive<rectangle> {
    public:
      rectangle(shader&& s, std::vector<float> const& vertices);

      void draw_implementation() {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }
  };


  template<typename Derived>
  class primitive {
    protected:
      unsigned VAO = -1;
      shader   shader_program;

    public:
      primitive(shader&& shader_program)
        : shader_program(std::forward<shader>(shader_program)) {}

      void draw() {
        shader_program.activate();
        glBindVertexArray(VAO);

        static_cast<Derived*>(this)->draw_implementation();
      }
  };


}