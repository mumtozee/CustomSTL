#include <iostream>
#include <string>
#include <vector>

using IntType = int64_t;

void InjectAsterisks(std::string& input_str) {
  const IntType new_size = 2 * input_str.size() + 1;
  std::string updated_string(new_size, '*');
  for (IntType i = 0; i < input_str.size(); ++i) {
    updated_string[2 * i + 1] = input_str[i];
  }
  input_str = std::move(updated_string);
}

IntType NaivePalindromeCount(const std::string& input_str, IntType idx,
                             IntType start_range = 0) {
  IntType i = start_range;
  while (idx - i - 1 >= 0 && idx + i + 1 < input_str.size() &&
         input_str[idx - i - 1] == input_str[idx + i + 1]) {
    ++i;
  }
  return i;
}

void ComputePalindromeIndexes(const std::string& input_str,
                              std::vector<IntType>& pal_array) {
  pal_array[0] = 0;
  IntType left_bound = 0;
  IntType right_bound = 0;
  for (IntType i = 1; i < input_str.size(); ++i) {
    if (i > right_bound) {
      pal_array[i] = NaivePalindromeCount(input_str, i);
    } else {
      IntType j = (right_bound - i) + left_bound;  // reflection of i
      IntType temp_range = std::min(right_bound - i, pal_array[j]);
      pal_array[i] = NaivePalindromeCount(input_str, i, temp_range);
    }

    if (i + pal_array[i] > right_bound) {
      right_bound = i + pal_array[i];
      left_bound = i - pal_array[i];
    }
  }
}

IntType CountPalindromes(const std::string& input_str) {
  IntType answer = 0;
  std::vector<IntType> pal_array(input_str.size(), 0);
  ComputePalindromeIndexes(input_str, pal_array);
  for (IntType i = 0; i < input_str.size(); ++i) {
    answer += pal_array[i] / 2;
  }
  return answer;
}

const IntType max_text_size = 100'000;

int main() {
  std::string original_text;
  original_text.reserve(max_text_size);
  std::cin >> original_text;
  InjectAsterisks(original_text);
  std::cout << CountPalindromes(original_text);
  return 0;
}