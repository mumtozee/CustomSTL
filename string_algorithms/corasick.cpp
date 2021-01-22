#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

const int null = -1;

struct TrieNode {
  std::unordered_map<char, int> son_;
  std::unordered_map<char, int> go_;
  int parent_;
  int sufflink_;
  int up_;
  char char_to_parent_;
  bool is_leaf_;
  std::vector<int> leaf_pattern_number_;

  TrieNode(int parent = null, char char_to_parent = '$', bool is_leaf = true)
      : son_({}),
        go_({}),
        parent_(parent),
        sufflink_(null),
        up_(null),
        char_to_parent_(char_to_parent),
        is_leaf_(is_leaf),
        leaf_pattern_number_({}) {}
};

class Trie {
 public:
  Trie() : nodes_{{}} {}
  Trie(const std::vector<std::pair<std::string, int>>& trie_strings) : Trie() {
    for (const auto& item : trie_strings) {
      AddString(item.first, item.second);
    }
  }
  Trie(const Trie& other) = default;
  Trie(Trie&& other) = default;
  Trie& operator=(const Trie& other) = default;
  Trie& operator=(Trie&& other) = default;
  ~Trie() = default;

  int GetSuffLink(int node_idx) {
    if (nodes_[node_idx].sufflink_ == null) {
      if (node_idx == 0 || nodes_[node_idx].parent_ == 0) {
        nodes_[node_idx].sufflink_ = 0;
      } else {
        nodes_[node_idx].sufflink_ =
            GetLink(GetSuffLink(nodes_[node_idx].parent_),
                    nodes_[node_idx].char_to_parent_);
      }
    }
    return nodes_[node_idx].sufflink_;
  }

  int GetLink(int node_idx, char c) {
    if (nodes_[node_idx].go_.find(c) == nodes_[node_idx].go_.end()) {
      if (nodes_[node_idx].son_.find(c) != nodes_[node_idx].son_.end()) {
        nodes_[node_idx].go_[c] = nodes_[node_idx].son_[c];
      } else if (node_idx == 0) {
        nodes_[node_idx].go_[c] = 0;
      } else {
        nodes_[node_idx].go_[c] = GetLink(GetSuffLink(node_idx), c);
      }
    }
    return nodes_[node_idx].go_[c];
  }

  int GetUp(int node_idx) {
    if (nodes_[node_idx].up_ == null) {
      int suflink_of_node = GetSuffLink(node_idx);
      if (nodes_[suflink_of_node].is_leaf_) {
        nodes_[node_idx].up_ = suflink_of_node;
      } else if (suflink_of_node == 0) {
        nodes_[node_idx].up_ = 0;
      } else {
        nodes_[node_idx].up_ = GetUp(suflink_of_node);
      }
    }
    return nodes_[node_idx].up_;
  }

  void AddString(const std::string& str, int pattern_num) {
    int curr_idx = 0;  // root
    for (char c : str) {
      if (nodes_[curr_idx].son_.find(c) == nodes_[curr_idx].son_.end()) {
        TrieNode new_node = {curr_idx, c, false};
        nodes_.push_back(new_node);
        nodes_[curr_idx].son_[c] = nodes_.size() - 1;
      }
      curr_idx = nodes_[curr_idx].son_[c];
    }
    nodes_[curr_idx].is_leaf_ = true;
    nodes_[curr_idx].leaf_pattern_number_.push_back(pattern_num);
  }

  TrieNode& operator[](int idx) { return nodes_[idx]; }

  const TrieNode& operator[](int idx) const { return nodes_[idx]; }

 private:
  std::vector<TrieNode> nodes_;
};

std::vector<std::pair<std::string, int>> SplitPatternToWordsWithEndIndices(
    const std::string& pattern, int& word_count) {
  int i = 0;
  std::vector<std::pair<std::string, int>> answer;
  while (i < pattern.size()) {
    std::string sub_str;
    int j = i;

    while (pattern[j] != '?' && j < pattern.size()) {
      sub_str.push_back(pattern[j]);
      ++j;
    }

    if (j - 1 >= 0) {
      answer.emplace_back(sub_str, j - 1);
      ++word_count;
    }

    while (pattern[j] == '?' && j < pattern.size()) {
      ++j;
    }
    i = j;
  }
  return answer;
}

void MarkBeginningOfEntry(const Trie& trie, int node_idx, int idx,
                          std::vector<int>& entry_start_indices) {
  for (const auto& index : trie[node_idx].leaf_pattern_number_) {
    int start_idx = idx - index;
    if (start_idx >= 0) {
      ++entry_start_indices[start_idx];
    }
  }
}

std::vector<int> FindEntries(const std::string& pattern,
                             const std::string& text) {
  int words_in_pattern = 0;
  auto trie_strings =
      SplitPatternToWordsWithEndIndices(pattern, words_in_pattern);
  Trie trie(trie_strings);

  std::vector<int> result_vector;
  std::vector<int> entry_start_indices(text.size(), 0);

  int curr_node = 0;

  for (int i = 0; i < text.size(); ++i) {
    curr_node = trie.GetLink(curr_node, text[i]);
    int curr_node_copy = curr_node;

    if (trie[curr_node_copy].is_leaf_) {
      MarkBeginningOfEntry(trie, curr_node_copy, i, entry_start_indices);
    }

    while (curr_node_copy != 0) {
      curr_node_copy = trie.GetUp(curr_node_copy);
      MarkBeginningOfEntry(trie, curr_node_copy, i, entry_start_indices);
    }
  }

  for (int i = 0; i < entry_start_indices.size(); ++i) {
    if (entry_start_indices[i] == words_in_pattern &&
        i + pattern.size() <= text.size()) {
      result_vector.push_back(i);
    }
  }

  return result_vector;
}

const int max_pattern_size = 5000;
const int max_text_size = 2'000'000;

int main() {
  std::string pattern;
  pattern.reserve(max_pattern_size);
  std::string text;
  text.reserve(max_text_size);

  std::cin >> pattern >> text;
  auto entries = FindEntries(pattern, text);

  for (const auto& entry : entries) {
    std::cout << entry << ' ';
  }
  return 0;
}