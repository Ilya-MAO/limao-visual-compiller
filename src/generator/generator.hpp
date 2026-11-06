#pragma once
#include <sstream>
#include <string>
#include "../fwd.hpp"

class Generator {
public:
  void start();
  void finish();

  void declarateVariables(std::string type, std::vector<InitPair> pairs);

  std::string output() { return m_output.str(); }

private:
  std::stringstream m_output{};
};