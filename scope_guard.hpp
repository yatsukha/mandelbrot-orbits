#pragma once

namespace pr {

  // scope boilerplate

  template<typename Action>
  class scope_guard {
    Action a = nullptr;

  public:
    scope_guard(Action const a)
    try
      : a(a) {}
    catch (std::exception const&) {
      a();
      throw;
    }

    void release() { a = nullptr; }

    ~scope_guard() {
      a();
    }
  };

  namespace detail {
    template<typename T>
    void set(T& current, T next) {
      current = next;
    }
  }

  // helper

  template<typename T>
  auto undo_guard(T& t) {
    return scope_guard(std::bind(detail::set<T>, std::ref(t), t));
  }

}