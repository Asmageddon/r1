#ifndef ASTRING_HPP_
#define ASTRING_HPP_

#include <iostream>

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <set>

#include <cstring>
#include <cmath>

class AString: public std::string {
    public:
        AString();
        //Constructors
        AString(char c);
        AString(int n);
        AString(double n);
        AString(const std::string& str);

        template<class T> AString(const T *num_array, int n);
        template<class T> AString(const typename std::vector<T>& _vector);
        template<class T> AString(const typename std::list<T>& _list);
        template<class K, class V> AString(const typename std::map<K, V>& _map);
        template<class T> AString(const typename std::set<T>& _set);

        //Operators
        AString& operator=(const std::string& rhs);

        void operator*=(float num);
        AString operator*(float num) const;

        void operator-=(unsigned int num);
        AString operator-(unsigned int num) const;

        const char& operator[] ( long pos ) const;

        operator std::string() const {
            return std::string(c_str());
        }
        //Misc. functions

        bool contains(const AString& substring) const;
        bool contains(const char character) const;

        AString slice(int start, int end) const;
        AString slice(int start = 0) const;

        //Operations
        AString reverse() const;

        template<class T> AString join(const typename std::vector<T>& _vector) const;
        template<class T> AString join(const typename std::list<T>& _list) const;
        template<class T> AString join(const typename std::set<T>& _set) const;

        AString lstrip(const std::string& chars) const;
        AString rstrip(const std::string& chars) const;
        AString strip(const std::string& chars) const;

        AString rpad(unsigned int width, const std::string& filling = " ") const;
        AString lpad(unsigned int width, const std::string& filling = " ") const;
        AString pad(unsigned int width, const std::string& filling = " ") const;

        AString rtrim(unsigned int width, const std::string& cutoff_string = "") const;
        AString ltrim(unsigned int width, const std::string& cutoff_string = "") const;
        AString trim(unsigned int width, const std::string& cutoff_string = "") const;

        AString rtrimpad(unsigned int width, const std::string& filling, const std::string& cutoff_string) const;
        AString ltrimpad(unsigned int width, const std::string& filling, const std::string& cutoff_string) const;
        AString trimpad(unsigned int width, const std::string& filling, const std::string& cutoff_string) const;

        AString replace(const std::string& old_str, const std::string& new_str) const;
        AString replace(const std::set<std::string>& old_str_set, const std::string& new_str) const;
        AString replace(const std::vector<std::string>& old_str_vec, const std::string& new_str) const;
        AString replace(const std::list<std::string>& old_str_list, const std::string& new_str) const;
        AString replace(const std::map<std::string, std::string>& replacement_map) const;

        AString replace(const std::vector<std::string>& old_strings, const std::vector<std::string>& new_strings) const;

        AString replace(const std::list<std::string>& old_strings, const std::list<std::string>& new_strings) const;

        std::vector<AString> lpartition(const std::string& split_str=" ", bool include_separator = true) const;
        std::vector<AString> rpartition(const std::string& split_str=" ", bool include_separator = true) const;
        std::vector<AString> partition(const std::string& split_str=" ", bool include_separator = true) const;

        std::vector<AString> split(const std::string& split_str=" ", bool ignore_empty=false) const;

        std::vector<AString> splitlines() const;

        AString upper() const;
        bool isupper() const;

        AString lower() const;
        bool islower() const;

        AString capitalize() const;
        bool iscapital() const;

        bool isnumber() const;

        bool startswith(const std::string& pattern) const;
        bool startswith(const std::vector<std::string>& pattern_vector) const;
        bool startswith(const std::list<std::string>& pattern_list) const;
        bool startswith(const std::set<std::string>& pattern_set) const;

        bool endswith(const std::string& pattern) const;
        bool endswith(const std::vector<std::string>& pattern_vector) const;
        bool endswith(const std::list<std::string>& pattern_list) const;
        bool endswith(const std::set<std::string>& pattern_set) const;

        AString remove(const std::string& chars) const;

        std::vector<AString> words() const;
        std::set<AString> words_set() const;

        AString center(unsigned int width, const std::string& lfilling, const std::string& rfilling) const;
        AString center(unsigned int width, const std::string& filling = " ") const;

        int distance(const std::string& other) const ;

        long as_int() const;
        double as_float() const;
        bool as_bool() const;

        std::vector<AString> as_string_vector() const;
        std::vector<int> as_int_vector() const;
        std::vector<float> as_float_vector() const;
        std::vector<bool> as_bool_vector() const;

        std::list<AString> as_string_list() const;
        std::list<int> as_int_list() const;
        std::list<float> as_float_list() const;
        std::list<bool> as_bool_list() const;

        std::set<AString> as_string_set() const;
        std::set<int> as_int_set() const;
        std::set<float> as_float_set() const;
        std::set<bool> as_bool_set() const;
};

#include "AString.tpp"

#endif
