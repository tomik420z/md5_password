
#ifndef GENERATOR_H
#define GENERATOR_H
#include <string>

/// @brief генератор всех возможный комбинаций из английского алфавита и цифр
class generator {
public:
    /// @briefконструктор с пармаетрами 
    /// @param _start - началльное значение 
    /// @param _sz - длина строки  
    generator(size_t _sz = 5, int _start = 0) : m_seq() {
        
        m_seq.resize(_sz);

        for(auto it = m_seq.rbegin(); it != m_seq.rend(); ++it) {
            *it = s_alphabet[_start % 36];
            _start /= 36;
        }

        m_index = init_index();
        const char ch =  m_seq[m_index];

        if ('a' <= ch && ch <= 'z') {
            m_iter_alphapet = ch - 97; 
        } else {
            m_iter_alphapet = ch - 22;
        }

    }

    /// @brief получить текущую последовательность 
    /// @return константная ссылка на текущую последовательность 
    const std::string& seq() {
        return m_seq;
    }

    /// @brief сгенерировать следующую строку  
    /// @return - строка 
    const std::string& gen() {

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

        return m_seq;
    }

private:

    int init_index() const {
        for(int i = m_seq.size() - 1; i >= 0; ++i) {
            if (m_seq[i] != '9') {
                return i;
            }
        }

        return 0;
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
    char next()  {
        return s_alphabet[++m_iter_alphapet];
    } 
private:
    /// последовательность 
    std::string m_seq; 
    
    int m_index;
    // итератор по алфавиту 
    int m_iter_alphapet;
    const std::string s_alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
};

#endif