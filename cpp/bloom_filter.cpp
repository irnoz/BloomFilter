#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>
#include <bitset>
#include <random>
#include <ctime>

const int BLOOM_FILTER_SIZE = 500;
const int HASH_COUNT = 7;

class BloomFilter {
public:
    BloomFilter() : bit_array(BLOOM_FILTER_SIZE) {
        std::random_device rd;
        for (int i = 0; i < HASH_COUNT; ++i) {
            hash_seeds.push_back(rd());
        }
    }

    void add(const std::string& item) {
        for (const auto& seed : hash_seeds) {
            size_t hash_value = hash_function(item, seed);
            bit_array[hash_value % BLOOM_FILTER_SIZE] = true;
        }
    }

    bool check(const std::string& item) const {
        for (const auto& seed : hash_seeds) {
            size_t hash_value = hash_function(item, seed);
            if (!bit_array[hash_value % BLOOM_FILTER_SIZE]) {
                return false;
            }
        }
        return true;
    }

private:
    std::bitset<BLOOM_FILTER_SIZE> bit_array;
    std::vector<size_t> hash_seeds;

    size_t hash_function(const std::string& item, size_t seed) const {
        std::hash<std::string> hasher;
        return hasher(item + std::to_string(seed));
    }
};

class ElementCollection {
public:
    void add(const std::string& item) {
        elements.insert(item);
        bloom_filter.add(item);
    }

    bool check(const std::string& item) const {
        if (bloom_filter.check(item)) {
            return elements.find(item) != elements.end();
        }
        return false;
    }

private:
    BloomFilter bloom_filter;
    std::unordered_set<std::string> elements;
};

std::vector<std::string> read_words_from_file(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            words.push_back(word);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
    return words;
}

void check_word(const std::string& word, const std::vector<ElementCollection>& collections) {
    bool found = false;
    for (size_t i = 0; i < collections.size(); ++i) {
        if (collections[i].check(word)) {
            found = true;
            std::cout << "'" << word << "' is possibly in collection " << i + 1 << std::endl;
            if (collections[i].check(word)) {
                std::cout << "'" << word << "' is actually in collection " << i + 1 << std::endl;
            } else {
                std::cout << "'" << word << "' is a false positive in collection " << i + 1 << std::endl;
            }
        }
    }
    if (!found) {
        std::cout << "'" << word << "' is not in any collection" << std::endl;
    }
}

int main() {
    std::vector<ElementCollection> collections(5);

    std::vector<std::string> words = read_words_from_file("../data/in/sample_data.txt");

    std::default_random_engine generator(static_cast<unsigned>(time(0)));
    std::uniform_int_distribution<int> distribution(0, collections.size() - 1);

    for (const auto& word : words) {
        int index = distribution(generator);
        collections[index].add(word);
    }

    std::vector<std::string> words_to_check = {"Bloom", "filter", "larger", "element", "irakli", "definitely", "not", "bla", "ahaaa", "bloom"};

    for (const auto& word : words_to_check) {
        check_word(word, collections);
        std::cout << std::endl;
    }

    return 0;
}
