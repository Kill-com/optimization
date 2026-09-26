#pragma once

#include "includes.hpp"

template<typename U>
class GOLD_SECH_:public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T=scalar_of_t<U>;
    using EPSContainer<T>::EPS;
    using TAUContainer<T>::TAU;
    USING_ALL(T)
public:
    static T f(type_var_foo target_f, T a, T b){
        REPACK(target_f,T,
        T x1 = a + (1-TAU)*(b-a);  
        T x2 = a + TAU*(b-a);

        WHILE (b-a,[&](int i){return b-a>EPS;},{
            x1 = a + (1-TAU)*(b-a);  
            x2 = a + TAU*(b-a);
            if (get_result_foo(target_f_, x1) < get_result_foo(target_f_, x2)){
                b = x2;
                x2 = x1;
                x1 = a + (1-TAU)*(x2-a);            
            }
            else{
                a = x1;
                x1 = x2;
                x2 = a + TAU*(b-a);  
            }
            LOG_METHOD((a+ b)/2)
        } );
        return(a+ b)/2;
    )
    }
    REGISTER_FUNCTION
};
