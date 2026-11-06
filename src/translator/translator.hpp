#pragma once
#include <string_view>
#include "../fwd.hpp"
#include "../signal.hpp"

struct InitPair {
  std::string_view name{};
  std::string_view value{};
};

class Translator {
public:
  void start(Tokens& tokens);

public: // Signals
  Signal<> translationStarted{};
  Signal<> translationFinished{};

  Signal<std::string, std::vector<InitPair>> initPairsRequested{};
};