#include <utility>
#include <iostream>

namespace stdvector {

template <typename Ret, typename... Args>
class Function {
  public:
    typedef Ret  (*to_call_pointer)(void*, Args&&...);
    typedef void (*to_copy_conctruct_pointer)(const void*, void*);
    typedef void (*to_destruct_pointer)(void*);

    Function() : object_(nullptr), object_size_(0) {}

    template <typename Functor>
    Function(const Functor& func) {
        call_func     = reinterpret_cast<to_call_pointer>(call<Functor>);
        copy_func     = reinterpret_cast<to_copy_conctruct_pointer>(copy_conctruct<Functor>);
        destruct_func = reinterpret_cast<to_destruct_pointer>(destruct<Functor>);

        object_      = operator new(sizeof(Functor));
        object_size_ = sizeof(Functor);
        
        copy_func(&func, object_);

        std::cout << "Contr\n";
    }

    Function(const Function& rhs) : object_(nullptr), call_func(rhs.call_func), copy_func(rhs.copy_func), destruct_func(rhs.destruct_func) {
        deleteObject();
        object_size_ = rhs.object_size_;
        object_      = operator new(object_size_);

        copy_func(rhs.object_, object_);
    }

    ~Function() {
        deleteObject();
    }

    Ret operator()(Args&&... args) {
        return call_func(object_, std::forward<Args>(args)...);
    }

    Function& operator=(const Function& rhs) {
        call_func     = rhs.call_func;
        copy_func     = rhs.copy_func;
        destruct_func = rhs.destruct_func;

        deleteObject();
        object_size_ = rhs.object_size_;
        object_      = operator new(object_size_);

        copy_func(rhs.object_, object_);

        return *this;
    }

  private:
    void* object_;       //Here actual Functor stored, need to cast to actual Functor to access to operator()
    size_t object_size_; //why we even need it?
    to_call_pointer call_func;
    to_copy_conctruct_pointer copy_func;
    to_destruct_pointer destruct_func;


    //to make a call
    template <typename Functor>
    static Ret call(Functor* func, Args&&... args) {
        return (*func)(std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void copy_conctruct(Functor* src, void* dst){
        new (dst) Functor(*src);
    };

    template <typename Functor>
    static void destruct(Functor* func) {
        func->~Functor(); 
    }

    void deleteObject() {
        if (object_ != nullptr) {
            destruct_func(object_);
            operator delete(object_);
        }
    }

};

} //namespace stdvector