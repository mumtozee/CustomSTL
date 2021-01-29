#include <iostream>
#include <string>

std::string CreateInitialCPS(int n) {
  std::string answer(2 * n, '(');
  std::fill(answer.begin() + n, answer.end(), ')');
  return answer;
}

std::string GetNextCPS(const std::string& input) {
  std::string answer{input};
  int counter_close = 0;
  int counter_open = 0;
  int limit = static_cast<int>(input.size()) - 1;
  int i = limit;
  for (; i >= 0; --i) {
    if (answer[i] == '(') {
      ++counter_open;
      if (counter_close > counter_open) {
        answer[i] = ')';
        for (int j = 1; j <= counter_open; ++j) {
          answer[i + j] = '(';
        }
        for (int j = 1; j <= counter_close; ++j) {
          answer[i + counter_open + j] = ')';
        }
        break;
      }
    } else {
      ++counter_close;
    }
  }
  if (counter_open + counter_close == input.size()) {
    return {};
  }
  return answer;
}

int main() {
  int n = 0;
  std::cin >> n;
  auto cps = CreateInitialCPS(n);
  while (!cps.empty()) {
    std::cout << cps << '\n';
    cps = GetNextCPS(cps);
  }
  return 0;
}