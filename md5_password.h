#ifndef MD5_PASSWORS_H
#define MD5_PASSWORD_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>


using boost::uuids::detail::md5;

/// @brief класс взаимодействия с md5 ключом  
class md5_password {
public:
    /// @brief конструктор по умолчанию 
    md5_password() : dig() {}

    /// @brief конструктор от кода 
    /// @param _code  - код
    md5_password(const char* _code) {  
        int* data = reinterpret_cast<int*>(&dig);
        boost::algorithm::unhex(_code, _code + strlen(_code), data);
    }

    /// @brief сравнение на равенство двух ключей
    /// @param _rhs - ключ спарва от равенства 
    /// @return 
    bool operator==(const md5_password &_rhs) const {
        const int* dataLhs = reinterpret_cast<const int*>(&dig);
        const int* dataRhs = reinterpret_cast<const int*>(&(_rhs.dig));
        return std::equal(dataLhs, dataLhs +(sizeof(md5::digest_type) / sizeof(int)), dataRhs);
    }

    /// @brief перевод ключа md5 в строку в 16-ричной записи 
    /// @return - строка 
    std::string to_string() const {
        const auto intDigest = reinterpret_cast<const int*>(&dig);
        std::string result;
        boost::algorithm::hex(intDigest, intDigest + (sizeof(md5::digest_type)/sizeof(int)), std::back_inserter(result));
        return result;
    }

    /// @brief - закодировать строку 
    /// @param _seq - строка (слово)
    void encode(const std::string& _seq) {
        md5 hash;
        hash.process_bytes(_seq.data(), _seq.size());
        hash.get_digest(dig);
    }

    const md5::digest_type& get_password() const noexcept {
        return dig;
    }

private:
    md5::digest_type dig;
};

std::ostream& operator<<(std::ostream& os, const md5_password& _code) {
    
    os << _code.to_string();
    
    return os;
}

#endif