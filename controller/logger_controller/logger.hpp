
#include "../derived_controller.hpp"


template<typename T>//,class Derived
class ILog{//: public ControlDerived<Derived>{
    public:
    virtual ~ILog()=default;
    virtual T get()=0;
};

// class ILogger{
// private:
//     static 

// };