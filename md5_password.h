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
        char* data = reinterpret_cast<char*>(dig);
        boost::algorithm::unhex(_code, _code + strlen(_code), data);
    }

    /// @brief сравнение на равенство двух ключей
    /// @param _rhs - ключ спарва от равенства 
    /// @return 
    bool operator==(const md5_password &_rhs) const {
        const char* dataLhs = reinterpret_cast<const char*>(&dig);
        const char* dataRhs = reinterpret_cast<const char *>(&(_rhs.dig));
        return std::equal(dataLhs, dataLhs + sizeof(md5::digest_type), dataRhs);
    }

    /// @brief перевод ключа md5 в строку в 16-ричной записи 
    /// @return - строка 
    std::string to_string() const {
        const auto char_digest = reinterpret_cast<const char *>(&dig);
        std::string result;
        boost::algorithm::hex(char_digest, char_digest + sizeof(md5::digest_type), std::back_inserter(result));
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
    const char* first = reinterpret_cast<const char*>(&_code);
    const char* last = first + sizeof(md5::digest_type);
    
    for(; first != last; ++first) {
        os << std::hex << *first;
    }
    return os;
}

#endif