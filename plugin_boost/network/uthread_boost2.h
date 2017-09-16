#include <boost/context/all.hpp>
#include <functional>

namespace phxrpc {
/**
 * 使用 boost 新版本实现的
 */
template <typename Alloc> 
class boost_uthread {
public:
  typedef std::function<void(void *)> uthread_functionx;
  
  boost_uthread() : alloc_() { this->stack_ = this->alloc_.allocate(); }

  ~boost_uthread() { this->alloc_.deallocate(this->stack_); }

  void run(uthread_functionx callback, void *args) {
    this->callback_ = (callback);
    this->args_ = (args);
    boost::context::detail::fcontext_t fctx = boost::context::detail::make_fcontext( this->stack_.sp, this->stack_.size, &boost_uthread<Alloc>::uthreadEntry);
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(fctx, reinterpret_cast<void *>(this));
    this->fctx_ = t.fctx;
  }

  void resume() {
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->fctx_, nullptr);
    this->fctx_ = t.fctx;
  }

  void yield() {
    boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(this->from_fctx_, nullptr);
    this->from_fctx_ = t.fctx;
  }

private:
  static void uthreadEntry(boost::context::detail::transfer_t t) {
    boost_uthread<Alloc> *ptr = static_cast<boost_uthread<Alloc> *>(t.data);
    boost::context::detail::transfer_t t_ = boost::context::detail::jump_fcontext(t.fctx, nullptr);
    ptr->from_fctx_ = t_.fctx;
    ptr->callback_(ptr->args_);
  }

  boost::context::detail::fcontext_t fctx_ = nullptr;
  boost::context::detail::fcontext_t from_fctx_ = nullptr ;

  uthread_functionx callback_  = nullptr;;
  void *args_  = nullptr;;

  Alloc alloc_ ;
  boost::context::stack_context stack_;
};

typedef boost_uthread<boost::context::protected_fixedsize_stack> default_boost_uthread;
typedef boost_uthread<boost::context::fixedsize_stack>           fixed_boost_uthread;

}
