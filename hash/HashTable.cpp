#include "HashTable.h"
#include <stdexcept>

HashTable::HashTable(size_t size) noexcept
    : _capacity(static_cast<int32_t>(size)), _filled(0), table(size)
{}

HashTable::~HashTable() = default;

size_t HashTable::hash_function(const KeyType &key) const
{
    size_t hash = 0;
    for (char c : key)
        hash = hash * 31 + static_cast<unsigned char>(c);
    return hash % static_cast<size_t>(_capacity);
}

void HashTable::insert(const KeyType &key, const ValueType &value)
{
    if (getLoadFactor() > 0.75)
    {
        int32_t newCapacity = _capacity * 2;
        std::vector<std::list<std::pair<KeyType, ValueType>>> newTable(newCapacity);

        for (auto &bucket : table)
            for (auto &pair : bucket)
            {
                size_t idx = 0;
                for (char c : pair.first)
                    idx = idx * 31 + static_cast<unsigned char>(c);
                idx %= static_cast<size_t>(newCapacity);
                newTable[idx].emplace_back(pair);
            }

        table = std::move(newTable);
        _capacity = newCapacity;
    }

    size_t idx = hash_function(key);
    for (auto &pair : table[idx])
        if (pair.first == key)
        {
            pair.second = value;
            return;
        }

    table[idx].emplace_back(key, value);
    ++_filled;
}

bool HashTable::find(const KeyType &key, ValueType &value) const
{
    size_t idx = hash_function(key);
    for (const auto &pair : table[idx])
        if (pair.first == key)
        {
            value = pair.second;
            return true;
        }
    return false;
}

void HashTable::remove(const KeyType &key)
{
    size_t idx = hash_function(key);
    auto &bucket = table[idx];
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
        if (it->first == key)
        {
            bucket.erase(it);
            --_filled;
            return;
        }
}

ValueType& HashTable::operator[](const KeyType &key)
{
    size_t idx = hash_function(key);
    for (auto &pair : table[idx])
        if (pair.first == key)
            return pair.second;

    insert(key, ValueType{});
    idx = hash_function(key);
    for (auto &pair : table[idx])
        if (pair.first == key)
            return pair.second;

    throw std::runtime_error("HashTable: unexpected error in operator[]");
}

double HashTable::getLoadFactor()
{
    return static_cast<double>(_filled) / static_cast<double>(_capacity);
}
