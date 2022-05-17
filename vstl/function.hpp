#include <utility>
#include <cstddef>

namespace vstl {

template <typename Ret, typename... Args>
class Function {
  public:
    using call_f_pointer           = Ret  (*)(void*, Args&&...);
    using copy_conctruct_f_pointer = void (*)(const void*, void*);
    using destruct_f_pointer       = void (*)(void*);

    Function() : object_(nullptr), object_size_(0) {}

    template <typename Functor>
    Function(const Functor& func) {
        call_func     = reinterpret_cast<call_f_pointer>(Call<Functor>);
        copy_func     = reinterpret_cast<copy_conctruct_f_pointer>(CopyConctruct<Functor>);
        destruct_func = reinterpret_cast<destruct_f_pointer>(Destruct<Functor>);

        object_      = operator new(sizeof(Functor));
        object_size_ = sizeof(Functor);
        
        copy_func(&func, object_);
    }

    Function(const Function& rhs) : object_(nullptr), call_func(rhs.call_func), copy_func(rhs.copy_func), destruct_func(rhs.destruct_func) {
        DeleteObject();
        object_size_ = rhs.object_size_;
        object_      = operator new(object_size_);

        copy_func(rhs.object_, object_);
    }

    ~Function() {
        DeleteObject();
    }

    Ret operator()(Args&&... args) {
        return call_func(object_, std::forward<Args>(args)...);
    }

    Function& operator=(const Function& rhs) {
        call_func     = rhs.call_func;
        copy_func     = rhs.copy_func;
        destruct_func = rhs.destruct_func;

        DeleteObject();
        object_size_ = rhs.object_size_;
        object_      = operator new(object_size_);

        copy_func(rhs.object_, object_);

        return *this;
    }

  private:
    void* object_;       //Here actual Functor stored, need to cast to actual Functor to access to operator()
    size_t object_size_; 
    
    call_f_pointer call_func;
    copy_conctruct_f_pointer copy_func;
    destruct_f_pointer destruct_func;


    //to make a call
    template <typename Functor>
    static Ret Call(Functor* func, Args&&... args) {
        return (*func)(std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void CopyConctruct(Functor* src, void* dst){
        new (dst) Functor(*src);
    };

    template <typename Functor>
    static void Destruct(Functor* func) {
        func->~Functor(); 
    }

    void DeleteObject() {
        if (object_ != nullptr) {
            destruct_func(object_);
            operator delete(object_);
        }
    }

};

} //namespace stdvector