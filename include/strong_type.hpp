#pragma once

#include <utility>
#include <type_traits>

namespace pr {
  
  template<typename T, typename Tag>
  class strong_type {

      T value;

    public:

      explicit strong_type(T const& value) noexcept(std::is_nothrow_copy_constructible_v<T>) 
        : value(value) {}

      explicit strong_type(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
        : value(std::forward<T>(value)) {}

      T      & get()       noexcept { return value; }
      T const& get() const noexcept { return value; }
  };

}