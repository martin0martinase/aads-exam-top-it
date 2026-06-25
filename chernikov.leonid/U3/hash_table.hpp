#ifndef CHERNIKOV_HASH_TABLE_HPP
#define CHERNIKOV_HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <functional>
#include <stdexcept>

namespace chernikov
{

  template <class Key, class Value, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
  struct HashTable
  {
    struct Node
    {
      Key key;
      Value value;
      Node *next;
    };

    struct Iterator
    {
      Node **table;
      std::size_t slotCount;
      std::size_t currentSlot;
      Node *currentNode;
    };

    Node **table;
    std::size_t slotCount;
    std::size_t elementCount;
    Hash hash;
    Equal equal;
  };

  template <class Key, class Value, class Hash, class Equal>
  void initHashTable(HashTable<Key, Value, Hash, Equal> &ht, std::size_t slotCount = 16)
  {
    ht.table = new typename HashTable<Key, Value, Hash, Equal>::Node *[slotCount]();
    ht.slotCount = slotCount;
    ht.elementCount = 0;
  }

  template <class Key, class Value, class Hash, class Equal>
  void destroyHashTable(HashTable<Key, Value, Hash, Equal> &ht)
  {
    for (std::size_t i = 0; i < ht.slotCount; ++i)
    {
      auto *current = ht.table[i];
      while (current != nullptr)
      {
        auto *next = current->next;
        delete current;
        current = next;
      }
    }
    delete[] ht.table;
  }

  template <class Key, class Value, class Hash, class Equal>
  typename HashTable<Key, Value, Hash, Equal>::Node *
  findNode(const HashTable<Key, Value, Hash, Equal> &ht, const Key &key)
  {
    std::size_t slot = ht.hash(key) % ht.slotCount;
    auto *current = ht.table[slot];
    while (current != nullptr)
    {
      if (ht.equal(current->key, key))
      {
        return current;
      }
      current = current->next;
    }
    return nullptr;
  }

  template <class Key, class Value, class Hash, class Equal>
  void addToHashTable(HashTable<Key, Value, Hash, Equal> &ht, const Key &key, const Value &value)
  {
    auto *existingNode = findNode(ht, key);
    if (existingNode != nullptr)
    {
      existingNode->value = value;
      return;
    }

    std::size_t slot = ht.hash(key) % ht.slotCount;
    auto *newNode = new typename HashTable<Key, Value, Hash, Equal>::Node{key, value, ht.table[slot]};
    ht.table[slot] = newNode;
    ++ht.elementCount;
  }

  template <class Key, class Value, class Hash, class Equal>
  Value dropFromHashTable(HashTable<Key, Value, Hash, Equal> &ht, const Key &key)
  {
    std::size_t slot = ht.hash(key) % ht.slotCount;
    auto *current = ht.table[slot];
    typename HashTable<Key, Value, Hash, Equal>::Node *prev = nullptr;

    while (current != nullptr && !ht.equal(current->key, key))
    {
      prev = current;
      current = current->next;
    }

    if (current == nullptr)
    {
      throw std::out_of_range("Key not found");
    }

    if (prev == nullptr)
    {
      ht.table[slot] = current->next;
    }
    else
    {
      prev->next = current->next;
    }

    Value droppedValue = current->value;
    delete current;
    --ht.elementCount;
    return droppedValue;
  }

  template <class Key, class Value, class Hash, class Equal>
  bool hasInHashTable(const HashTable<Key, Value, Hash, Equal> &ht, const Key &key)
  {
    return findNode(ht, key) != nullptr;
  }

  template <class Key, class Value, class Hash, class Equal>
  Value &getFromHashTable(HashTable<Key, Value, Hash, Equal> &ht, const Key &key)
  {
    auto *node = findNode(ht, key);
    if (node == nullptr)
    {
      addToHashTable(ht, key, Value{});
      node = findNode(ht, key);
    }
    return node->value;
  }

  template <class Key, class Value, class Hash, class Equal>
  const Value &getFromHashTable(const HashTable<Key, Value, Hash, Equal> &ht, const Key &key)
  {
    auto *node = findNode(ht, key);
    if (node == nullptr)
    {
      throw std::out_of_range("Key not found");
    }
    return node->value;
  }

  template <class Key, class Value, class Hash, class Equal>
  void rehashHashTable(HashTable<Key, Value, Hash, Equal> &ht, std::size_t newSlotCount)
  {
    auto **oldTable = ht.table;
    std::size_t oldSlotCount = ht.slotCount;

    ht.table = new typename HashTable<Key, Value, Hash, Equal>::Node *[newSlotCount]();
    ht.slotCount = newSlotCount;

    for (std::size_t i = 0; i < oldSlotCount; ++i)
    {
      auto *current = oldTable[i];
      while (current != nullptr)
      {
        auto *next = current->next;
        std::size_t newSlot = ht.hash(current->key) % newSlotCount;
        current->next = ht.table[newSlot];
        ht.table[newSlot] = current;
        current = next;
      }
    }

    delete[] oldTable;
  }

  template <class Key, class Value, class Hash, class Equal>
  typename HashTable<Key, Value, Hash, Equal>::Iterator
  beginHashTable(HashTable<Key, Value, Hash, Equal> &ht)
  {
    typename HashTable<Key, Value, Hash, Equal>::Iterator it;
    it.table = ht.table;
    it.slotCount = ht.slotCount;
    it.currentSlot = 0;
    it.currentNode = nullptr;

    while (it.currentSlot < it.slotCount && it.table[it.currentSlot] == nullptr)
    {
      ++it.currentSlot;
    }

    if (it.currentSlot < it.slotCount)
    {
      it.currentNode = it.table[it.currentSlot];
    }

    return it;
  }

  template <class Key, class Value, class Hash, class Equal>
  typename HashTable<Key, Value, Hash, Equal>::Iterator
  endHashTable(HashTable<Key, Value, Hash, Equal> &ht)
  {
    typename HashTable<Key, Value, Hash, Equal>::Iterator it;
    it.table = ht.table;
    it.slotCount = ht.slotCount;
    it.currentSlot = ht.slotCount;
    it.currentNode = nullptr;
    return it;
  }

  template <class Iterator>
  void advanceIterator(Iterator &it)
  {
    if (it.currentNode != nullptr)
    {
      it.currentNode = it.currentNode->next;
    }

    if (it.currentNode != nullptr)
    {
      return;
    }

    ++it.currentSlot;
    while (it.currentSlot < it.slotCount && it.table[it.currentSlot] == nullptr)
    {
      ++it.currentSlot;
    }

    if (it.currentSlot < it.slotCount)
    {
      it.currentNode = it.table[it.currentSlot];
    }
  }

  template <class Iterator>
  bool iteratorsEqual(const Iterator &a, const Iterator &b)
  {
    return a.currentSlot == b.currentSlot && a.currentNode == b.currentNode;
  }

  template <class Iterator>
  bool iteratorsNotEqual(const Iterator &a, const Iterator &b)
  {
    return !iteratorsEqual(a, b);
  }

  template <class Key, class Value, class Hash, class Equal>
  std::size_t sizeHashTable(const HashTable<Key, Value, Hash, Equal> &ht)
  {
    return ht.elementCount;
  }

  template <class Key, class Value, class Hash, class Equal>
  bool emptyHashTable(const HashTable<Key, Value, Hash, Equal> &ht)
  {
    return ht.elementCount == 0;
  }

}

#endif
