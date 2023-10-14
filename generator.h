
#ifndef GENERATOR_H
#define GENERATOR_H
#include <string>

/// @brief генератор всех возможный комбинаций из английского алфавита и цифр
class generator {
public:
    generator(size_t _sz = 5) 
        : m_seq()
        , m_index(_sz - 1)
        , m_iter_alphapet(s_alphabet.begin()) {

        m_seq.reserve(_sz);
        for(size_t i = 0; i < _sz; ++i) {
            m_seq.push_back('a');
        }
    }

    generator(size_t _sz, size_t _start) {
        m_seq.resize(_sz);
        for(auto it = m_seq.rbegin(); it != m_seq.rend(); ++it) {
            size_t mod = _start % s_alphabet.size();
            *it = s_alphabet[mod];
            _start /= 36;
        }

        set_index();
        set_iter_alphapet();
    }



    /// @brief сгенерыровать следующую строку  
    /// @return - строка 
    std::string gen() {
        const std::string prev = m_seq;

        if (m_seq[m_index] == '9') {
            int i = m_index - 1;

            while(m_seq[i] == '9') {
                if (i == 0) {
                    return last_value();
                }
                --i;
            }

            m_seq[i] = next(m_seq[i]);

            reset(i);  

            m_index = m_seq.size() - 1;     
            m_iter_alphapet = s_alphabet.begin();
        } else {
            m_seq[m_index] = next();            
        }

        return prev;
        
    }

private:
    /// @brief установить значение m_iter_alphapet
    void set_iter_alphapet() {
        char ch = m_seq[m_index];
        for(auto it = s_alphabet.begin(); it != s_alphabet.end(); ++it) {
            if (ch == *it) {
                m_iter_alphapet = it;
                return;
            }
        }
    } 
    
    /// @brief установить значение m_index
    void set_index() {
        int i = m_seq.size() - 1;
        while(m_seq[i] == '9') {
            --i;
        }
        m_index = i;
    }
    /// @brief сбосить символы справа от индекса  
    /// @param index 
    void reset(int index) {
        for(size_t i = index + 1; i < m_seq.size(); ++i) {
            m_seq[i] = 'a';
        }
    } 
    /// @brief следующий символ старшего разряда 
    /// @param ch - текущий символ 
    /// @return 
    char next(char ch) {
        if ('a' <= ch && ch <= 'z') {
            return s_alphabet[ch - 97 + 1]; 
        } else {
            return s_alphabet[ch - 22 + 1];
        }
    }
    
    /// @brief следующий символ
    char next() {
        return *(++m_iter_alphapet);
    } 
    
    std::string last_value() {
        std::string last_val(m_seq.size(), '\0');
        std::fill(std::begin(last_val), std::end(last_val), '9');
        return last_val;
    }
private:
    std::string m_seq; 
    int m_index;
    std::string::const_iterator m_iter_alphapet;
    const std::string s_alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
};

#endif