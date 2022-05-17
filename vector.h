#include <iostream>
#include <utility>
#include <iterator>
//#include <vector>
#include <cassert>
#include <initializer_list>
#include <cstring>

namespace stdvector {

template <typename T, size_t N>
class StaticMemory {
  public:
    StaticMemory() : capacity_(N) {
      data_ = reinterpret_cast<T*>(storage_);

      for (size_t i = 0; i < N; ++i) {
        new(data_ + i) T();
      }
    }

    StaticMemory(const T& val) : capacity_(N) {
      data_ = reinterpret_cast<T*>(storage_);
      
      for (size_t i = 0; i < N; ++i) {
        new(data_ + i) T(val);
      }
    }

    T& operator [](int index) {
        return data_[index];
    }

    const T& operator [](int index) const {
        return data_[index];
    }

  private:
    uint8_t storage_[N * sizeof(T)];
    T* data_;
    size_t capacity_;
};


template <typename T>
struct DynamicMemory {
  public:
    DynamicMemory() : capacity_(1) {
        storage_ = new uint8_t[capacity_ * sizeof(T)];
        data_ = reinterpret_cast<T*>(storage_);
    }
    DynamicMemory(size_t count) : capacity_(count) {
        storage_ = new uint8_t[capacity_ * sizeof(T)];
        data_ = reinterpret_cast<T*>(storage_);
        
        for (size_t i = 0; i < count; ++i) {
            new(data_ + i) T();
        }
    }
    DynamicMemory(size_t count, const T& val) : capacity_(count) {
        storage_ = new uint8_t[capacity_ * sizeof(T)];
        data_ = reinterpret_cast<T*>(storage_);
      
        for (size_t i = 0; i < count; ++i) {
            new(data_ + i) T(val);
        }
    }
    ~DynamicMemory() {
        delete[] storage_;
    }

  protected:

    size_t capacity() const {
        return capacity_;
    }

    T& operator [](int idx) {
        return data_[idx];
    }
    const T& operator [](int idx) const {
        return data_[idx];
    }

    void insert(size_t idx, const T& val) {
        new(data_ + idx) T(val);
    }

    void insert(size_t idx, T&& val) {
        new(data_+ idx) T(std::move(val));
    }

    void storageRealloc() {
        reallocate();
    }

  private:
    uint8_t* rawData() const {
        return storage_;
    }
    
    T* data() const {
        return data_;
    }
    
    void reallocate() {
      size_t old_capacity_ = capacity_;
      capacity_ *= 2;

      uint8_t* new_storage = new uint8_t[capacity_ * sizeof(T)];
      T* new_data = reinterpret_cast<T*>(new_storage);

      for (size_t i = 0; i < old_capacity_; ++i) {
        new(new_data + i) T(std::move(data_[i]));
      }

      data_ = new_data;
      delete[] storage_;
      storage_ = new_storage;
    };

    size_t capacity_;  //in T
    uint8_t* storage_; //owner of array of T
    T* data_;          //not owner of array of T
};

template <typename T, template <typename> class Storage = DynamicMemory>
class Vector : protected Storage<T> {
  public:
    Vector() : size_(0), Storage<T>(1) {};
    Vector(size_t count) : size_(count), Storage<T>(count) {};
    Vector(size_t count, const T& val) : size_(count), Storage<T>(count, val) {};
    Vector(std::initializer_list<T> list) : size_(0), Storage<T>(1) {
        for (const auto& elem: list) {
            this->pushBack(elem);
        }
    }

    class Iterator {
      public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference  = T&;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() : v_(nullptr), pos_(0) {}   
        Iterator(Vector<T>* v, size_t start_pos): v_(v), pos_(start_pos) {}
        
        reference operator*() {
            return (*v_)[pos_];
        }
        const reference operator*() const {
            return (*v_)[pos_];
        }
        pointer operator->() {
            return &((*v_)[pos_]);
        }
        const pointer operator->() const {
            return &((*v_)[pos_]);
        }
        reference operator[](int idx) {
            return (*v_)[pos_ + idx];
        }
        const reference operator[](int idx) const {
            return (*v_)[pos_ + idx];
        }


        Iterator& operator++() {
            ++pos_;
            return *this;
        }
        Iterator& operator--() {
            --pos_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            ++pos_;
            return tmp;
        }
        Iterator operator--(int) {
            Iterator tmp(*this);
            --pos_;
            return tmp;
        }

        Iterator& operator+=(int n) {
            pos_ += n;
            return *this;
        }
        Iterator& operator-=(int n) {
            pos_ -= n;
            return *this;
        }

        Iterator operator+(int n) const {
            Iterator tmp(*this);
            return tmp += n;
        }
        Iterator operator-(int n) const {
            Iterator tmp(*this);
            return tmp -= n;
        }

        difference_type operator-(Iterator const& it) const {
            return pos_ - it.pos_;
        }

        bool operator<(Iterator const& it) const {
            return pos_ < it.pos_;
        }
        bool operator<=(Iterator const& it) const {
            return pos_ <= it.pos_;
        }
        bool operator>(Iterator const& it) const {
            return pos_ > it.pos_;
        }
        bool operator>=(Iterator const& it) const {
            return pos_ >= it.pos_;
        }
        bool operator!=(const Iterator &it) const {
            return pos_ != it.pos_;
        }
        bool operator==(const Iterator &it) const {
            return pos_ == it.pos_;
        }

      private:
        Vector<T>* v_;
        size_t pos_;
    };

    T& operator [](int idx) {
        return this->Storage<T>::operator[](idx);
    }
    const T& operator [](int idx) const {
        return this->Storage<T>::operator[](idx);
    }

    void pushBack(const T& val) {
        if (size_ + 1 == this->capacity()) {
            this->storageRealloc();
        }
        this->Storage<T>::insert(size_, val);
        ++size_;
    }
    void pushBack(T&& val) {
        if (size_ + 1 == this->capacity()) {
            this->storageRealloc();
        }
        this->Storage<T>::insert(size_, std::move(val));
        ++size_;
    }
    void popBack() {
        --size_;
    }

    Iterator begin() {
        return Iterator(this, 0);
    }
    Iterator end() {
        return Iterator(this, size_);
    }
    
    size_t size() const {
        return size_;
    }
    size_t capacity() const {
        return this->Storage<T>::capacity();
    }

    size_t capacity() {
        return this->Storage<T>::capacity();
    }

  private:
    size_t size_;
};

/*
template <typename T>
class Vector {

};
*/

/*
template <typename T, template <typename, typename> class Storage = StaticMemory>
class Vector : protected Storage<T, N> {

};
*/


template <>
class Vector<bool> {
  public:
    Vector() : size_(0), capacity_(8) {
        data_ = new uint8_t[1];
    };
    Vector(size_t count) : size_(count), capacity_(count / 8 + 1) {
        data_ = new uint8_t[capacity_];
    };
    Vector(size_t count, bool val) : size_(count), capacity_(count / 8 + 1) {
        data_ = new uint8_t[capacity_];
        for (size_t i = 0; i < count; ++i) {
            this->set(i, val);
        }
    };
    Vector(std::initializer_list<bool> list) : size_(0), capacity_(list.size() / 8 + 1) {
        data_ = new uint8_t[capacity_];
        for (auto elem: list) {
            this->pushBack(elem);
        }
    }
    ~Vector() {
        delete[] data_;
    }

    class BoolRef {
      public:
        BoolRef(Vector<bool>* v, int idx) : v_(v), idx_(idx) {};

        operator bool() const {
            return v_->get(idx_);
        };
        BoolRef& operator=(const BoolRef& ref) {
            *this = bool(ref);
            return *this;
        };
        BoolRef& operator=(bool value) {
            v_->set(idx_, value);
            return *this;
        };

      private:
        Vector<bool>* v_;
        int idx_;
    };

    class Iterator {
      public:
        using value_type = bool;
        using difference_type = std::ptrdiff_t;
        using pointer = bool*;
        using reference  = BoolRef;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() : v_(nullptr), pos_(0) {}   
        Iterator(Vector<bool>* v, size_t start_pos): v_(v), pos_(start_pos) {}
        
        reference operator*() {
            return BoolRef(v_, pos_);
        }
        const reference operator*() const {
            return BoolRef(v_, pos_);
        }
        reference operator[](int idx) {
            return BoolRef(v_, pos_ + idx);
        }
        const reference operator[](int idx) const {
            return BoolRef(v_, pos_ + idx);
        }


        Iterator& operator++() {
            ++pos_;
            return *this;
        }
        Iterator& operator--() {
            --pos_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            ++pos_;
            return tmp;
        }
        Iterator operator--(int) {
            Iterator tmp(*this);
            --pos_;
            return tmp;
        }

        Iterator& operator+=(int n) {
            pos_ += n;
            return *this;
        }
        Iterator& operator-=(int n) {
            pos_ -= n;
            return *this;
        }

        Iterator operator+(int n) const {
            Iterator tmp(*this);
            return tmp += n;
        }
        Iterator operator-(int n) const {
            Iterator tmp(*this);
            return tmp -= n;
        }

        difference_type operator-(Iterator const& it) const {
            return pos_ - it.pos_;
        }

        bool operator<(Iterator const& it) const {
            return pos_ < it.pos_;
        }
        bool operator<=(Iterator const& it) const {
            return pos_ <= it.pos_;
        }
        bool operator>(Iterator const& it) const {
            return pos_ > it.pos_;
        }
        bool operator>=(Iterator const& it) const {
            return pos_ >= it.pos_;
        }
        bool operator!=(const Iterator &it) const {
            return pos_ != it.pos_;
        }
        bool operator==(const Iterator &it) const {
            return pos_ == it.pos_;
        }

      private:
        Vector<bool>* v_;
        size_t pos_;
    };

    BoolRef operator [](int idx) {
        return BoolRef(this, idx);
    }

    void pushBack(const bool& val) {
        if (size_++ == this->capacity()) {
            this->reallocate();
        }
        this->set(size_ - 1, val);
    }
    void popBack() {
        --size_;
    }

    Iterator begin() {
        return Iterator(this, 0);
    }
    Iterator end() {
        return Iterator(this, size_);
    }
    
    size_t size() const {
        return size_;
    }
    size_t capacity() const {
        return capacity_ * 8;
    }

  private:
    void set(int idx, bool val) {
        if (val) {
            data_[idx / 8]|= 1 << (idx % 8); 
        } else {
            data_[idx / 8] &= 255 - (1 << (idx % 8));
        }
    }
    bool get(int idx) {
        uint8_t chunk = data_[idx / 8];
        return (chunk >> (idx % 8)) & 1;
    }

    void reallocate() {
        size_t old_capacity_ = capacity_;
        capacity_ *= 2;

        uint8_t* new_data = new uint8_t[capacity_];
        memcpy(new_data, data_, old_capacity_);
        
        delete[] data_;
        data_ = new_data;
    }

    uint8_t* data_;
    
    size_t capacity_; //in bytes
    size_t size_;     //in bits
};

//store memory in chunks of 16kb each
template <typename T>
struct ChunkedMemory {
  public:
    ChunkedMemory() {}
    ChunkedMemory(size_t count) : data_vector_(count / obj_per_chunk + 1) {
        //std::cout << "s_capa_input: " << count / obj_per_chunk + 1 << "\n";
        //std::cout << "s_size: " << data_vector_.size() << "\n";
        //std::cout << "s_capacity: " << data_vector_.capacity() << "\n";
    }
    ChunkedMemory(size_t count, const T& val) : data_vector_(count / obj_per_chunk + 1) {
        for (size_t i = 0; i < count; ++i) {
            this->insert(i, val);
        }
    }

  protected:
    size_t capacity() {
        if (data_vector_.size() == 0) {
            return 0;
        } else {
            return data_vector_.size() * data_vector_[0].size();
        }
    }

    T& operator [](int idx) {
        return data_vector_[idx / obj_per_chunk].get(idx % obj_per_chunk);
    }
    const T& operator [](int idx) const {
        return data_vector_[idx / obj_per_chunk].get(idx % obj_per_chunk);
    }

    void insert(size_t idx, const T& val) {
        //std::cout << "size: " << data_vector_.size() << "\n";
        //std::cout << "capacity: " << data_vector_.capacity() << "\n";
        while (data_vector_.size() <= (idx / obj_per_chunk)) {
            data_vector_.pushBack(Node());
        }
        //std::cout << "size: " << data_vector_.size() << "\n";
        //std::cout << "1: " << idx / obj_per_chunk << "\n";
        //std::cout << "2: " << idx % obj_per_chunk << "\n";
        data_vector_[idx / obj_per_chunk].set(idx % obj_per_chunk, val);
    }

    void insert(size_t idx, T&& val) {
        while (data_vector_.size() <= (idx / obj_per_chunk)) {
            data_vector_.pushBack(Node());
        }
        data_vector_[idx / obj_per_chunk].set(idx % obj_per_chunk, val);
    }

    void storageRealloc() {
    }

  private:
    const static size_t chunk_size = 16 * 1024; //in bytes
    size_t obj_per_chunk = chunk_size / sizeof(T);

    class Node {
      public:
        Node() : size_(chunk_size / sizeof(T)) {
            //std::cout << "Node construct default" << "\n";
            storage_ = new uint8_t[chunk_size];
            data_ = reinterpret_cast<T*>(storage_);
        }
        Node(const T& val) : size_(chunk_size / sizeof(T)) {
            //std::cout << "Node construct not default" << "\n";
            storage_ = new uint8_t[chunk_size];
            data_ = reinterpret_cast<T*>(storage_);

            for (size_t i = 0; i < size_; ++i) {
                new(data_ + i) T(val);
            }
        }
        ~Node() {
            delete[] storage_;
        }

        size_t size() {
            return size_;
        }

        T& get(size_t idx) {
            return data_[idx];
        }

        void set(size_t idx, const T& val) {
            new(data_ + idx) T(val);
        }

        void set(size_t idx, T&& val) {
            new(data_+ idx) T(std::move(val));
        }

      private:
        size_t size_;
        uint8_t* storage_; //owner of array of T
        T* data_; //not owner of array of T 
    };

    stdvector::Vector<Node> data_vector_;
};

} //namespace stdvector