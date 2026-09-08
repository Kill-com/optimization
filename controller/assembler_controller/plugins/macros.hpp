
#define REGISTER_FUNCTION \
static auto f_() { \
    return &f; \
}