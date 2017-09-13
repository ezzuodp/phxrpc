#include <boost/context/all.hpp>
#include <functional>

namespace phxrpc {
typedef std::function<void(void *)> uthread_functionx;

template <typename Alloc> class boost_uthread {
public:
  ~boost_uthread();

  boost_uthread();

  void run(uthread_functionx callback, void *args);

  void resume();

  void yield();

private:
  static void uthreadEntry(boost::context::detail::transfer_t t);

  boost::context::detail::fcontext_t fctx_;
  boost::context::detail::fcontext_t from_fctx_;

  uthread_functionx callback_;
  void *args_;

  Alloc alloc_;
  boost::context::stack_context stack_;
};

template <typename Alloc> boost_uthread<Alloc>::boost_uthread() : alloc_() {
  this->stack_ = this->alloc_.allocate();
}

template <typename Alloc> boost_uthread<Alloc>::~boost_uthread() {
  this->alloc_.deallocate(this->stack_);
}

template <typename Alloc>
void boost_uthread<Alloc>::run(uthread_functionx callback, void *args) {
  this->callback_ = (callback);
  this->args_ = (args);
  boost::context::detail::fcontext_t fctx =
      boost::context::detail::make_fcontext(
          this->stack_.sp, this->stack_.size,
          &boost_uthread<Alloc>::uthreadEntry);
  boost::context::detail::transfer_t t = boost::context::detail::jump_fcontext(
      fctx, reinterpret_cast<void *>(this));
  this->fctx_ = t.fctx;
}

template <typename Alloc> void boost_uthread<Alloc>::resume() {
  boost::context::detail::transfer_t t =
      boost::context::detail::jump_fcontext(this->fctx_, nullptr);
  this->fctx_ = t.fctx;
}

template <typename Alloc> void boost_uthread<Alloc>::yield() {
  boost::context::detail::transfer_t t =
      boost::context::detail::jump_fcontext(this->from_fctx_, nullptr);
  this->from_fctx_ = t.fctx;
}

template <typename Alloc>
void boost_uthread<Alloc>::uthreadEntry(boost::context::detail::transfer_t t) {
  boost_uthread<Alloc> *ptr = static_cast<boost_uthread<Alloc> *>(t.data);
  boost::context::detail::transfer_t t_ =
      boost::context::detail::jump_fcontext(t.fctx, nullptr);
  ptr->from_fctx_ = t_.fctx;
  ptr->callback_(ptr->args_);
}

typedef boost_uthread<boost::context::protected_fixedsize_stack> pfx_ucthread;
}
