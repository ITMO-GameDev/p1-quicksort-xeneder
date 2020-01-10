template<typename T, typename Compare>
void mySort(T first, T last, int size, Compare comp) {
  using Q = typename std::remove_pointer<T>::type;

  T i = first;
  T j = last;
  const T pivot = new Q[11];

  if ((j - i) >= 2) {
    memcpy(pivot, (i + (j - i) / 2), size);
  } else {
    memcpy(pivot, (i + rand() % (j - i + 1)), size);
  }

  do {
    while (comp(*i, *pivot) && i < j) {
      i++;
    }
    while (comp(*pivot, *j) && i < j) {
      j--;
    }
    if (i < j) {
      std::swap(*i, *j);
      i++;
    }
  } while (i < j);

  if (i - first > 1) {
    mySort(first, i - 1, size, comp);
  }

  if (last - i > 1) {
    mySort(i, last, size, comp);
  }
}
