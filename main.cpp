#include "md5_password.h" 
#include "generator.h"

static constexpr unsigned long long _pow = std::pow(37, 5);

void upper_case(char* _first, char* _last) {
    for(; _first != _last; ++_first) {
        *_first = static_cast<char>(toupper(*_first));
    }
}

int main (int argc, char* argv[]) {
    upper_case(argv[1], argv[1] + strlen(argv[1]));

    md5_password key(argv[1]); 
    
    generator g(5);
    for(size_t i = 0; i < _pow; ++i) {
        md5_password seq;
        const auto& str = g.gen(); 
        seq.encode(str);
        if (seq == key) {
            std::cout << str << std::endl;
            break;
        }
    }
    return 0;
}