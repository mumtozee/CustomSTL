#include <iostream>
#include <string>
#include <vector>

struct Bucket {
    std::string val_;
    bool is_free_;
    bool deleted_;

    Bucket() : val_(""), is_free_(true), deleted_(false) {
    }

    explicit Bucket(std::string&& str) : val_(std::move(str)), is_free_(false), deleted_(false) {
    }

    explicit Bucket(const std::string& str) : val_(str), is_free_(false), deleted_(false) {
    }

    Bucket(Bucket&& other) noexcept = default;

    Bucket(const Bucket& other) = default;

    Bucket& operator=(const Bucket& other) = default;

    Bucket& operator=(Bucket&& other) noexcept = default;

    ~Bucket() = default;
};

class HashMap {
public:
    HashMap() : non_empty_(0), map_(8) {
    }

    HashMap(const HashMap& other) = default;

    HashMap(HashMap&& other) noexcept = default;

    HashMap& operator=(const HashMap& other) = default;

    HashMap& operator=(HashMap&& other) noexcept = default;

    ~HashMap() = default;

    long double GetLoadFactor() const {
        return static_cast<long double>(non_empty_) / static_cast<long double>(map_.size());
    }

    void Rehash() {
        size_t new_size = map_.size() * 2;
        std::vector<Bucket> tmp = std::move(map_);
        map_ = std::vector<Bucket>(new_size);
        non_empty_ = 0;
        for (const auto& item : tmp) {
            if (!item.is_free_ && !item.deleted_) {
                Add(item);
            }
        }
    }

    bool Add(const Bucket& item) {
        size_t h1 = hash1(item.val_);
        size_t h2 = hash2(item.val_);

        for (size_t i = 0; i < map_.size(); ++i) {
            if (map_[h1].is_free_ || map_[h1].deleted_) {
                map_[h1] = item;
                ++non_empty_;
                break;
            } else if (map_[h1].val_ == item.val_) {
                return false;
            }

            h1 = (h1 + h2) % map_.size();
        }

        if (GetLoadFactor() >= 0.75) {
            Rehash();
        }

        return true;
    }

    size_t Search(const Bucket& item) {
        size_t h1 = hash1(item.val_);
        size_t h2 = hash2(item.val_);

        for (size_t i = 0; i < map_.size(); ++i) {
            if (map_[h1].is_free_) {
                return map_.size();
            } else if (map_[h1].val_ == item.val_) {
                if (!map_[h1].deleted_) {
                    return h1;
                }
            }

            h1 = (h1 + h2) % map_.size();
        }

        return map_.size();
    }

    bool Delete(const Bucket& item) {
        size_t found_idx = Search(item);
        if (found_idx == map_.size()) {
            return false;
        } else {
            map_[found_idx].deleted_ = true;
            --non_empty_;
            return true;
        }
    }

    size_t Size() const {
        return map_.size();
    }

protected:
    size_t non_empty_;
    std::vector<Bucket> map_;

private:
    size_t ComputeKeyWithHorner(const std::string& str, size_t pow) const {
        size_t ans = 0;
        size_t exp = 1;

        for (const char c : str) {
            ans = (ans + c * exp) % Size();
            exp = (exp * pow) % Size();
        }

        return ans;
    }

    size_t hash1(const std::string& str) const {
        return ComputeKeyWithHorner(str, 29);
    }

    size_t hash2(const std::string& str) const {
        size_t ans = str.size() % (Size() - 1);
        return ((ans % 2 == 0) ? ans + 1 : ans);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    HashMap map;
    char operation;

    while (std::cin >> operation) {
        std::string item;
        std::cin >> item;
        if (operation == '+') {
            std::cout << (map.Add(Bucket(std::move(item))) ? "OK" : "FAIL") << '\n';
        } else if (operation == '-') {
            std::cout << (map.Delete(Bucket(std::move(item))) ? "OK" : "FAIL") << '\n';
        } else if (operation == '?') {
            std::cout << (map.Search(Bucket(std::move(item))) != map.Size() ? "OK" : "FAIL") << '\n';
        }
    }

    return 0;
}