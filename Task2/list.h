#include "array.h"

const int CHUNK_SIZE = 8;

template<typename T>
class List {
private:
    struct Chunk {
        Array<T>* containerPointer;
        int chunkSize;
        int chunkCapacity;
        Chunk* next;
        Chunk* prev;

        Chunk(const int capacity, Chunk* prev_chunk, Chunk* next_chunk) :
                chunkSize(0),
                chunkCapacity(capacity),
                next(next_chunk),
                prev(prev_chunk) {
          containerPointer = new Array<T>(capacity);
        }

        Chunk(const Chunk& other) :
                chunkSize(other.chunkSize),
                chunkCapacity(other.chunkCapacity),
                next(nullptr),
                prev(nullptr) {
          this->containerPointer = new Array<T>(*other.containerPointer);
        }

        Chunk(const Chunk&& other) noexcept :
                chunkSize(other.chunkSize),
                chunkCapacity(other.chunkCapacity),
                next(nullptr),
                prev(nullptr) {
          this->containerPointer = new Array<T>(*other.containerPointer);

          delete *other.containerPointer;
          other.chunkSize = 0;
          other.chunkCapacity = 0;
          other.prev = nullptr;
          other.next = nullptr;
        }

        ~Chunk() {
          delete containerPointer;
        };

        void prepend(const T& value) {
          this->containerPointer->insert(0, value);
          this->chunkSize++;
        }

        void append(const T& value) {
          this->containerPointer->insert(value);
          this->chunkSize++;
        }

        int getSize() {
          return this->chunkSize;
        }

        void removeFirstElement() {
          this->containerPointer->remove(0);
          this->chunkSize--;
        }

        void removeLastElement() {
          this->containerPointer->remove(this->containerPointer->getSize() - 1);
          this->chunkSize--;
        }

        const T& getAtPosition(int position) {
          auto it = this->containerPointer->iterator();
          it.toIndex(position);
          return it.get();
        }

        const T& getFirst() {
          return getAtPosition(0);
        }

        const T& getLast() {
          return this->getAtPosition(this->containerPointer->getSize() - 1);
        }

        bool isFilled() {
          return this->chunkCapacity == this->chunkSize;
        }

        Chunk& operator=(const Chunk& other) {
          if (&other == this)
            return *this;

          this->containerPointer = nullptr;

          this->containerPointer = new Array<T>(this->chunkCapacity);

          this->chunkCapacity = other.chunkCapacity;
          this->chunkSize = other.chunkSize;
          this->chunkCapacity = other.chunkCapacity;
          this->prev = other.prev;
          this->next = other.next;

          return *this;
        }

        Chunk& operator=(Chunk&& other) noexcept {
          if (&other != this) {
            this->containerPointer = nullptr;

            this->containerPointer = new Array<T>(this->chunkCapacity);

            this->chunkCapacity = other.chunkCapacity;
            this->chunkSize = other.chunkSize;
            this->chunkCapacity = other.chunkCapacity;
            this->prev = other.prev;
            this->next = other.next;

            other.containerPointer = nullptr;
            other.next = nullptr;
            other.prev = nullptr;
            other.chunkCapacity = 0;
            other.chunkSize = 0;
          }

          return *this;
        }
    };

public:
    class Iterator {
    private:
        List<T>& list;
        Chunk* chunkPointer;
        int position;
    public:
        explicit Iterator(List<T>& list) : list(list), position(0), chunkPointer(list.headChunkPointer) {}

        const T& get() const {
          return this->chunkPointer->getAtPosition(position);
        }

        void set(const T& value) {
          this->chunkPointer->containerPointer->pointer[this->position] = value;
        }

        void insert(const T& value) {
          T lastElement = value;
          Chunk* currentChunkPointer = this->chunkPointer;
          int currentPosition = position;
          while (currentChunkPointer != nullptr) {
            T buffer = lastElement;
            lastElement = currentChunkPointer->getLast();
            std::memmove(currentChunkPointer->containerPointer->pointer + currentPosition + 1,
                         currentChunkPointer->containerPointer->pointer + currentPosition,
                         (currentChunkPointer->chunkSize - currentPosition - 1) * sizeof(T));
            currentChunkPointer->containerPointer->pointer[this->position] = buffer;
            if (!currentChunkPointer->isFilled() && currentChunkPointer->next == nullptr) {
              currentChunkPointer->chunkSize++;
            }
            currentChunkPointer = currentChunkPointer->next;
            currentPosition = 0;
          }
          this->list.size++;
        }

        void remove() {
          T firstElement;
          bool isFirstElementEmpty = true;
          Chunk* currentChunkPointer = this->list.tailChunkPointer;
          while (currentChunkPointer != this->chunkPointer) {
            T buffer = firstElement;
            firstElement = currentChunkPointer->getFirst();
            std::memmove(currentChunkPointer->containerPointer->pointer,
                         currentChunkPointer->containerPointer->pointer + 1,
                         (currentChunkPointer->chunkSize - 1) * sizeof(T));
            if (isFirstElementEmpty) {
              isFirstElementEmpty = false;
            } else {
              currentChunkPointer->containerPointer[currentChunkPointer->chunkSize - 1] = firstElement;
            }
            currentChunkPointer = currentChunkPointer->prev;
          }

          std::memmove(currentChunkPointer->containerPointer->pointer + (this->position),
                       currentChunkPointer->containerPointer->pointer + this->position + 1,
                       (currentChunkPointer->chunkSize - this->position - 1) * sizeof(T));

          if (firstElement == NULL) {
            this->chunkPointer->chunkSize--;
          } else {
            this->chunkPointer->containerPointer->pointer[this->chunkPointer->chunkSize - 1] = firstElement;
          }
          this->list.size--;
        }

        void next() {
          if (this->position == this->chunkPointer->chunkSize - 1) {
            if (this->chunkPointer->next == nullptr) {
              throw std::range_error("Position out of bounds");
            } else {
              this->position = 0;
              this->chunkPointer = this->chunkPointer->next;
            }
          } else {
            this->position++;
          }
        }

        void prev() {
          if (this->position == 0) {
            if (this->chunkPointer->prev == nullptr) {
              throw std::range_error("Position out of bounds");
            } else {
              this->position = this->chunkPointer->prev->chunkSize - 1;
              this->chunkPointer = this->chunkPointer->prev;
            }
          } else {
            this->position--;
          }
        }

        bool hasNext() const {
          return this->position < this->chunkPointer->getSize() - 1 || this->chunkPointer->next != nullptr;
        }

        bool hasPrev() const {
          return this->position > 0 || this->chunkPointer->prev != nullptr;
        }
    };

    Chunk* headChunkPointer;
    Chunk* tailChunkPointer;
    int size;

    List() : size(0) {
      this->tailChunkPointer = new Chunk(CHUNK_SIZE, nullptr, nullptr);
      this->headChunkPointer = this->tailChunkPointer;
    }

    ~List() {
      Chunk* nextPointer;
      for (Chunk* i = this->headChunkPointer; i != nullptr; i = nextPointer) {
        nextPointer = i->next;
        i->next = nullptr;
        i->prev = nullptr;
      }
    }

    List(const List& other) : size(other.size) {
      this->headChunkPointer = new Chunk(*other.headChunkPointer);
      Chunk* currentPointer = this->headChunkPointer;
      Chunk* otherPointer = other.headChunkPointer;
      while (otherPointer->next != nullptr) {
        currentPointer->next = new Chunk(*otherPointer->next);
        currentPointer->next->prev = currentPointer;
        currentPointer = currentPointer->next;
        otherPointer = otherPointer->next;
      }
      this->tailChunkPointer = currentPointer;
      this->tailChunkPointer->next = nullptr;
    }

    List(List&& other) noexcept : size(other.size) {
      Chunk* otherPointer = other.headChunkPointer->next;
      *(this->headChunkPointer) = std::move(*other.headChunkPointer);
      Chunk* currentPointer = this->headChunkPointer;
      while (otherPointer != nullptr) {
        Chunk* nextPointer = otherPointer->next;
        *currentPointer->next = std::move(*otherPointer);
        currentPointer->next->prev = currentPointer;
        currentPointer = currentPointer->next;
        otherPointer = nextPointer;
      }
      this->tailChunkPointer = currentPointer;
      this->tailChunkPointer->next = nullptr;

      other.headChunkPointer = nullptr;
      other.tailChunkPointer = nullptr;
      other.size = 0;
    }

    const T& getHead() {
      if (this->getSize() == 0)
        throw std::range_error("List is empty");
      return this->headChunkPointer->getFirst();
    }

    const T& getTail() {
      if (this->getSize() == 0)
        throw std::range_error("List is empty");
      return this->tailChunkPointer->getLast();
    }

    int getSize() const {
      return this->size;
    }

    void prependToHead(const T& value) {
      if (this->headChunkPointer->isFilled()) {
        this->headChunkPointer->prev = new Chunk(CHUNK_SIZE, nullptr, this->headChunkPointer);
        this->headChunkPointer = this->headChunkPointer->prev;
      }
      this->headChunkPointer->prepend(value);
      this->size++;
    }

    void appendToTail(const T& value) {
      if (this->tailChunkPointer->isFilled()) {
        this->tailChunkPointer->next = new Chunk(CHUNK_SIZE, this->tailChunkPointer, nullptr);
        this->tailChunkPointer = this->tailChunkPointer->next;
      }
      this->tailChunkPointer->append(value);
      this->size++;
    }

    void removeFromHead() {
      if (this->getSize() == 0)
        return;

      this->headChunkPointer->removeFirstElement();
      this->size--;

      // get rid of empty chunks
      if (this->headChunkPointer->next == nullptr) // keep chunk if it's last
        return;

      if (this->headChunkPointer->getSize() == 0) { // and remove if it's not
        this->headChunkPointer = this->headChunkPointer->next;
        this->headChunkPointer->prev->~Chunk();
        this->headChunkPointer->prev = nullptr;
      }
    }

    void removeFromTail() {
      if (this->getSize() == 0)
        return;

      this->tailChunkPointer->removeLastElement();
      this->size--;

      // get rid of empty chunks
      if (this->tailChunkPointer->prev == nullptr)  // keep chunk if it's last
        return;

      if (this->tailChunkPointer->getSize() == 0)  // and remove if it's not
      {
        this->tailChunkPointer = this->tailChunkPointer->prev;
        delete this->tailChunkPointer->next;
        this->tailChunkPointer->next = nullptr;
      }
    }

    List& operator=(const List& other) {
      if (&other == this)
        return *this;

      Chunk* nextPointer;
      for (Chunk* i = this->headChunkPointer; i != nullptr; i = nextPointer) {
        nextPointer = i->next;
        i->next = nullptr;
        i->prev = nullptr;
      }

      this->headChunkPointer = new Chunk(*other.headChunkPointer);
      Chunk* currentPointer = this->headChunkPointer;
      Chunk* otherPointer = other.headChunkPointer;
      while (otherPointer->next != nullptr) {
        currentPointer->next = new Chunk(*otherPointer->next);
        currentPointer->next->prev = currentPointer;
        currentPointer = currentPointer->next;
        otherPointer = otherPointer->next;
      }
      this->tailChunkPointer = currentPointer;
      this->tailChunkPointer->next = nullptr;
      this->size = other.size;

      return *this;
    }

    List& operator=(List&& other) noexcept {
      if (this != &other) {
        Chunk* otherPointer = other.headChunkPointer->next;
        *(this->headChunkPointer) = std::move(*other.headChunkPointer);
        Chunk* currentPointer = this->headChunkPointer;
        while (otherPointer != nullptr) {
          Chunk* nextPointer = otherPointer->next;
          *currentPointer->next = std::move(*otherPointer);
          currentPointer->next->prev = currentPointer;
          currentPointer = currentPointer->next;
          otherPointer = nextPointer;
        }
        this->tailChunkPointer = currentPointer;
        this->tailChunkPointer->next = nullptr;
        this->size = other.size;

        other.headChunkPointer = nullptr;
        other.tailChunkPointer = nullptr;
        other.size = 0;
      }

      return *this;
    }

    List<T>::Iterator iterator() {
      return Iterator(*this);
    }
};