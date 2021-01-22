#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

struct Suffix {
  int suf_index_;
  std::array<int, 2> rank_;

  static bool CompareByRank(const Suffix& lhs, const Suffix& rhs) {
    return lhs.rank_[0] < rhs.rank_[0] ||
           (lhs.rank_[0] == rhs.rank_[0] && lhs.rank_[1] < rhs.rank_[1]);
  }
};

void CountSort(std::vector<Suffix>& suffices, const int radix, const int min,
               const int max) {
  std::vector<Suffix> output(suffices.size());
  const int range = max - min + 1;
  std::vector<int> count(range);
  for (int i = 0; i < suffices.size(); ++i) {
    ++count[suffices[i].rank_[radix] - min];
  }

  for (int i = 1; i < range; ++i) {
    count[i] += count[i - 1];
  }

  for (int i = suffices.size() - 1; i >= 0; --i) {
    output[count[suffices[i].rank_[radix] - min] - 1] = suffices[i];
    --count[suffices[i].rank_[radix] - min];
  }
  suffices = std::move(output);
}

void RadixSort(std::vector<Suffix>& suffices, const int max) {
  CountSort(suffices, 1, -1, max);
  CountSort(suffices, 0, 0, max);
}

std::vector<Suffix> BuildSuffixArray(const std::string& input_string) {
  std::vector<Suffix> suffix_array(input_string.size());
  for (int i = 0; i < suffix_array.size(); ++i) {
    suffix_array[i].suf_index_ = i;
    suffix_array[i].rank_[0] = input_string[i] - 'a';
    suffix_array[i].rank_[1] =
        ((i + 1) < input_string.size()) ? input_string[i + 1] - 'a' : -1;
  }
  std::sort(suffix_array.begin(), suffix_array.end(), Suffix::CompareByRank);

  std::vector<int> indices(suffix_array.size());
  for (int k = 2; k < suffix_array.size(); k *= 2) {
    int curr_rank = 0;
    int prev_rank = suffix_array[0].rank_[0];
    suffix_array[0].rank_[0] = curr_rank;
    indices[suffix_array[0].suf_index_] = 0;

    for (int i = 1; i < suffix_array.size(); ++i) {
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

    for (int i = 0; i < suffix_array.size(); ++i) {
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

std::vector<int> LCPArray(const std::string& input_str,
                          const std::vector<Suffix>& suffices) {
  std::vector<int> lcp_array(suffices.size());
  std::vector<int> inv_suff(suffices.size());
  for (int i = 0; i < suffices.size(); ++i) {
    inv_suff[suffices[i].suf_index_] = i;
  }
  int curr_lcp_size = 0;
  for (int i = 0; i < suffices.size(); ++i) {
    if (inv_suff[i] == suffices.size() - 1) {
      curr_lcp_size = 0;
      continue;
    }
    int j = suffices[inv_suff[i] + 1].suf_index_;
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

int CountDistinctSubstrings(const std::string& input_str) {
  std::vector<Suffix> suff_arr = BuildSuffixArray(input_str);
  std::vector<int> lcp_arr = LCPArray(input_str, suff_arr);

  int result = input_str.size() - suff_arr[0].suf_index_;
  for (int i = 1; i < lcp_arr.size(); ++i) {
    result += (input_str.size() - suff_arr[i].suf_index_) - lcp_arr[i - 1];
  }
  return result;
}

int main() {
  std::string text;
  std::cin >> text;
  std::cout << CountDistinctSubstrings(text) << std::endl;
  return 0;
}