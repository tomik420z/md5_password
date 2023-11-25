#include "md5_password.h" 
#include "generator.h"
#include <future>
#include <cmath>
#include <ctime>



class md5_password_calculator {
public:
    /// @brief конструктор с парметрами 
    /// @param _size_str - размер строки 
    /// @param _count_thr - кол-во потоков
    /// @param _code - код 
    md5_password_calculator(size_t _size_str, 
                            size_t _count_thr, 
                            const char* _code) 
        : m_size_str(_size_str),
          m_count_thr(_count_thr),
          m_ans(),
          m_key(_code),
          m_flag_ans(false), 
          m_count_combiantion(std::pow(36, _size_str)) {}

    //! деструктор 
    ~md5_password_calculator() = default;

    /// @brief - метод, вычисляющий  путём грубой силы, закодированную строку   
    /// @return - строка, которая была закодирована, с помощью ключа md5 
    std::string calculate() {
        // распределния вычислений по потокам 
        size_t intrvl = m_count_combiantion / m_count_thr;

        std::vector<std::future<void>> vec_thr;
        vec_thr.reserve(m_count_thr);
        size_t left_border = 0;
        for(size_t i = 0; i < m_count_thr - 1; ++i) {
            vec_thr.emplace_back(std::async(calculate_range, 
                                            this, 
                                            left_border,
                                            left_border + intrvl));

            left_border += intrvl;
        }

        vec_thr.emplace_back(std::async(calculate_range, 
                                            this, 
                                            left_border,
                                            m_count_combiantion - 1)); 


        for(auto& fut : vec_thr) {
            fut.wait();
        }

        return m_ans;
    }

private:
    /// @brief вычислить интервал (фунция для потоков)
    /// @param _first - левая граница 
    /// @param _last  - правая граница (не включёная)
    void calculate_range(size_t _first, size_t _last) {
        
        generator g(m_size_str, _first);
        // проверить первое значение 
        md5_password current_key;
        const auto& str = g.seq();
        current_key.encode(str);
        
        if (current_key == m_key) {
            m_flag_ans = true;
            m_ans = str;
        }
        ++_first;
        

        for(; _first < _last && !m_flag_ans; ++_first) {

            const auto& str = g.gen(); 
            current_key.encode(str);
            if (current_key == m_key) {
                m_flag_ans = true;
                m_ans = str;
            }
        }
        
    }

    // введённый ключ  
    md5_password m_key;
    // размер строки     
    size_t m_size_str;
    // кол-во потоков 
    size_t m_count_thr;
    // ответ 
    std::string m_ans;
    // признак нахождения ответа 
    bool m_flag_ans;
    // кол-во комбинаций 
    const size_t m_count_combiantion;
};


void upper_case(char* _first, char* _last) {
    for(; _first != _last; ++_first) {
        *_first = static_cast<char>(toupper(*_first));
    }
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "incorrect input format.\ninput format:\n./main.exe -hash_md5 -string_length -count_thr" << std::endl;
        return 0;
    }
    
    size_t str_length = std::atoi(argv[2]);
    size_t count_thr = std::atoi(argv[3]);
    if (str_length == 0 || count_thr == 0) {
        std::cerr << "incorrect input" << std::endl;
    }
    
    upper_case(argv[1], argv[1] + strlen(argv[1]));
    clock_t start = clock();
    md5_password_calculator calculator(str_length, count_thr, argv[1]);
    std::string ans = calculator.calculate();
    clock_t finish = clock();

    std::cout << "result: "<< ans << std::endl;
    std::cout << "time execution: " << static_cast<double>(finish - start) / CLOCKS_PER_SEC << " sec." << std::endl; 
    return 0;
}