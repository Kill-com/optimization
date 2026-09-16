#pragma once
#include <vector>
#include <string>

#include <thread>

#include "../start_controller/execution.hpp"
#include "Ithread.hpp"

template<template<typename...> class Classes, typename ...Args>
class ExectControl:public StartCalculation<Classes, Args...>,
    public ThreadTask
{

public:
    using StartCalculation<Classes, Args...>::StartCalculation;
    void exect(Args&& ... args){
        LOCK
        this->input_value(std::forward<Args>(args)...);
        ex=std::thread([this]{this->StartCalculation<Classes, Args...>::exect();});
    }
    ~ExectControl(){
        if (ex.joinable()) ex.join();
        ThreadTask::~ThreadTask();
    }
};