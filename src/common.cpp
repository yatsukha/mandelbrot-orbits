#include "../include/common.hpp"

namespace pr {

  void terminate(std::ostream& out, char const* msg) {
    out << msg << std::endl;
    std::exit(EXIT_FAILURE);
  }

}