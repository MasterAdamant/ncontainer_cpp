#ifndef N_CONTAINER_HPP
#define N_CONTAINER_HPP

#define FOR_LIMIT 65535

#include <iostream>

namespace NE{

    using size_t = decltype(sizeof(0));
/*
    template <typename T>
    
    size_t GetLength(T* value){
        return std::size(value);
    };
    */

    template<typename T>
    class NContainer{
        private:
            size_t capacity = 0;
            size_t length = 0;
            T* list = nullptr;

            void SetLength(const size_t);
            void Reallocate(const size_t);
            void CopyData(const T*, const size_t);

        public:
            NContainer();
            NContainer(const size_t);
            NContainer(const T*, const size_t);
            NContainer(const NContainer&);

            ~NContainer();

            NContainer& Assign(const T*, const size_t);
            NContainer& Assign(const NContainer&);

            inline const T* GetValue() const {
                return this->list;
            };
            inline const size_t GetLength() const {
                return this->length;
            };

            int Contains(const T);
            bool Contains(const NContainer);

            NContainer& Append(const T, const size_t);
            NContainer& Remove(const size_t);
            NContainer& Replace(const T, const size_t);

            NContainer& Append(const NContainer<T>, const size_t);
            NContainer& Remove(const NContainer<T>);

            T& operator[](const size_t index) const {return this->list[index];};
            operator T*() const {return this->list;};
            T* ToArray(){return this->list;};
            void operator=(const T* value) const {this->Assign(value);};

            
    };

    template<typename T>
    void NContainer<T>::SetLength(const size_t value){
        size_t size = sizeof(T);
        this->length = value;
        this->capacity = this->length;
        while(size*(this->capacity)%4!=0){
            ++(this->capacity);
        }
        
        return;
    }

    template<typename T>
    void NContainer<T>::Reallocate(const size_t length){
        if(list!=nullptr){
            delete[] list;
        }
        SetLength(length);
        list = new T[capacity];
    }


    template<typename T>
    void NContainer<T>::CopyData(const T* value,const size_t length){
        Reallocate(length);
        for(size_t i = 0; i<length; ++i){
            this->list[i] = value[i];
        }
    }

    template<typename T>
    int NContainer<T>::Contains(const T value){
        for(int i=0; i<GetLength(); ++i){
            if(list[i] == value){
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    bool NContainer<T>::Contains(const NContainer<T> value){
        for(size_t i=0; i<value.GetLength(); ++i){
            if(Contains(value[i])<0){
                return false;
            }
        }
        return true;
    }
    template<typename T>
    NContainer<T>::NContainer(){
        list = new T[0];
        CopyData(list, 0);
    }

    template<typename T>
    NContainer<T>::NContainer(const size_t length){
        Reallocate(length);
    }

    template<typename T>
    NContainer<T>::NContainer(const T* value, const size_t length){
        CopyData(value, length);
    }

    template<typename T>
    NContainer<T>::NContainer(const NContainer& value){
        if(&value == this){
            throw;
        }
        CopyData(value.GetValue(), value.GetLength());
    }

    template<typename T>
    NContainer<T>::~NContainer(){
        if(list!=nullptr){
            delete[] list;
        }
    }

    template<typename T>
    NContainer<T>& NContainer<T>::Assign(const T* value, const size_t length){
        CopyData(value, length);
        return *this;
    }

    template<typename T>
    NContainer<T>& NContainer<T>::Assign(const NContainer& value){
        if(&value == this){
            throw;
        }
        CopyData(value.GetValue(), value.GetLength());
        return *this;
    }

    template<typename T>
    NContainer<T>& NContainer<T>::Append(const T value, const size_t index){
        NContainer<T> save = NContainer<T>(*this);
        Reallocate(GetLength()+1);
        for(int i=0,t=0; i<GetLength(); ++i, ++t){
            if(i==index){
                this->list[i++] = value;
            }
            this->list[i] = save[t];
        }
        return *this;
    }

    template<typename T>
    NContainer<T>& NContainer<T>::Remove(const size_t index){
        T* result = new T[GetLength()-1];
        for(int i=0,t=0; i<GetLength()-1; ++i, ++t){
            if(i==index){
                ++t;
            }
            result[i] = list[t];
        }
        Assign(result, GetLength()-1);
        delete result;
        return *this;
    }

    template<typename T>
    NContainer<T>& NContainer<T>::Replace(const T value, const size_t index){
        list[index]=value;
        return *this;
    }
    
    template<typename T>
    NContainer<T>& NContainer<T>::Append(const NContainer<T> value, const size_t index){
        if(&value == this){
            throw;
        }
        NContainer<T> save = NContainer<T>(*this);
        Reallocate(this->GetLength()+value.GetLength());
        for(int i=0,t=0; i<this->GetLength(); ++i, ++t){
            if(i==index){
                for(;i<value.GetLength()+index;++i){
                    this->list[i] = value[i-index];
                }
            }
            this->list[i] = save[t];
        }
        return *this;
    }

    
    template<typename T>
    NContainer<T>& NContainer<T>::Remove(const NContainer value){
        for (size_t i=0; i<value.GetLength(); ++i)
        {
            while(true){
                if(Contains(value[i])<0){
                    break;
                }
                Remove(Contains(value[i]));
            }
        }
        return *this;
    }
    
    
}


#endif //N_CONTAINER_HPP