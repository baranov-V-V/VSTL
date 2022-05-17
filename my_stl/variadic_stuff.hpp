#include <cstdio>
#include <iostream>
#include <iterator>

void Print(const char* format) {
    std::cout << format;
}

template <typename T, typename... Args>
void Print(const char* format, const T& arg, const Args&... args) {
    char* end_p = (char*)format;
    char* start_p = end_p;

    for (; *end_p != '\0'; ++end_p) {
        if (*end_p == '%') {
            if (*(end_p + 1) == '%') {
                fwrite(format, 1, end_p - start_p, stdout);
                ++end_p;
                start_p = end_p;
            } else {
                fwrite(start_p, 1, end_p - start_p, stdout);
                std::cout << arg;
                return Print(end_p + 2, args...);
            }
        }
    }

    return Print(end_p);
}

template <typename T, size_t N>
class MathVector {
  public:
    MathVector(std::initializer_list<T> list) {
        int i = 0;
        for (const auto& elem: list) {
            data_[i] = elem;
            ++i;
        }
    }
    MathVector() {};
    ~MathVector() {};

    operator MathVector<T, N - 1>() const {
        MathVector<T, N - 1> tmp;
        for (int i = 0; i < N - 1; ++i) {
            tmp[i] = data_[i];
        }
        return tmp;
    }

    T& operator [](int idx) {
        return data_[idx];
    }
    const T& operator [](int idx) const {
        return data_[idx];
    }
  private:
    T data_[N];
};

template<typename T>
T dot(const MathVector<T, 1>& lhs, const MathVector<T, 1>& rhs) {
    return lhs[0] * rhs[0];
}

template<typename T, size_t N>
T dot(const MathVector<T, N>& lhs, const MathVector<T, N>& rhs) {
    return lhs[N - 1] * rhs[N - 1] + dot(static_cast<MathVector<T, N - 1>>(lhs), static_cast<MathVector<T, N - 1>>(rhs));
}

template <size_t N, size_t M, typename T = int>
class Matrix {
  public:
    Matrix(std::initializer_list<T> list) {
        int i = 0;
        int j = 0;
        auto it = list.begin();
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                data_[i][j] = *it;
                it = std::next(it);
            }
        }
    }

    T& operator()(size_t x, size_t y) {
        return data_[x][y];
    }

  private:
    T data_[N][M];
};

