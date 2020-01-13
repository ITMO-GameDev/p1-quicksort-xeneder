#include <stack>

template<typename K, typename V>
class Dictionary final {
public:
    struct Node {
    public:
        K key;
        V value;
        Node* left;
        Node* right;
        int height;
        bool isRed;

        explicit Node(K key, V value) : key(key), value(value), left(nullptr), right(nullptr), height(1),
                                        isRed(true) {};
    };

    class Iterator {
    private:
        Dictionary<K, V>& dictionary;
        Node* nodePointer;
        int counter;
        std::stack<Node*> nextStack;
        std::stack<Node*> prevStack;
    public:
        explicit Iterator(Dictionary<K, V>& dictionary) :
                dictionary(dictionary),
                nodePointer(dictionary.rootNode),
                counter(0) {};

        const K& key() const {
          return this->nodePointer->key;
        };

        const V& get() const {
          return this->nodePointer->value;
        };

        void set(const V& value) {
          this->nodePointer->val = value;
        };

        void next() {
          if (this->nodePointer == nullptr) return;

          this->prevStack.push(this->nodePointer);

          if (this->nodePointer->right != nullptr) {
            this->nextStack.push(this->nodePointer->right);
          }
          if (this->nodePointer->left != nullptr) {
            this->nodePointer = this->nodePointer->left;
          } else {
            this->nodePointer = this->nextStack.top();
            this->nextStack.pop();
          }
          this->counter++;
        };

        void prev() {
          this->nodePointer = this->prevStack.top();
          this->prevStack.pop();
          this->counter--;
        };

        bool hasNext() const {
          return this->counter < this->dictionary.size - 1;
        };

        bool hasPrev() const {
          return this->nodePointer != this->dictionary.rootNode;
        };
    };

    Node* rootNode;
    int size;

    Dictionary() : rootNode(nullptr), size(0) {};

    Dictionary(const Dictionary&) = delete;

    Dictionary(Dictionary&&) = delete;

    ~Dictionary() {
      this->deleteNode(rootNode);
    }

    void deleteNode(Node* node) {
      if (node != nullptr) {
        this->deleteNode(node->left);
        this->deleteNode(node->right);
        delete node;
      }
    }

    void put(const K& key, const V& value) {
      this->rootNode = this->findAndInsertNode(this->rootNode, key, value);
      this->rootNode->isRed = false;
    };

    Node* findAndInsertNode(Node* node, const K& key, const V& value) {
      if (node == nullptr) {
        this->size++;
        return new Node(key, value);
      }

      if (key == node->key) {
        node->value = value;
      } else {
        if (key < node->key) {
          node->left = this->findAndInsertNode(node->left, key, value);
        } else {
          node->right = this->findAndInsertNode(node->right, key, value);
        }
      }

      node = rebalance(node);

      return node;
    }

    void remove(const K& key) {
      this->rootNode = this->findAndRemoveNode(rootNode, key);
    };

    Node* findAndRemoveNode(Node* node, const K& key) {
      if (key < this->rootNode->key) {
        if ((node->left == nullptr || !node->left->isRed) &&
                (node->left == nullptr || node->left->left == nullptr || !node->left->left->isRed))
          node = moveRedLeft(node);
        node->left = findAndRemoveNode(node->left, key);
      } else {
        if (node->left != nullptr && node->left->isRed)
          node = rotateRight(node);
        if (key == node->key && node->right == nullptr) {
          this->size--;
          return nullptr;
        }

        if ((node->right == nullptr || !node->right->isRed) &&
            (node->left == nullptr || node->left->right == nullptr || !node->left->right->isRed))
          node = moveRedRight(node);

        if (key == node->key) {
          Node* mn = minNode(node->right);
          node->value = mn->value;
          node->key = mn->key;
          node->right = deleteMin(node->right);
          this->size--;
          delete mn;
        } else {
          node->right = findAndRemoveNode(node->right, key);
        }
      }

      return rebalance(node);
    };

    Node* minNode(Node* node) {
      return (node->left == nullptr) ? node : minNode(node->left);
    }

    Node* deleteMin(Node* node) {
      if (node->left == nullptr) {
        return nullptr;
      }

      if ((node->left == nullptr || !node->left->isRed) &&
          (node->left == nullptr || node->left->left == nullptr || !node->left->left->isRed))
        node = moveRedLeft(node);

      node->left = deleteMin(node->left);
      return rebalance(node);
    }

    bool contains(const K& key) {
      Node* buffer = this->rootNode;
      while (buffer != nullptr) {
        if (key == buffer->key) {
          return true;
        } else {
          if (key < buffer->key) {
            buffer = buffer->left;
          } else {
            buffer = buffer->right;
          }
        }
      }
      return false;
    };

    Node* rebalance(Node* node) {
      if ((node->right != nullptr && node->right->isRed) && (node->left == nullptr || !node->left->isRed))
        node = rotateLeft(node);

      if (node->left != nullptr && node->left->left != nullptr && node->left->isRed && node->left->left->isRed)
        node = rotateRight(node);

      if (node->left != nullptr && node->right != nullptr && node->left->isRed && node->right->isRed)
        flipColors(node);

      return node;
    }

    Node* rotateLeft(Node* node) {
      Node* buffer = node->right;
      node->right = buffer->left;
      buffer->left = node;
      buffer->isRed = node->isRed;
      node->isRed = true;
      return buffer;
    }

    Node* rotateRight(Node* node) {
      Node* buffer = node->left;
      node->left = buffer->right;
      buffer->right = node;
      buffer->isRed = node->isRed;
      node->isRed = true;
      return buffer;
    }

    Node* moveRedLeft(Node* node) {
      flipColors(node);
      if (node->right->left->isRed) {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
        flipColors(node);
      }
      return node;
    }

    Node* moveRedRight(Node* node) {
      flipColors(node);
      if (node->left == nullptr || node->left->left == nullptr || !node->left->left->isRed) {
        node = rotateRight(node);
        flipColors(node);
      }
      return node;
    }

    void flipColors(Node* node) {
      node->isRed = !node->isRed;
      if (node->left != nullptr)
        node->left->isRed = !node->left->isRed;
      if (node->right != nullptr)
        node->right->isRed = !node->right->isRed;
    }

    V& search(K& key) {
      Node* buffer = this->rootNode;
      while (buffer != nullptr) {
        if (key == buffer->key) {
          return buffer->value;
        } else {
          if (key < buffer->key) {
            buffer = buffer->left;
          } else {
            buffer = buffer->right;
          }
        }
      }

      auto none = V();
      return none;
    }

    const V& operator[](const K& key) const {
      if (this->contains(key)) {
        return this->search(key);
      } else {
        return V();
      }
    };

    V& operator[](const K& key) {
      K k = key;
      if (this->contains(key)) {
        return this->search(k);
      } else {
        V v = V();
        this->put(k, v);
        return v;
      }
    };

    Dictionary& operator=(const Dictionary&) = delete;

    Dictionary& operator=(const Dictionary&&) = delete;

    Dictionary<K, V>::Iterator iterator() {
      return Iterator(*this);
    };
};
