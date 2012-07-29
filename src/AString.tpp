template<class T> AString::AString(const T *num_array, int n) {
    (*this) += "[";

    for(int i = 0; i < n; i++) {
        (*this) += AString(num_array[i]);
        if (i != n - 1)
            (*this) += ", ";
    }

    (*this) += "]";
}
template<class T> AString::AString(const typename std::vector<T>& _vector) {
    (*this) = AString("[") + AString(", ").join(_vector) + AString("]");
}
template<class T> AString::AString(const typename std::list<T>& _list) {
    (*this) = AString("[") + AString(", ").join(_list) + AString("]");
}
template<class K, class V> AString::AString(const typename std::map<K, V>& _map) {
    std::vector<std::string> strvec;
    strvec.resize(_map.size());
    typename std::map<K, V>::const_iterator it = _map.begin();

    for(int i = 0; it != _map.end(); (it++, i++)) {
        strvec[i] = AString(it->first) + ": " + AString(it->second);
    }

    (*this) = AString("{") + AString(", ").join(strvec) + AString("}");
}
template<class T> AString::AString(const typename std::set<T>& _set) {
    (*this) = AString("{") + AString(", ").join(_set) + AString("}");
}

template<class T> AString AString::join(const typename std::vector<T>& _vector) const {
    std::vector<AString> strvec;
    int total_length = 0;
    strvec.resize(_vector.size());

    typename std::vector<T>::const_iterator it = _vector.begin();

    for(int i = 0; it != _vector.end(); (it++, i++)) {
        strvec[i] = AString(*it);
        total_length += strvec[i].length();
        total_length += this->length();
    }

    if (!_vector.empty())
        total_length -= this->length();

    char* result = new char[total_length + 1];
    result[total_length] = 0;

    typename std::vector<AString>::iterator it2 = strvec.begin();

    unsigned int pos = 0;
    for(unsigned int i = 0; it2 != strvec.end(); (it2++, i++)) {
        memcpy(result + pos, it2->c_str(), it2->length());
        pos += it2->length();

        if (i != strvec.size() - 1) {
            memcpy(result + pos, c_str(), length());
            pos += length();
        }
    }

    return AString(result);
}
template<class T> AString AString::join(const typename std::list<T>& _list) const {
    typename std::vector<T> _vector = std::vector<T>(_list.begin(), _list.end());
    return join(_vector);
}
template<class T> AString AString::join(const typename std::set<T>& _set) const {
    typename std::vector<T> _vector = std::vector<T>(_set.begin(), _set.end());
    return join(_vector);
}
