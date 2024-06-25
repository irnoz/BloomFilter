import random
import hashlib
from bitarray import bitarray

class BloomFilter:
    def __init__(self, size, hash_count):
        self.size = size
        self.hash_count = hash_count
        self.bit_array = bitarray(size)
        self.bit_array.setall(0)

    def _hashes(self, item):
        return [int(hashlib.sha256((item + str(i)).encode()).hexdigest(), 16) % self.size for i in range(self.hash_count)]

    def add(self, item):
        for hash_value in self._hashes(item):
            self.bit_array[hash_value] = 1

    def check(self, item):
        return all(self.bit_array[hash_value] for hash_value in self._hashes(item))

class ElementCollection:
    def __init__(self, size, hash_count):
        self.bloom_filter = BloomFilter(size, hash_count)
        self.elements = set()

    def add(self, item):
        self.elements.add(item)
        self.bloom_filter.add(item)

    def check(self, item):
        if self.bloom_filter.check(item):
            if item in self.elements:
                return True
        return False

collections = [ElementCollection(size=500, hash_count=7) for _ in range(5)]

with open('../data/in/sample_data.txt', 'r') as file:
    sample_text = file.read()

words = sample_text.split()
for word in words:
    collection = random.choice(collections)
    collection.add(word)

def check_word(word):
    found = False
    for i, collection in enumerate(collections):
        if collection.check(word):
            found = True
            print(f"'{word}' is possibly in collection {i + 1}")
            if word in collection.elements:
                print(f"'{word}' is actually in collection {i + 1}")
            else:
                print(f"'{word}' is a false positive in collection {i + 1}")
    if not found:
        print(f"'{word}' is not in any collection")

words_to_check = ["Bloom", "filter", "larger", "element", "irakli", "definitely", "not", "bla", "ahaaa", "Bloon"]

for word in words_to_check:
    check_word(word)
    print()
