#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void ComputeZIndices(const std::string& input_string,
                     std::vector<int>& z_array) {
  z_array[0] = input_string.size();
  int left_bound = 0;
  int right_bound = 0;
  for (int i = 1; i < input_string.size(); ++i) {
    auto previous_index = std::min(z_array[i - left_bound], right_bound - i);
    z_array[i] = std::max(0, previous_index);
    while (input_string[z_array[i]] == input_string[i + z_array[i]]) {
      ++z_array[i];
    }
    if (i + z_array[i] > right_bound) {
      left_bound = i;
      right_bound = i + z_array[i];
    }
  }
}

std::vector<int> FoundPatternPositions(const std::string& text,
                                       const std::string& pattern) {
  std::vector<int> answer;
  std::string buffer_string = pattern + '#' + text;
  std::vector<int> z_array(buffer_string.size(), 0);
  ComputeZIndices(buffer_string, z_array);

  for (int i = pattern.size() + 1; i < z_array.size(); ++i) {
    if (z_array[i] == pattern.size()) {
      answer.emplace_back(i - (pattern.size() + 1));
    }
  }
  return answer;
}

const int max_pattern_size = 30'000;
const int max_text_size = 300'000;

int main() {
  std::string pattern;
  pattern.reserve(max_pattern_size);

  std::string text;
  text.reserve(max_text_size);

  std::cin >> pattern >> text;
  std::vector<int> positions = FoundPatternPositions(text, pattern);

  for (const auto& item : positions) {
    std::cout << item << ' ';
  }

  return 0;
}