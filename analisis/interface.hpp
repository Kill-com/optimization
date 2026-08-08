#pragma once

#include "../controller/derived_controller.hpp"
#include "../controller/logger_controller/interface.hpp"


template<class Derived>
class GetLog:public ControlDerived<Derived>, public SubjectLog{
public:
    
};