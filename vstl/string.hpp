#include <exception>
#include <iostream>
#include <utility>
#include <cstring>

namespace stdvector {

class String {
  public:
    String(const char* str) {
        size_t len = std::strlen(str);
        if (len > max_len - 1) {
            is_small_ = false;
            large_.capacity_ = len + 1;
            size_ = len + 1;
            data = new char[large_.capacity_];
            std::memcpy(data, str, size_);
        } else {
            is_small_ = true;
            size_ = len + 1;
            data = &small_[0];
            std::memcpy(data, str, size_);
        }
    }

    String() : is_small_(true), size_(1), data(&small_[0]) {
        data[0] = '\0';
    }

    String(const String& lval) {
        if (lval.size() < max_len) {
            is_small_ = true;
            size_ = lval.size_;
            data = &small_[0];
            std::memcpy(data, lval.c_str(), size_);
        } else {
            is_small_ = false;
            large_.capacity_ = lval.size_;
            size_ = lval.size_;
            data = new char[large_.capacity_];
            std::memcpy(data, lval.c_str(), size_); 
        }
    }

    String(String&& rval) {
        if (rval.size() < max_len) {
            is_small_ = true;
            size_ = rval.size_;
            data = &small_[0];
            std::memcpy(data, rval.c_str(), size_);
        } else {
            is_small_ = false;
            large_.capacity_ = rval.size_;
            size_ = large_.capacity_;
            data = rval.data;
            rval.data = nullptr;
        }
    }
    
    ~String() {
        if (!is_small_ && data != nullptr) {
            delete[] data;
        }
    }

    String& operator=(const String& lval) {
        if (lval.size() < max_len) {
            is_small_ = true;
            size_ = lval.size_;
            data = &small_[0];
            std::memcpy(data, lval.c_str(), size_);
        } else {
            is_small_ = false;
            large_.capacity_ = lval.size_;
            size_ = lval.size_;
            data = new char[large_.capacity_];
            std::memcpy(data, lval.c_str(), size_); 
        }
        return *this;
    };


    class Iterator {
      public:
        using value_type = char;
        using difference_type = std::ptrdiff_t;
        using pointer = char*;
        using reference  = char&;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() : s_(nullptr), pos_(0) {}   
        Iterator(String* s, size_t start_pos): s_(s), pos_(start_pos) {}
        
        reference operator*() {
            return (*s_)[pos_];
        }
        const reference operator*() const {
            return (*s_)[pos_];
        }
        pointer operator->() {
            return &((*s_)[pos_]);
        }
        const pointer operator->() const {
            return &((*s_)[pos_]);
        }
        reference operator[](int idx) {
            return (*s_)[pos_ + idx];
        }
        const reference operator[](int idx) const {
            return (*s_)[pos_ + idx];
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
        String* s_;
        size_t pos_;
    };


    size_t size() const {
        return size_ - 1;
    }
    size_t capacity() const {
        if (is_small_) {
            return max_len;
        }
        return large_.capacity_ - 1;
    }

    char& operator [](int idx) {
        return data[idx];
    }
    const char& operator [](int idx) const {
        return data[idx];
    }

    void push_back(const char ch) {
        size_t pos = prepare_push();
        data[pos] = ch;
        data[pos + 1] = '\0';
        ++size_;
    }

    void pop_back() {
       data[--size_ - 1] = '\0'; 
    }

    char* c_str() const {
        return data;
    }

    String& operator+=(const String& rhs) {
        char* ptr = reserve(rhs.size_);
        std::memcpy(ptr, rhs.c_str(), rhs.size_);
        size_ += rhs.size();
        return *this;
    }
    String& operator+=(const char* rhs) {
        size_t rhs_size = std::strlen(rhs);
        char* ptr = reserve(rhs_size + 1);
        std::memcpy(ptr, rhs, rhs_size + 1);
        size_ += rhs_size;
        return *this;
    }
    String& operator+=(const char rhs) {
        push_back(rhs);
        return *this;
    }

  private:
    size_t prepare_push() {
        if (is_small_) {
            if (size_ == max_len) {
                resize_from_small(max_len * 2);
            }  
        } else {
            if (size_ == large_.capacity_) {
                resize(large_.capacity_ * 2);
            }   
        }
        return size_ - 1;
    }

    void resize(size_t count) {
        size_t old_capacity = large_.capacity_;
        large_.capacity_ = count;
        char* new_data = new char[large_.capacity_];
        std::memcpy(new_data, data, old_capacity);
        delete[] data;
        data = new_data;
    }

    void resize_from_small(size_t count) {
        char* new_data = new char[count];
        std::memcpy(new_data, data, max_len);
        large_.capacity_ = count;
        data = new_data;
        is_small_ = false;
    }  

    char* reserve(size_t count) {
        std::cout << "reserving " << count << " bytes\n";
        if (is_small_) {
            if (size_ + count >= max_len) {
                resize_from_small(max_len + count);
            }
        } else {
            if ((size_ + count >= large_.capacity_) && (size_ + count < large_.capacity_ * 2)) {
                resize(large_.capacity_ * 2);
            } else {
                resize(large_.capacity_ + count);
            }
        }
        return data + size_ - 1;
    }

    static const size_t max_len = 16;

    char* data;                 //always valid
    size_t size_;               //size with \0
    union {
        struct {
            size_t capacity_;   //capa with \0
            char padding[8];
        } large_;
        char small_[sizeof(large_)];
    };
    bool is_small_;
};

std::ostream& operator<<(std::ostream& out, String& str) {
    out << str.c_str();
    return out;
}

String operator+(const String& lhs, const String& rhs) {
    String tmp(lhs);
    tmp += rhs;
    return tmp;
}

String operator+(const String& lhs, const char* rhs) {
    String tmp(lhs);
    tmp += rhs;
    return tmp;
}

String operator+(const char* lhs, const String& rhs) {
    String tmp(lhs);
    tmp += rhs;
    return tmp;
}

} //namespace stdvector