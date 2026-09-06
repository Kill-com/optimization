
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>



class point
{
public:

    float f_value;
    float d_value;

    point(float x, float y) : f_value(x), d_value(y) {};
    point(float x): f_value(x), d_value(0.0) {};
    point(): f_value(0.0), d_value(0.0) {};

    point create_dual_point(float x){
        return point(x, 1.0);
    }

    point operator+(const point& a) const;
    point operator-(const point& a) const;
    point operator*(const point& a) const;
    point operator/(const point& a) const;

    point operator+(float a) const;
    point operator-(float a) const;
    point operator*(float a) const;
    point operator/(float a) const;

};

point point::operator+(const point& a) const{
    point temp;
    temp.f_value = f_value + a.f_value;
    temp.d_value = d_value + a.d_value;
    return temp;
};

point point::operator-(const point& a) const{
    point temp;
    temp.f_value = f_value - a.f_value;
    temp.d_value = d_value - a.d_value;
    return temp;
};

point point::operator*(const point& a) const{
    point temp;
    temp.f_value = f_value * a.f_value;
    temp.d_value = d_value * a.f_value + f_value * a.d_value;
    return temp;
};

point point::operator/(const point& a) const{
    point temp;
    temp.f_value = f_value / a.f_value;
    temp.d_value = (d_value * a.f_value - f_value * a.d_value)/pow(a.f_value, 2);
    return temp;
}
point point::operator+(float a) const{
    point temp;
    temp.f_value = f_value + a;
    temp.d_value = d_value;
    return temp;
};

point point::operator-(float a) const{
    point temp;
    temp.f_value = f_value - a;
    temp.d_value = d_value;
    return temp;
};

point point::operator*(float a) const{
    point temp;
    temp.f_value = f_value * a;
    temp.d_value = d_value * a;
    return temp;
};
point point::operator/(float a) const{
    point temp;
    temp.f_value = f_value / a;
    temp.d_value = d_value / a;
    return temp;
};

point sin(point a){
    point temp;
    temp.f_value = sin(a.f_value);
    temp.d_value = cos(a.f_value)*a.d_value;
    return temp;
}

point cos(point a){
    point temp;
    temp.f_value = cos(a.f_value);
    temp.d_value = sin(a.f_value)*(-1)*a.d_value;
    return temp;
}

point exp(point a){
    point temp;
    temp.f_value = exp(a.f_value);
    temp.d_value = exp(a.f_value)*a.d_value;
    return temp;
}


template<typename T>
T f(std::vector<T> arg){
    
    return exp(arg[1]*arg[1]/(-4))*sin(arg[0]*2-0.707)*4;
}




int grad_descent_Nesterov(std::function<point(std::vector<point>)>& func, std::vector<float>& arg){

    int n = arg.size();

    if(n == 0) return 0;

    float a = 0.00001; //скорость обучения
    float b = 0.9; // скорость затухания инерции "шарика)"
    int m = 100000; // колво итераций
    float ideal = 1e-6; // условие сходимости 

    std::vector<float> v(n, 0.0);

    for(int i_n = 0; i_n<m; i_n++){
        std::vector<std::vector<point>> grad_point(n, std::vector<point>(n));
        std::vector<std::vector<point>> grad_pointS(n, std::vector<point>(n));

        for(int i = 0; i<n; i++){
            for(int j = 0; j<n; j++){
                grad_point[i][j] = point(arg[j], (j == i)? 1.0 : 0.0);
                grad_pointS[i][j] = point(arg[j]+b*v[j], (j == i)? 1.0 : 0.0);
            }
        }

        std::vector<float> grad(n);
        std::vector<float> gradS(n);
        for(int i = 0; i < n; i++){
            grad[i] = func(grad_point[i]).d_value;
            gradS[i] = func(grad_pointS[i]).d_value;
        }

        float grad_norm = 0;

        for(int i = 0; i<n; i++){
            grad_norm += grad[i]*grad[i];
        }

        grad_norm = sqrt(grad_norm);

        if(grad_norm < ideal){
            std::cout << "Сошлось..." << std::endl;
            break;
        }

        for(int i = 0; i<n; i++){
            v[i] = b*v[i]-a*gradS[i];
            arg[i] = arg[i] + v[i];
        }


    }
    return 0;
}


int main(){
    int n = 0;
    std::cin >> n;
    std::function<point(std::vector<point>)> func = f<point>;
    std::vector<float> arg(n);
    for(int i = 0; i<n; i++){
        std::cin >> arg[i];
    }

    grad_descent_Nesterov(func, arg);

    std::cout << "(";
    for(int i = 0; i<n; i++){
        std::cout << arg[i];
        if(i<n-1) std::cout << " ";
    }
    std::cout << ")";

    return 0;
}