#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using IntType = int;

struct Suffix {
  IntType suf_index_;
  std::array<IntType, 2> rank_;
};

auto is_less = [](const Suffix& lhs, const Suffix& rhs) {
  return lhs.rank_[0] < rhs.rank_[0] ||
         (lhs.rank_[0] == rhs.rank_[0] && lhs.rank_[1] < rhs.rank_[1]);
};

void CountSort(std::vector<Suffix>& suffices, const IntType radix,
               const IntType min, const IntType max) {
  std::vector<Suffix> output(suffices.size());
  const IntType range = max - min + 1;
  std::vector<IntType> count(range);
  for (IntType i = 0; i < suffices.size(); ++i) {
    ++count[suffices[i].rank_[radix] - min];
  }

  for (IntType i = 1; i < range; ++i) {
    count[i] += count[i - 1];
  }

  for (IntType i = suffices.size() - 1; i >= 0; --i) {
    output[count[suffices[i].rank_[radix] - min] - 1] = suffices[i];
    --count[suffices[i].rank_[radix] - min];
  }
  suffices = std::move(output);
}

void RadixSort(std::vector<Suffix>& suffices, const IntType max) {
  CountSort(suffices, 1, -1, max);
  CountSort(suffices, 0, 0, max);
}

std::vector<Suffix> BuildSuffixArray(const std::string& input_string) {
  std::vector<Suffix> suffix_array(input_string.size());
  for (IntType i = 0; i < suffix_array.size(); ++i) {
    suffix_array[i].suf_index_ = i;
    suffix_array[i].rank_[0] = input_string[i] - 'a';
    suffix_array[i].rank_[1] =
        ((i + 1) < input_string.size()) ? input_string[i + 1] - 'a' : -1;
  }
  std::sort(suffix_array.begin(), suffix_array.end(), is_less);

  std::vector<IntType> indices(suffix_array.size());
  for (IntType k = 2; k < suffix_array.size(); k *= 2) {
    IntType curr_rank = 0;
    IntType prev_rank = suffix_array[0].rank_[0];
    suffix_array[0].rank_[0] = curr_rank;
    indices[suffix_array[0].suf_index_] = 0;

    for (IntType i = 1; i < suffix_array.size(); ++i) {
      if (suffix_array[i].rank_[0] == prev_rank &&
          suffix_array[i].rank_[1] == suffix_array[i - 1].rank_[1]) {
        prev_rank = suffix_array[i].rank_[0];
        suffix_array[i].rank_[0] = curr_rank;
      } else {
        prev_rank = suffix_array[i].rank_[0];
        suffix_array[i].rank_[0] = ++curr_rank;
      }
      indices[suffix_array[i].suf_index_] = i;
    }

    for (IntType i = 0; i < suffix_array.size(); ++i) {
      int next_index = suffix_array[i].suf_index_ + k;
      suffix_array[i].rank_[1] =
          (next_index < suffix_array.size())
              ? suffix_array[indices[next_index]].rank_[0]
              : -1;
    }
    RadixSort(suffix_array, curr_rank);
  }
  return suffix_array;
}

std::vector<IntType> LCPArray(const std::string& input_str,
                              const std::vector<Suffix>& suffices) {
  std::vector<IntType> lcp_array(suffices.size());
  std::vector<IntType> inv_suff(suffices.size());
  for (IntType i = 0; i < suffices.size(); ++i) {
    inv_suff[suffices[i].suf_index_] = i;
  }
  IntType curr_lcp_size = 0;
  for (IntType i = 0; i < suffices.size(); ++i) {
    if (inv_suff[i] == suffices.size() - 1) {
      curr_lcp_size = 0;
      continue;
    }
    IntType j = suffices[inv_suff[i] + 1].suf_index_;
    while (i + curr_lcp_size < suffices.size() &&
           j + curr_lcp_size < suffices.size() &&
           input_str[i + curr_lcp_size] == input_str[j + curr_lcp_size]) {
      ++curr_lcp_size;
    }
    lcp_array[inv_suff[i]] = curr_lcp_size;
    if (curr_lcp_size > 0) {
      --curr_lcp_size;
    }
  }
  return lcp_array;
}

IntType CountDistinctSubstrings(const std::string& input_str) {
  std::vector<Suffix> suff_arr = BuildSuffixArray(input_str);
  std::vector<IntType> lcp_arr = LCPArray(input_str, suff_arr);

  IntType result = input_str.size() - suff_arr[0].suf_index_;
  for (IntType i = 1; i < lcp_arr.size(); ++i) {
    result += (input_str.size() - suff_arr[i].suf_index_) - lcp_arr[i - 1];
  }
  return result;
}

bool AreInDifferentStrings(const IntType suff_a_idx, const IntType suff_b_idx,
                           const IntType sharp_idx) {
  return (suff_a_idx < sharp_idx && suff_b_idx > sharp_idx) ||
         (suff_a_idx > sharp_idx && suff_b_idx < sharp_idx);
}

std::pair<IntType, IntType> FindKthCommonSubstring(const std::string& str_a,
                                                   const std::string& str_b,
                                                   const uint64_t k) {
  std::pair<IntType, IntType> answer;
  std::string buffer_string = str_a + '#' + str_b;
  auto suff_arr = BuildSuffixArray(buffer_string);
  auto lcp_arr = LCPArray(buffer_string, suff_arr);
  for (IntType i = 0; i < lcp_arr.size() - 1; ++i) {
    IntType comm_subtr_count = 0;
    if (AreInDifferentStrings(suff_arr[i].suf_index_,
                              suff_arr[i + 1].suf_index_, str_a.size())) {
    }
  }
  return answer;
}

const IntType max_string_size = 100'000;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string string_a;
  string_a.reserve(max_string_size);
  std::string string_b;
  string_a.reserve(max_string_size);
  uint64_t k = 0;
  std::cin >> string_a >> string_b >> k;
  auto answer = FindKthCommonSubstring(string_a, string_b, k);
  std::cout << (answer.empty() ? "-1" : answer) << std::endl;
  return 0;
}