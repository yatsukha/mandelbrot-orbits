#pragma once

#include "shader.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>

namespace pr::gl {

  // enable collection of crtp constructs
  class primitive_interface {
    public:
      virtual void draw() = 0;
  };


  template<typename Derived>
  class primitive : public primitive_interface {
    protected:
      unsigned VAO = -1;

    private:
      shader shader_program;

    public:
      primitive(shader&& shader_program)
        : shader_program(std::forward<shader>(shader_program)) {}

      void draw() override {
        shader_program.activate();
        glBindVertexArray(VAO);

        // if you have an error here its an intellisense error
        static_cast<Derived*>(this)->draw_impl();
      }
  };

  class rectangle : public primitive<rectangle> {
    public:
      rectangle(shader&& s, std::vector<float> const& vertices);

      void draw_impl() {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }
  };

  class connected_lines : public primitive<connected_lines> {
    private:
      unsigned VBO;
      unsigned points;

    public:
      connected_lines(shader&& s, std::vector<float> const& lines);

      void draw_impl() {
        glDrawArrays(GL_LINES, 0, points);
      }

      void update(std::vector<float> const& new_lines);
  };


}