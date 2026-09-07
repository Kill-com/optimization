
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>
#include <fstream>      
#include <iomanip>      
#include <cstdlib>

#include "done/bracket_interval.h"
#include "done/search_min.h"
#include "done/gradient_calc.h"

template<typename B>
B f(std::vector<B> arg){
    //return (4-2.1*arg[0]*arg[0] + 3*arg[0]*arg[0]*arg[0]*arg[0])*arg[0]*arg[0] + arg[1]*arg[0] + (-4+4*arg[1]*arg[1])*arg[1]*arg[1];
    return 4*exp(-arg[1]*arg[1]/4) * sin(2*arg[0]- 1.414);
}

template<typename A>
std::vector<A> grad_descent(const std::function<A(std::vector<A>)>& func, std::vector<A> arg, std::vector<std::vector<double>>* history = nullptr){

    if (history) history->push_back(arg);
    std::size_t n = arg.size();

    if(n == 0) return {};

    A ideal = static_cast<A>(1e-4); // условие сходимости

    A grad_norm = 1;

    size_t iteration = 0;

    do{
        

        std::vector<A> grad = gradient_first_step<A>(func, arg);
        

        grad_norm = gradient_norm<A>(grad);
        
        if(grad_norm < ideal) break;

        std::function<A(A)> func_one = [&func, &arg, &grad, &grad_norm](A t){
            std::vector<A> arg_one_per = arg;
            for(std::size_t i = 0; i<arg.size(); i++) {arg_one_per[i] = arg_one_per[i] - grad[i]/grad_norm * t;}
            return func(arg_one_per);
        };

        
        std::function<A(A)> line_func(func_one);
        A a = parabola_method(line_func, bracket_constant<A>(line_func));
        

        for(std::size_t i = 0; i<n; i++){
            arg[i] = arg[i] - a*grad[i]/grad_norm;
        }

        if (history) history->push_back(arg);
        iteration++;
    }while(grad_norm > ideal);
    std::cout << "Количество итераций: " << iteration << "\n";
    return arg;
}


void save_history(const std::vector<std::vector<double>>& history, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << " для записи\n";
        return;
    }
    out << std::fixed << std::setprecision(6);
    for (const auto& pt : history) {
        for (size_t i = 0; i < pt.size(); ++i) {
            out << pt[i];
            if (i + 1 < pt.size()) out << " ";
        }
        out << "\n";
    }
    out.close();
}


// Генерация скрипта Gnuplot для двумерного случая (n=2)
void generate_gnuplot_script(const std::string& datafile1, const std::string& scriptfile) {
    std::ofstream gp(scriptfile);
    if (!gp.is_open()) {
        std::cerr << "Не удалось создать скрипт " << scriptfile << "\n";
        return;
    }
    gp << "set terminal qt size 500,500 enhanced\n";
    gp << "set title 'Траектория градиентного спуска на линиях уровня'\n";
    gp << "set xlabel 'x'\n";
    gp << "set ylabel 'y'\n";
    gp << "set grid\n";
    gp << "set size square\n";
    gp << "set xrange [-10:10]\n";
    gp << "set yrange [-10:10]\n";
    gp << "set contour base\n";
    gp << "set cntrparam levels auto 100\n";
    gp << "unset surface\n";
    gp << "set table 'contours.dat'\n";
    gp << "splot 4*exp(-y**2/4) * sin(2*x - 1.414)\n";   // здесь нужно подставить вашу функцию!
    gp << "unset table\n";
    gp << "plot 'contours.dat' with lines lc rgb 'gray' notitle, \\\n";
    gp << "     '" << datafile1 << "' with linespoints lw 2 lc rgb 'red' pt 7 ps 0.5 title 'Градиентный спуск', \n";
   //gp << "     '" << datafile2 << "' with linespoints lw 2 lc rgb 'blue' pt 7 ps 0.05 title 'Метод Нейстерова'\n";
    gp << "pause mouse close\n";
    gp.close();
    std::cout << "Создан скрипт Gnuplot: " << scriptfile << "\n";
    std::cout << "Запустите его командой: gnuplot -persist " << scriptfile << "\n";
}





int main(){
    std::size_t n = 0;
    std::cin >> n;
    std::function<double(std::vector<double>)> func = f<double>;
    std::vector<double> arg(n);
    for(std::size_t i = 0; i<n; i++){
        std::cin >> arg[i];
    }
    std::vector<std::vector<double>> history1;
    std::vector<double> arg_new = grad_descent(func, arg, &history1);

    std::cout << "(";
    for(std::size_t i = 0; i<n; i++){
        std::cout << arg_new[i];
        if(i<n-1) std::cout << " ";
    }
    std::cout << ")\n";

    if (n == 2) {
        save_history(history1, "trajectory1.txt");
        generate_gnuplot_script("trajectory1.txt", "plot.gp");
    } else {
        std::cout << "Визуализация доступна только для n=2 (сейчас n=" << n << ")\n";
    }

    return 0;
}