
template<typename T>
class F_LIST_{
public:
    static T f(T x) {
        return std::pow((x-2), 2) + std::sin(x);
    }
    REGISTER_FUNCTION
};