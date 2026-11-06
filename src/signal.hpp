#pragma once
#include <functional>
#include <vector>

template <typename... Args>
class Signal {
  using Callback = std::function<void (Args...)>;

public:

  void connect(Callback callback) {
    m_slots.push_back(std::move(callback));
  }

  void emit(Args... args) {
    for (const auto& slot : m_slots) {
      slot(args...);
    }
  }

private:
  std::vector<Callback> m_slots{};
};