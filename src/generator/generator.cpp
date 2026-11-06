#include "generator.hpp"
#include "../translator/translator.hpp"

void Generator::start() {
  m_output.clear();
  m_output << "int main(int argc, char* argv[]) {\n";
}

void Generator::finish() {
  m_output << "\nreturn 0;\n}";
}

void Generator::declarateVariables(std::string type, std::vector<InitPair> pairs) {
  m_output << type;
  for (const InitPair& pair : pairs) {
    m_output << " " << pair.name << " = " << pair.value;
  }
  m_output << ";\n";
}