
template<class Derived>
class ControlDerived{
protected:
    Derived* derived_ptr;
public:
    ControlDerived() : derived_ptr(static_cast<Derived*>(this)) {};
};