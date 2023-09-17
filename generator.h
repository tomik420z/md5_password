
#ifndef GENERATOR_H
#define GENERATOR_H
#include <string>

/// @brief генератор всех возможный комбинаций из английского алфавита и цифр
class generator {
public:
    generator(size_t _sz = 5) : m_seq(), m_index(_sz - 1), m_iter_alphapet(0) {
        m_seq.reserve(_sz);
        for(size_t i = 0; i < _sz; ++i) {
            m_seq.push_back('a');
        }
    }



    /// @brief сгенерыровать следующую строку  
    /// @return - строка 
    std::string gen() {
        std::string prev = m_seq;

        if (m_seq[m_index] == '9') {
            int i = m_index - 1;
            while(m_seq[i] == '9') {
                --i;
            }
            m_seq[i] = next(m_seq[i]);

            reset(i);  

            m_index = m_seq.size() - 1;     
            m_iter_alphapet = 0;
        } else {
            m_seq[m_index] = next();
            
        }

        return prev;
        
    }

private:
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
    char next()  {
        return s_alphabet[++m_iter_alphapet];
    } 
private:
    std::string m_seq; 
    int m_index;
    int m_iter_alphapet;
    const std::string s_alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
};

#endif