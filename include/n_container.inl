#ifndef N_CONTAINER_INL
#define N_CONTAINER_INL

#include <iostream>
#include <memory>

namespace NE {

//--------- NContainer<T> class implementation ---------

template<typename T>
void NContainer<T>::SetLength(size_t value) {
    size_t size = sizeof(T);
    this->length = value;
    this->capacity = ((size * value + 3) / 4) * 4 / size;
}

template<typename T>
void NContainer<T>::RawAllocate() {
    list = new T[capacity];
}

template<typename T>
void NContainer<T>::SafeAllocate(size_t length) {
    SetLength(length);
    if(!length) {
        //throw std::runtime_error("Null allocation terminated.");
        Deallocate();
        return;
    }
    RawAllocate();
}

template<typename T>
void NContainer<T>::Deallocate() {
    if (list != nullptr) {
		void* ptr = static_cast<void*>(list);
        delete[] list;
        list = nullptr;
    }
    SetLength(0);
}

template<typename T>
void NContainer<T>::Reallocate() {
    Deallocate();
    SafeAllocate(length);
}

template<typename T>
void NContainer<T>::Reallocate(size_t length) {
    Deallocate();
    SafeAllocate(length);
}

template<typename T>
void NContainer<T>::ReallocateSafe() {
    if (!list) {
        SafeAllocate(length);
        return;
    }

    T* save = new T[length];
    memcpy(save, list, length * sizeof(T));
    Reallocate();
    memcpy(list, save, length * sizeof(T));
    delete[] save;
}

template<typename T>
void NContainer<T>::ReallocateSafe(size_t length) {
    if (!list) {
        SafeAllocate(length);
        return;
    }
    size_t oldSize = GetLength();
    T* save = new T[oldSize];
    memcpy(save, list, oldSize * sizeof(T));
    Reallocate(length);
    memcpy(list, save, std::min(oldSize, length) * sizeof(T));
    delete[] save;
}

template<typename T>
void NContainer<T>::CopyData(const T* value, size_t size) {
    if (!list) {
        SafeAllocate(size);
    } else {
        Reallocate(size);
    }
    if (length && value && list) {
        memcpy(list, value, length * sizeof(T));
    }
}

template<typename T>
int NContainer<T>::Contains(const T& value) const {
    for (int i = 0; i < GetLength(); ++i) {
        if (list[i] == value) {
            return i;
        }
    }
    return -1;
}

template<typename T>
int NContainer<T>::Contains(const T* value) const {
    for (int i = 0; i < GetLength(); ++i) {
        if (&list[i] == value) {
            return i;
        }
    }
    return -1;
}

template<typename T>
bool NContainer<T>::Contains(const NContainer<T> value) const {
    for (size_t i = 0; i < value.GetLength(); ++i) {
        if (Contains(value[i]) < 0) {
            return false;
        }
    }
    return true;
}

template<typename T>
NContainer<T>::NContainer() {
    SetLength(0);
}

template<typename T>
NContainer<T>::NContainer(size_t length) {
    SafeAllocate(length);
}

template<typename T>
template<size_t N>
NContainer<T>::NContainer(const T (&value)[N]) {
    CopyData(value, N);
}

template<typename T>
NContainer<T>::NContainer(const NContainer& value) {
    if (&value == this) {
        //throw std::runtime_error("Self copy detected");
        return;
    }
    CopyData(value.GetValue(), value.GetLength());
}

template<typename T>
NContainer<T>::~NContainer() {
    Deallocate();
}

template<typename T>
template <size_t N>
NContainer<T>& NContainer<T>::Assign(const T (&value)[N]) {
    Deallocate();
    CopyData(value, N);
    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::Assign(const NContainer& value) {
    if (&value == this) {
        throw std::runtime_error("Self assign detected");
    }
    Deallocate();
    CopyData(value.GetValue(), value.GetLength());
    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::Append(const T& value, size_t index) {
    index = std::min(index, length);
	size_t newLength = length + 1;
    T* result = new T[newLength];
    memcpy(result, list, index * sizeof(T));
	result[index] = value;
	if (length > index) memcpy(result + index + 1, list + index, (length - index) * sizeof(T));
    Deallocate();
	SetLength(newLength);
    list = result;
    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::Remove(size_t index) {
    index = std::min(index, length);
	size_t newSize = length - 1;
    T* result = new T[newSize]; 
    memcpy(result, list, index * sizeof(T));
    if (length>index) memcpy(result + index, list + index + 1, (length - index - 1) * sizeof(T));
    Deallocate();
	SetLength(newSize);
	list = result;
    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::Replace(const T& value, size_t index) {
    if (length>0) {
        //index = std::min(index, length);
        if(index<length) list[index] = value;
    }
    //Else UB
    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::Append(const NContainer<T> value, size_t index) {
    index = std::min(index, length);
    if (&value == this) {
        //throw std::runtime_error("Self append detected");
    }
    size_t newLength = length + value.length;
    T* result = new T[newLength];

    memcpy(result, list, index * sizeof(T));
    memcpy(result + index, value.list, value.length * sizeof(T));
    memcpy(result + index + value.length, list + index, (length - index) * sizeof(T));
	
    Deallocate();
	SetLength(newLength);
	list = result;

    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::Remove(const NContainer<T> value, bool removeAll) {
    for (size_t i = 0; i < value.GetLength(); ++i) {
        do {
            int idx = Contains(value[i]);
            if (idx < 0) break;
            Remove(idx);
        } while (removeAll);
    }
    return *this;
}

template <typename T>
bool NContainer<T>::operator==(const NContainer<T>& other) const {
    if (length != other.GetLength()) return false;
    for (size_t i = 0; i < length; ++i) {
        if (list[i] != other[i]) return false;
    }
    return true;
}

template<typename T>
NContainer<T>& NContainer<T>::BubbleSort() {
    for (size_t i = 0; i < GetLength(); ++i) {
        for (size_t j = 0; j < GetLength(); ++j) {
            if (list[j] > list[i]) {
                T temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    return *this;
}

template<typename T>
NContainer<T>& NContainer<T>::InsertionSort() {
    for (size_t i = 0; i < GetLength(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (list[i] < list[j]) {
                T temp = list[i];
                list[i] = list[j];
                list[j] = temp;
                break;
            }
        }
    }
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const NContainer<T>& cont) {
    os << "{ ";
    for (size_t i = 0; i < cont.GetLength(); ++i) {
        os << cont[i] << " ";
    }
    os << "}";
    return os;
}

} // namespace NE

#endif // N_CONTAINER_INL