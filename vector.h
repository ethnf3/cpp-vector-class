#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <initializer_list>
#include <stdexcept>

template <typename T>

class Vector {
public:
  class ConstIterator;
  class Iterator;
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Vector::Iterator;
  using const_iterator = Vector::ConstIterator;

private:
  // Instanzvariablen 
  size_type sz;         //Enthält die Anzahl der im Vector gespeicherten Elemente.
  size_type max_sz;     //Enthält die maximal mögliche Anzahl von Elementen
  pointer values;       //Zeigt auf ein (dynamisch alloziertes) Array, in dem der Vector die Elemente speichert

  //für RESERVE, SHRINK_TO_FIT
  void allocate(size_type new_max_sz) {
    if (new_max_sz < sz) return;
    pointer help = (new_max_sz ? new value_type[new_max_sz] : nullptr);
    for (size_type i{0}; i<sz; ++i) help[i] = values[i];
    delete[] values;
    values = help;
    max_sz = new_max_sz;
  }


public:
  // Methoden
  //1.2
  //default Konstruktor
  Vector(): sz{0}, max_sz{4}, values{new value_type[max_sz]{}}{}

  //copy konstruktor
  Vector(const Vector& cpy) : sz{cpy.sz}, max_sz{cpy.max_sz}, values{max_sz ? new value_type[max_sz]{} : nullptr} {
    for (size_type i {0}; i < sz; ++i) values[i] = cpy.values[i];
  }

  //konstruktor size_t n
  Vector(size_t n) : sz{0}, max_sz{n}, values{n ? new value_type[n] : nullptr} {
  }

  //konstruktor mit spezifiriertem Inhalt 
  Vector(std::initializer_list<value_type> li) : sz{li.size()}, max_sz{li.size()}, values{li.size() ? new value_type[li.size()] : nullptr} {
    size_type i = 0;
    for (const auto& elem : li) values[i++] = elem;
  }
  
  //destruktor
  ~Vector() {
    delete[] values;
  }


  //FUNKTIONEN 

  //Kopierzuweisungsoperator
  Vector& operator=(Vector cpy) {
    std::swap(sz, cpy.sz);
    std::swap(max_sz, cpy.max_sz);
    std::swap(values, cpy.values);
    return *this;
  }

  //Retourniert die Anzahl der aktuell gespeicherten Elemente
  size_type size() const{
    return sz;
  }

  //Retourniert true, wenn der Vector leer ist, false sonst
  bool empty() const{
    return sz == 0;
  }

  //Löscht alle Elemente aus dem Vector
  void clear() {
    sz = 0;
  }

  //Wenn die Kapazität des Vectors nicht schon zumindest n ist, wird dieser entsprechend vergrößert
  void reserve(size_type n) {
    if (n<=max_sz) return;
    allocate(n);
    //max_sz = n FALSCH!!
  }

  //Kapazität des Vectors wird auf das erforderliche Mindestmaß (Anzahl der aktuell gespeicherten Elemente) reduziert
  void shrink_to_fit(){
    if (max_sz > sz) allocate(sz);
  }

  //Eine Kopie von x wird zum Vectorals letztes Element (am Ende) hinzugefügt
  void push_back(value_type x) {
    if (sz >= max_sz) allocate(max_sz*2+1); //schneller *2 (+1 wegen "0")
    values[sz++] = x;
  }

  //Entfernt das letzte Element aus dem Vector. Wirft eine Exception vom Typ std::runtime_error, falls der Vector leer war.
  void pop_back() {
    if (sz == 0) throw std::runtime_error("vector leer");
    --sz;
  }

  //Retourniert eine Referenz auf das Element an der spezifizierten Position (index). Ist der Index nicht im erlaubten Bereich, muss eine Exception vom Typ std::runtime_error geworfen werden.
  reference operator[](size_type index) {
    if (index >= sz) throw std::runtime_error("index out of range");
    return values[index];
  }
  //(Version für const Vectoren)
  const_reference operator[](size_type index) const {
    if (index >= sz) throw std::runtime_error("index out of range");
    return values[index];
  }

  //Retourniert die aktuelle Kapazität des Vectors.
  size_type capacity() const {
    return max_sz;
  }


  //OUTPUT FORMAT
  friend std::ostream& operator<<(std::ostream& o, const Vector& v) {
    o << "[";
    bool first{true};
    for (size_type i{0}; i<v.size(); ++i) {
        if (first) first = false;
        else o << ", ";
        o << v[i];
    }
    o << "]";
    return o;
  }

  //Erweiterung der Klasse Vektor
  
  //Retourniert einen Iterator, der auf das erste Element im Vector verweist, bzw. gleich dem end-Iterator ist, falls der Vector leer ist.
  iterator begin() {
    return iterator(values);
  }
  
  //Retourniert einen Iterator, der auf das virtuelle Element hinter dem letzten im Vector enthaltenen Element verweist.
  iterator end() {
    return iterator(values + sz);
  }

  const_iterator begin() const {
    return const_iterator(values);
  }

  const_iterator end() const {
    return const_iterator(values+sz);
  }









  class Iterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::reference;
      using pointer = Vector::pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      // Instanzvariablen
      pointer ptr;
      //      pointer e; //end pointer
      //      Vector* v_ptr;

    public:
      // Methoden

      //Konstruktoren
      Iterator() : ptr{nullptr} {}
      Iterator(pointer ptr) : ptr{ptr} {}


      //FUNKTIONEN

      // Retourniert eine Referenz auf den Wert, auf den der Iterator verweist (auf den die Instanzvariable ptr zeigt).
      reference operator*() const {
        return *ptr;
      }

      //Retourniert einen Pointer auf den vom Iterator referenzierten Wert.
      pointer operator->() const {
        return ptr;
      }

      //Vergleicht, ob die beiden Pointer gleich sind. (Eine globale Funktion könnte eine bessere Wahl sein.)
      friend bool operator==(const Iterator& lop, const Iterator& rop) {
        return (lop.ptr==rop.ptr);
      }

      //Vergleicht, ob die beiden Pointer unterschiedlich sind. (Eine globale Funktion könnte eine bessere Wahl sein.)
      friend bool operator!=(const Iterator& lop, const Iterator& rop) {
        return !(lop==rop);
      }

      //(Prefix) ++it : Iterator wird auf das nächste Element im Vector weitergeschaltet. Die Methode retourniert eine Referenz auf den veränderten Iterator.
      Iterator& operator++(){
        ++ptr;
        return *this;
      } 

      //(Postfix) Iterator wird auf das nächste Element in Vector weitergeschaltet. Eine Kopie des ursprünglichen Iterators wird retourniert
      Iterator operator++(int) {    //То есть этот int — просто маркер, чтобы отличить функции.
        Iterator temp{*this};
        ++ptr;
        return temp;
      }

      //(Typumwandlung) Erlaubt die Konversion von Iterator zu ConstIterator.
      operator ConstIterator() const {
        return ConstIterator(ptr);
      }

  };


//CONSTITERATOR


  class ConstIterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::const_reference;
      using pointer = Vector::const_pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      // Instanzvariablen
      const_pointer ptr;

    public:
      // Methoden
      ConstIterator() : ptr{nullptr} {}
      ConstIterator(const_pointer ptr) : ptr{ptr} {}


      //FUNKTIONEN

      //Retourniert eine Referenz auf den Wert, auf den der Iterator verweist (auf den die Instanzvariable ptr zeigt).
      reference operator*() const {
        return *ptr;
      }

      //Retourniert einen Pointer auf den vom ConstIterator referenzierten Wert.
      pointer operator->() const {
        return ptr;
      }

      //Vergleicht, ob die beiden Pointer gleich sind. (Eine globale Funktion könnte eine bessere Wahl sein.)
      friend bool operator==(const ConstIterator& lop, const ConstIterator& rop) {
        return (lop.ptr==rop.ptr);
      }

      //Vergleicht, ob die beiden Pointer unterschiedlich sind. (Eine glrobale Funktion könnte eine bessere Wahl sein.)
      friend bool operator!=(const ConstIterator& lop, const ConstIterator& rop){
        return !(lop==rop);
      }

      //(Prefix) ConstIterator wird auf das nächste Element m Vector weitergeschaltet. Die Methode retourniert eine Referenz auf den veränderten ConstIterator.
      ConstIterator& operator++() {
        ++ptr;
        return *this;
      }

      //(Postfix) ConstIterator wird auf das nächste Element in Vector weitergeschaltet. Eine Kopie des ursprünglichen ConstIterators wird retourniert.
      ConstIterator operator++(int) {
        ConstIterator temp{*this};
        ++ptr;
        return temp;
      }

    friend difference_type operator-(const ConstIterator& lop, const ConstIterator& rop) {
      return lop.ptr - rop.ptr;
    }
  };



  
  iterator insert(const_iterator pos, const_reference val) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) > sz)
      throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    if (sz >= max_sz)
      reserve(max_sz * 2 + 1); // Achtung Sonderfall, wenn keine Mindestgroesze definiert ist
    for (auto i{sz}; i-- > current;)
      values[i + 1] = values[i];
    values[current] = val;
    ++sz;
    return iterator{values + current};
  }

  iterator erase(const_iterator pos) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= sz)
      throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i < sz - 1; ++i)
      values[i] = values[i + 1];
    --sz;
    return iterator{values + current};
  }
};
#endif