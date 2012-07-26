#include "AString.hpp"

#include <iostream>

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <set>

#include <cstring>
#include <cmath>

AString::AString() : std::string() { }
//Constructors
AString::AString(char c) {
    (*this) += c;
}
AString::AString(int n) {
    std::stringstream s;
    s << n;
    (*this) += s.str();
}
AString::AString(double n) {
    std::stringstream s;
    s << n;
    (*this) += s.str();
}
AString::AString(const std::string& str) {
    (*this) += str;
}

//Operators
AString& AString::operator=(const std::string& rhs) {
    (*this) = AString(rhs.c_str());

    return *this;
}

void AString::operator*=(float num) {
    if (num <= 0) {
        (*this) = AString();
        return;
    }

    int floored_num = (int)floor(num);
    int rest = (int)floor((num - floored_num) * this->size() + 0.5);

    if (floored_num == 0) {
        (*this) = this->slice(0, rest);
        return;
    }
    else {
        AString original = (*this);
        for(int i = 0; i < floored_num - 1; i++) {
            (*this) += original;
        }

        (*this) += original.slice(0, rest);
    }
}
AString AString::operator*(float num) const {
    AString result = (*this);
    result *= num;
    return result;
}

void AString::operator-=(unsigned int num) {
    if (num >= this->size())
        (*this) = AString();
    (*this) = this->slice(0, -num);
}
AString AString::operator-(unsigned int num) const {
    AString result = (*this);
    result -= num;
    return result;
}

const char& AString::operator[] ( long pos ) const {
    if (pos >= 0) {
        return std::string::operator[](pos);
    }
    else {
        pos = size() + pos;
        return std::string::operator[](pos);
    }
}

//Misc. functions

bool AString::contains(const AString& substring) const {
    return (find(substring) != std::string::npos);
}
bool AString::contains(const char character) const {
    return (find(character) != std::string::npos);
}

AString AString::slice(int start, int end) const {
    if (start < 0) start = size() + start;
    if (end < 0) end = size() + end;

    if (start >= end) return AString();

    return substr(start, end - start);
}
AString AString::slice(int start) const {
    return slice(start, size());
}

//Operations
AString AString::reverse() const {
    char* result = new char[length()];
    int i = 0;
    int i2 = length() - 1;
    result[length()] = 0x00;
    while (true) {
        char byte = (*this)[i];
        if (byte == 0x00) {
            break;
        }
        int oi = i;
        switch( byte & 0xfc ) {
            case 0xfc:
                result[i2--] = (*this)[oi + 5];
                i++;
            case 0xf8:
                result[i2--] = (*this)[oi + 4];
                i++;
            case 0xf0:
                result[i2--] = (*this)[oi + 3];
                i++;
            case 0xe0:
                result[i2--] = (*this)[oi + 2];
                i++;
            case 0xc0:
                result[i2--] = (*this)[oi + 1];
                i++;
            default:
                result[i2--] = byte;
                i++;
        }
    }

    return AString (result);
}


AString AString::lstrip(const std::string& chars) const {
    AString _chars(chars);
    unsigned int max_i = 0;
    for(unsigned int i = 0; i < length(); i++) {
        if (!_chars.contains((*this)[i])) {
            max_i = i;
            break;
        }
    }
    return slice(max_i);
}
AString AString::rstrip(const std::string& chars) const {
    AString _chars = AString(chars);
    unsigned int max_i = 0;
    for(unsigned int i = -1; i > -length(); i--) {
        if (!_chars.contains((*this)[i])) {
            max_i = i;
            break;
        }
    }
    if (max_i == 0)
        return (*this);
    else
        return slice(0, max_i);
}
AString AString::strip(const std::string& chars) const {
    return this->lstrip(chars).rstrip(chars);
}

AString AString::rpad(unsigned int width, const std::string& filling) const {
    if ( width <= length() )
        return (*this);
    else {
        AString _filling = AString(filling);
        if (_filling.length() == 0) _filling = AString(" ");

        AString result = (*this);
        float x = (1.0 * width - length()) / _filling.length();
        result += _filling * x;

        return result;
    }
}
AString AString::lpad(unsigned int width, const std::string& filling) const {
    if ( width <= length() )
        return (*this);
    else {
        AString _filling = AString(filling).reverse();
        if (_filling.length() == 0) _filling = AString(" ");

        float x = (1.0 * width - length()) / _filling.length();
        AString result = (_filling * x).reverse() + (*this);

        return result;
    }
}
AString AString::pad(unsigned int width, const std::string& filling) const {
    return rpad(width, filling);
}

AString AString::rtrim(unsigned int width, const std::string& cutoff_string) const {
    AString _cutoff_string = AString(cutoff_string);

    if (width >= length())
        return (*this);
    else if (_cutoff_string.length() == width)
        return _cutoff_string;
    else if (width < _cutoff_string.length())
        return _cutoff_string.slice(0, width);
    else
        return slice(0, width - _cutoff_string.length()) + _cutoff_string;
}
AString AString::ltrim(unsigned int width, const std::string& cutoff_string) const {
    AString _cutoff_string = AString(cutoff_string);

    if (width >= length())
        return (*this);
    else if (_cutoff_string.length() == width)
        return _cutoff_string;
    else if (width < _cutoff_string.length())
        return _cutoff_string.slice(-width);
    else
        return _cutoff_string + slice(-width + _cutoff_string.length());
}
AString AString::trim(unsigned int width, const std::string& cutoff_string) const {
    return rtrim(width, cutoff_string);
}

AString AString::rtrimpad(unsigned int width, const std::string& filling, const std::string& cutoff_string) const {
    if (length() == width)
        return (*this);
    else if (length() < width)
        return rpad(width, filling);
    else
        return rtrim(width, cutoff_string);
}
AString AString::ltrimpad(unsigned int width, const std::string& filling, const std::string& cutoff_string) const {
    if (length() == width)
        return (*this);
    else if (length() < width)
        return lpad(width, filling);
    else
        return ltrim(width, cutoff_string);
}
AString AString::trimpad(unsigned int width, const std::string& filling, const std::string& cutoff_string) const {
    if (length() == width)
        return (*this);
    else if (length() < width)
        return pad(width, filling);
    else
        return trim(width, cutoff_string);
}

AString AString::replace(const std::string& old_str, const std::string& new_str) const {
    int idx = find(old_str);
    AString result = (*this);
    while (idx != -1) {
        result = result.slice(0, idx) + new_str + result.slice(idx + old_str.length());
        idx = result.find(old_str);
    }
    return result;
}
AString AString::replace(const std::set<std::string>& old_str_set, const std::string& new_str) const {
    std::set<std::string>::const_iterator it = old_str_set.begin();

    AString result = (*this);
    for(; it != old_str_set.end(); it++) {
        AString old_str = (*it);

        int idx = find(old_str);
        while (idx != -1) {
            result = result.slice(0, idx) + new_str + result.slice(idx + old_str.length());
            idx = result.find(old_str);
        }
    }
    return result;
}
AString AString::replace(const std::vector<std::string>& old_str_vec, const std::string& new_str) const {
    std::set<std::string> old_str_set( old_str_vec.begin(), old_str_vec.end() );
    return replace(old_str_set, new_str);
}
AString AString::replace(const std::list<std::string>& old_str_list, const std::string& new_str) const {
    std::set<std::string> old_str_set( old_str_list.begin(), old_str_list.end() );
    return replace(old_str_set, new_str);
}
AString AString::replace(const std::map<std::string, std::string>& replacement_map) const {
    std::map<std::string, std::string>::const_iterator it = replacement_map.begin();

    AString result = (*this);
    for(; it != replacement_map.end(); it++) {
        AString old_str = it->first;
        AString new_str = it->second;

        int idx = find(old_str);
        while (idx != -1) {
            result = result.slice(0, idx) + new_str + result.slice(idx + old_str.length());
            idx = result.find(old_str);
        }
    }
    return result;
}

AString AString::replace(const std::vector<std::string>& old_strings, const std::vector<std::string>& new_strings) const {
    std::vector<std::string> _old_strings = old_strings;
    std::vector<std::string> _new_strings = new_strings;

    if ( old_strings.size() > new_strings.size() ) {
        _new_strings.resize(old_strings.size(), "");
    }
    else if ( old_strings.size() < new_strings.size() ) {
        _old_strings.resize(new_strings.size());
    }
    std::vector<std::string>::const_iterator it1, it2;
    it1 = _old_strings.begin();
    it2 = _new_strings.begin();

    AString result = (*this);
    for(; it1 != _old_strings.end(); (it1++, it2++)) {
        result = result.replace(*it1, *it2);
    }
    return result;
}

AString AString::replace(const std::list<std::string>& old_strings, const std::list<std::string>& new_strings) const {
    std::list<std::string> _old_strings = old_strings;
    std::list<std::string> _new_strings = new_strings;

    if ( old_strings.size() > new_strings.size() ) {
        _new_strings.resize(old_strings.size(), "");
    }
    else if ( old_strings.size() < new_strings.size() ) {
        _old_strings.resize(new_strings.size());
    }
    std::list<std::string>::const_iterator it1, it2;
    it1 = _old_strings.begin();
    it2 = _new_strings.begin();

    AString result = (*this);
    for(; it1 != _old_strings.end(); (it1++, it2++)) {
        result = result.replace(*it1, *it2);
    }
    return result;
}

std::vector<AString> AString::lpartition(const std::string& split_str, bool include_separator) const {
    std::vector<AString> result;

    unsigned int idx = find(split_str);
    if (idx == std::string::npos) {
        result.push_back(*this);
        result.push_back(AString());
        result.push_back(AString());
    }
    else {
        AString left, middle, right;

        left = slice(0, idx);
        middle = slice(idx, idx + split_str.length());
        right = slice(idx + split_str.length());

        result.push_back(left);
        if (include_separator) {
            result.push_back(middle);
        }

        result.push_back(right);
    }

    return result;
}
std::vector<AString> AString::rpartition(const std::string& split_str, bool include_separator) const {
    std::vector<AString> result;

    unsigned int idx = rfind(split_str);
    if (idx == std::string::npos) {
        result.push_back(AString());
        result.push_back(AString());
        result.push_back(*this);
    }
    else {
        AString left, middle, right;

        left = slice(0, idx);
        middle = slice(idx, idx + split_str.length());
        right = slice(idx + split_str.length());

        result.push_back(left);
        if (include_separator) {
            result.push_back(middle);
        }

        result.push_back(right);
    }

    return result;
}
std::vector<AString> AString::partition(const std::string& split_str, bool include_separator) const {
    return lpartition(split_str, include_separator);
}

std::vector<AString> AString::split(const std::string& split_str, bool ignore_empty) const {
    std::vector<AString> result;
    AString rest = (*this);
    while (rest.length() > 0) {
        unsigned int idx = rest.find(split_str);
        if (idx == std::string::npos) {
            if ( (rest.length() == 0) & ignore_empty ) break;
            result.push_back(rest);
            break;
        }
        else {
            if ( (idx > 0) | (!ignore_empty) ) {
                result.push_back(rest.slice(0, idx));
            }
            rest = rest.slice(idx + split_str.length());
        }
    }

    return result;
}

std::vector<AString> AString::splitlines() const {
    if (find("\r\n") < find("\n"))
        return split("\r\n");
    else
        return split("\n");
}

AString AString::upper() const {
    if (length() == 0) return (*this);

    char* result = new char[length()];
    for(unsigned int i = 0; i < length(); i++) {
        char chr = (*this)[i];
        if ( (chr >= 'a') && (chr <= 'z') ) {
            result[i] = chr - 32;
        }
        else {
            result[i] = chr;
        }
    }
    return AString(result);
}
bool AString::isupper() const {
    if (length() == 0) return false;

    for(unsigned int i = 0; i < length(); i++) {
        char chr = (*this)[i];
        if ( (chr >= 'a') && (chr <= 'z') ) {
            return false;
        }
    }
    return true;
}

AString AString::lower() const {
    if (length() == 0) return (*this);

    char* result = new char[length()];
    for(unsigned int i = 0; i < length(); i++) {
        char chr = (*this)[i];
        if ( (chr >= 'A') && (chr <= 'Z') ) {
            result[i] = chr + 32;
        }
        else {
            result[i] = chr;
        }
    }
    return AString(result);
}
bool AString::islower() const {
    if (length() == 0) return false;

    for(unsigned int i = 0; i < length(); i++) {
        char chr = (*this)[i];
        if ( (chr >= 'A') && (chr <= 'Z') ) {
            return false;
        }
    }
    return true;
}

AString AString::capitalize() const {
    if (length() == 0) return (*this);

    char chr[2] = { (*this)[0], 0 };
    if ( (chr[0] >= 'a') && (chr[0] <= 'z') ) {
        chr[0] -= 32;
        return AString(chr) + slice(1);
    }
    else {
        return (*this);
    }

}
bool AString::iscapital() const {
    if (length() == 0) return false;

    char chr = (*this)[0];
    return ( (chr >= 'a') && (chr <= 'z') );
}

bool AString::isnumber() const {
    if (length() == 0) return false;

    unsigned int i = 0;
    bool has_dot = false;
    if ((*this)[i] == '-') i = 1;
    if ((*this)[i] == '+') i = 1;

    for(; i < length(); i++) {
        char chr = (*this)[i];
        if (chr == '-') {
            if (has_dot) return false;
            has_dot = true;
        }
        else if ( (chr < '0') || (chr > '9') ) {
            return false;
        }
    }

    return true;
}

bool AString::startswith(const std::string& pattern) const {
    return (find(pattern) == 0);
}
bool AString::startswith(const std::vector<std::string>& pattern_vector) const {
    std::vector<std::string>::const_iterator it = pattern_vector.begin();
    for(; it != pattern_vector.end(); it++) {
        if (startswith(*it)) return true;
    }
    return false;
}
bool AString::startswith(const std::list<std::string>& pattern_list) const {
    std::list<std::string>::const_iterator it = pattern_list.begin();
    for(; it != pattern_list.end(); it++) {
        if (startswith(*it)) return true;
    }
    return false;
}
bool AString::startswith(const std::set<std::string>& pattern_set) const {
    std::set<std::string>::const_iterator it = pattern_set.begin();
    for(; it != pattern_set.end(); it++) {
        if (startswith(*it)) return true;
    }
    return false;
}

bool AString::endswith(const std::string& pattern) const {
    return (find(pattern) == length() - pattern.length());
}
bool AString::endswith(const std::vector<std::string>& pattern_vector) const {
    std::vector<std::string>::const_iterator it = pattern_vector.begin();
    for(; it != pattern_vector.end(); it++) {
        if (endswith(*it)) return true;
    }
    return false;
}
bool AString::endswith(const std::list<std::string>& pattern_list) const {
    std::list<std::string>::const_iterator it = pattern_list.begin();
    for(; it != pattern_list.end(); it++) {
        if (endswith(*it)) return true;
    }
    return false;
}
bool AString::endswith(const std::set<std::string>& pattern_set) const {
    std::set<std::string>::const_iterator it = pattern_set.begin();
    for(; it != pattern_set.end(); it++) {
        if (endswith(*it)) return true;
    }
    return false;
}

AString AString::remove(const std::string& chars) const {
    AString _chars = AString(chars);
    AString result;
    for(unsigned int i = 0; i < length(); i++) {
        if (!_chars.contains((*this)[i]))
            result += (*this)[i];
    }
    return result;
}

std::vector<AString> AString::words() const {
    AString chars = AString("\t \r\n1234567890!@#$%^&*()`~-_=+[{]};:'\",<.>/?\\|");
    AString stripped_str;
    bool space_inserted = false;
    for(unsigned int i = 0; i < length(); i++) {
        if (!chars.contains((*this)[i])) {
            stripped_str += (*this)[i];
            space_inserted = false;
        }
        else {
            if (!space_inserted) {
                stripped_str += " ";
                space_inserted = true;
            }
        }
    }
    std::vector<AString> word_vec = stripped_str.split(" ", true);
    return word_vec;
}
std::set<AString> AString::words_set() const {
    std::vector<AString> word_vec = words();
    std::set<AString> word_set( word_vec.begin(), word_vec.end() );
    return word_set;
}

AString AString::center(unsigned int width, const std::string& lfilling, const std::string& rfilling) const {
    if ( length() >= width) return (*this);
    else {
        AString _lfilling(lfilling);
        AString _rfilling(rfilling);
        int ln = (width - length()) / 2;

        return this->lpad(length() + ln, _lfilling).rpad(width, _rfilling);
    }
}
AString AString::center(unsigned int width, const std::string& filling) const {
    return center(width, filling, filling);
}

int AString::distance(const std::string& other) const {
    //Copied from http://en.wikibooks.org/wiki/Algorithm_Implementation/AStrings/Levenshtein_distance#C.2B.2B
    const size_t len1 = this->size(), len2 = other.size();
    std::vector<std::vector<unsigned int> > d(len1 + 1, std::vector<unsigned int>(len2 + 1));

    d[0][0] = 0;
    for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
    for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

    for(unsigned int i = 1; i <= len1; ++i)
    for(unsigned int j = 1; j <= len2; ++j)
        d[i][j] = std::min( std::min(d[i - 1][j] + 1,d[i][j - 1] + 1),
            d[i - 1][j - 1] + ((*this)[i - 1] == other[j - 1] ? 0 : 1) );

    return d[len1][len2];
}

long AString::as_int() const {
    if (!isnumber()) return 0;
    long n;
    std::stringstream(*this) >> n;
    return n;
}
double AString::as_float() const {
    if (!isnumber()) return 0;
    double n;
    std::stringstream(*this) >> n;
    return n;
}
bool AString::as_bool() const {
    AString _this = this->lower();
    if (_this == "true") return true;
    if (_this == "yes") return true;
    if (_this == "1") return true;
    if (_this == "on") return true;
    if (_this == "enabled") return true;

    if (_this == "false") return false;
    if (_this == "no") return false;
    if (_this == "0") return false;
    if (_this == "off") return false;
    if (_this == "disabled") return false;

    return false;
}

std::vector<AString> AString::as_string_vector() const {
    AString _this = (*this);
    if ( (_this[0] == '[') && (_this[-1] == ']') ) {
        //Most likely converted via AString( ... ), pardon if not
        _this = _this.strip("[]").replace(", ", " ");
    }
    else if ( (_this[0] == '{') && (_this[-1] == '}') ) {
        //Most likely converted via AString( ... ), pardon if not
        _this = _this.strip("{}").replace(", ", " ");
    }

    return split(" ", true);
}
std::vector<int> AString::as_int_vector() const {
    std::vector<AString> svec = as_string_vector();
    std::vector<int> ivec;
    ivec.resize(svec.size());

    std::vector<AString>::iterator it = svec.begin();

    for(; it != svec.end(); it++) {
        ivec[ it - svec.begin() ] = (*it).as_int();
    }

    return ivec;
}
std::vector<float> AString::as_float_vector() const {
    std::vector<AString> svec = as_string_vector();
    std::vector<float> fvec;
    fvec.resize(svec.size());

    std::vector<AString>::iterator it = svec.begin();

    for(; it != svec.end(); it++) {
        fvec[ it - svec.begin() ] = (*it).as_float();
    }

    return fvec;
}
std::vector<bool> AString::as_bool_vector() const {
    std::vector<AString> svec = as_string_vector();
    std::vector<bool> bvec;
    bvec.resize(svec.size());

    std::vector<AString>::iterator it = svec.begin();

    for(; it != svec.end(); it++) {
        bvec[ it - svec.begin() ] = (*it).as_bool();
    }

    return bvec;
}

std::list<AString> AString::as_string_list() const {
    std::vector<AString> _vector = as_string_vector();
    std::list<AString> _list( _vector.begin(), _vector.end() );
    return _list;
}
std::list<int> AString::as_int_list() const {
    std::vector<int> _vector = as_int_vector();
    std::list<int> _list( _vector.begin(), _vector.end() );
    return _list;
}
std::list<float> AString::as_float_list() const {
    std::vector<float> _vector = as_float_vector();
    std::list<float> _list( _vector.begin(), _vector.end() );
    return _list;
}
std::list<bool> AString::as_bool_list() const {
    std::vector<bool> _vector = as_bool_vector();
    std::list<bool> _list( _vector.begin(), _vector.end() );
    return _list;
}

std::set<AString> AString::as_string_set() const {
    std::vector<AString> _vector = as_string_vector();
    std::set<AString> _set( _vector.begin(), _vector.end() );
    return _set;
}
std::set<int> AString::as_int_set() const {
    std::vector<int> _vector = as_int_vector();
    std::set<int> _set( _vector.begin(), _vector.end() );
    return _set;
}
std::set<float> AString::as_float_set() const {
    std::vector<float> _vector = as_float_vector();
    std::set<float> _set( _vector.begin(), _vector.end() );
    return _set;
}
std::set<bool> AString::as_bool_set() const {
    std::vector<bool> _vector = as_bool_vector();
    std::set<bool> _set( _vector.begin(), _vector.end() );
    return _set;
}
