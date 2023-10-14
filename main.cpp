#include "md5_password.h" 
#include "generator.h"
#include <thread>
#include <future>
#include <vector>
#include <ctime>


void upper_case(char* _first, char* _last) {
    for(; _first != _last; ++_first) {
        *_first = static_cast<char>(toupper(*_first));
    }
}

class word_search_algorithm {
public:
    word_search_algorithm(const char* _key, size_t _sz_word) 
        : m_key(_key)
        , is_find(false)
        , m_size_str(_sz_word) {}

    std::string alg() {
        size_t count_thr = std::thread::hardware_concurrency();
        const size_t count_combiantation = static_cast<size_t>(std::pow(36, m_size_str)); 
        size_t intrvl = count_combiantation / count_thr;
        std::vector<std::future<void>> v_thr;
        v_thr.reserve(count_thr);
        size_t left_border = 0;  

        for(size_t i = 0; i < count_thr - 1; ++i) {
            v_thr.emplace_back(
                std::async(word_search_algorithm::calculate_range, this,  left_border, left_border + intrvl));
            left_border += intrvl;
        }
        v_thr.emplace_back(std::async(
            word_search_algorithm::calculate_range, this, left_border, count_combiantation));

        for(size_t i = 0; i < count_thr; ++i) {
            v_thr[i].wait();
        }

        return ans;
    }
private:
    void calculate_range(size_t _first, size_t _last) {
        generator g(m_size_str, _first);

        for(; _first < _last && !is_find; ++_first) {
            md5_password seq;
            
            const auto& str = g.gen(); 
            
            seq.encode(str);
            if (seq == m_key) {
                ans = str;
                is_find = true;
                break;
            }
        }
    }


private:
    // ключ
    md5_password m_key;
    // флаг для потоков, найден ли ответ 
    bool is_find;
    // размер строки 
    size_t m_size_str;
    /// рещультирующая стркоа 
    std::string ans;
};

int main (int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "incorrect input format\n\ninput format ./main.exe -hash_md5";
        return -1;
    }
     
    upper_case(argv[1], argv[1] + strlen(argv[1]));

    size_t sz_str = atoi(argv[2]);

    word_search_algorithm a(argv[1], sz_str);
    clock_t t_start = clock();
    auto ans = a.alg();
    clock_t t_finish = clock();
    std::cout << ans << std::endl;
    std::cout << "time = " << static_cast<double>(t_finish - t_start) / 1000.0  << std::endl;
    return 0;
}