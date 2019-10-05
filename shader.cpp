#include "shader.hpp"
#include "common.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

namespace pr::gl {

  // TODO: code dup
  shader::shader(std::string const& vertex_path, std::string const& fragment_path) {
    std::ifstream vertex_file(vertex_path);
    std::ifstream fragment_file(fragment_path);

    if (!vertex_file.is_open())
      terminate(std::cerr, "Invalid vertex source file path.");

    if (!fragment_file.is_open())
      terminate(std::cerr, "Invalid fragment source file path.");

    std::stringstream vertex_ss;
    std::stringstream fragment_ss;

    vertex_ss << vertex_file.rdbuf();
    fragment_ss << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    std::string vertex_code = vertex_ss.str();
    std::string fragment_code = fragment_ss.str();

    auto v_chars = vertex_code.c_str();
    auto f_chars = fragment_code.c_str();

    unsigned vertex = glCreateShader(GL_VERTEX_SHADER);
    unsigned fragment = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    std::array<char, 512> log;

    glShaderSource(vertex, 1, &v_chars, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, log.max_size(), nullptr, log.data());
      terminate(std::cerr, log.data());
    }

    glShaderSource(fragment, 1, &f_chars, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, log.max_size(), nullptr, log.data());
      terminate(std::cerr, log.data());
    }

    m_ID = glCreateProgram();

    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);

    glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(m_ID, log.max_size(), nullptr, log.data());
      terminate(std::cerr, log.data());
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void shader::activate() {
    glUseProgram(m_ID);
  }
  

} // end pr::gl