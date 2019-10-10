#pragma once

#include <string>

namespace pr::gl {

  class shader {
    private:
      unsigned m_ID = 0;

    public:
      unsigned const& ID = m_ID;

      shader(std::string const& vertex_path, std::string const& fragment_path);
      virtual ~shader() = default;

      shader(shader const&) = delete;
      shader& operator=(shader const&) = delete;

      shader(shader&&) = default;
      shader& operator=(shader&&) = default;

      void activate();
  };

}