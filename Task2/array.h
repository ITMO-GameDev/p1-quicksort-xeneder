template<typename T>
class Array {
public:
    T* pointer;
    int size;
    int capacity;

    class Iterator {
    private:
        Array<T>& array;
        int position;
    public:
        explicit Iterator(Array<T>& array): array(array), position(0) {}

        const T& get() const {
          return this->array[this->position];
        }

        void set(const T& value) {
          this->array[this->position] = value;
        }

        void insert(const T& value) {
          this->array.insert(this->position, value);
        }

        void remove() {
          this->array.remove(this->position);
        }

        void next() {
          if (this->position > this->array.getSize())
            throw std::range_error("Position out of bounds");
          this->position++;
        }

        void prev() {
          if (this->position <= 0)
            throw std::range_error("Position out of bounds");
          this->position--;
        }

        void toIndex(int index) {
          if ((index >= this->array.getSize()) || (index < -1))
            throw std::range_error("Position out of bounds");
          this->position = index;
        }

        bool hasNext() const {
          return this->position < this->array.getSize();
        }

        bool hasPrev() const
        {
          return this->position > 0;
        }
    };

    Array():pointer(nullptr), capacity(8), size(0) {
      this->pointer = new T[capacity];
    }

    Array(int capacity): pointer(nullptr), capacity(capacity), size(0) {
      if (capacity < 0)
        throw std::runtime_error("Invalid capacity");
      this->pointer = new T[capacity];
    }

    Array(const Array& other) {
      this->pointer = new T[other.capacity];
      std::memcpy(this->pointer, other.pointer, other.size * sizeof(T));

      capacity = other.capacity;
      size = other.size;
    }

    Array(Array&& other) {
      this->pointer = new T[other.capacity];
      std::memmove(this->pointer, other.pointer, other.size * sizeof(T));

      capacity = other.capacity;
      size = other.size;

      other.pointer = nullptr;
      other.size = 0;
      other.capacity = 0;
    }

    ~Array() {
      delete[] this->pointer;
    }

    int getSize() {
      return this->size;
    }

    void expandStorage() {
      T* newPointer;
      if (this->capacity == 0)
        this->capacity++;
      this->capacity = this->capacity * 2;

      newPointer = new T[this->capacity];

      std::memcpy(newPointer, this->pointer, this->size * sizeof(T));

      this->pointer = newPointer;
    }

    void insert(const T& value) {
      this->insert(this->size, value);
    }

    void insert(int index, const T& value) {
      if (index > this->size || index < 0)
        throw std::range_error("Index out of bounds");

      if (this->capacity == this->size)
        this->expandStorage();

      this->pointer[index] = value;

      this->size++;
    }

    void remove(int index) {
      if ((index >= this->size) || (index < 0))
        throw std::range_error("Index out of bounds");

      std::memmove(this->pointer + index, this->pointer + index + 1, (this->size - index - 1) * sizeof(T));

      this->size--;
    }

    T& operator[](int index) {
      if ((index > this->size - 1) || (index < 0))
        throw std::range_error("Index out of bounds");
      return this->pointer[index];
    }

    const T& operator[](int index) const {
      if ((index > this->size() - 1) || (index < -1))
        throw std::range_error("Index out of bounds");
      return std::move(this->pointer[index]);
    }

    Array& operator =(const Array& other) {
      if (&other == this)
        return *this;

      delete[] this->pointer;

      this->pointer = new T[other.capacity];
      std::memcpy(this->pointer, other.pointer, this->size * sizeof(T));

      capacity = other.capacity;
      size = other.size;

      return *this;
    }

    Array& operator =(Array&& other) noexcept {
      if (this != &other) {
        delete[] this->pointer;
        this->pointer = new T[other.capacity];

        std::memcpy(this->pointer, other.pointer, other.size * sizeof(T));

        capacity = other.capacity;
        size = other.size;

        other.pointer = nullptr;
        other.size = 0;
        other.capacity = 0;
      }

      return *this;
    }

    Array<T>::Iterator iterator() {
      return Iterator(*this);
    }
};