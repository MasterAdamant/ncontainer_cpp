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
        
        protected:
            void ReallocateSave(const size_t);

        public:
            NContainer();
            NContainer(const size_t);
            NContainer(const T*, const size_t);
            NContainer(const NContainer&);

            ~NContainer();

            NContainer& Assign(const T*, const size_t);
            NContainer& Assign(const NContainer&);

            const T* GetValue() const { return this->list; };
            const size_t GetLength() const { return this->length; };

            int Contains(const T) const;
            int Contains(const T*) const;
            bool Contains(const NContainer) const;

            NContainer& Append(const T, const size_t);
            NContainer& Remove(const size_t);
            NContainer& Replace(const T, const size_t);

            NContainer& Append(const NContainer<T>, const size_t);
            NContainer& Remove(const NContainer<T>);

            T& operator[](const size_t index) const {return this->list[index];};
            operator T*() const {return this->list;};
            T* ToArray(){return this->list;};
            void operator=(const T* value) const {this->Assign(value);};

            class Iterator{
                using Iterator_category = std::forward_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = T;
                using pointer           = T*;  // or also value_type*
                using reference         = T&;
                private:
                    size_t index=0;
                    pointer ptr;
                    const NContainer* m_container;
                public:
                    Iterator(pointer ptr, const NContainer<value_type>& container){
                        if(container.Contains(ptr)<0){
                            throw;
                        }
                        this->ptr=ptr;
                        this->index = container.Contains(ptr);
                        m_container = &container;
                    }
                    Iterator(const size_t index, const NContainer<value_type>& container){
                        if(index>container.GetLength()){
                            throw;
                        }
                        this->index = (index);
                        ptr = container.list + index;
                        m_container = &container;
                    }

                    size_t GetIndex() const { return index; };
                    const T* GetPointer() const { return ptr; };
                    NContainer* GetContainer() const { return m_container; };

                    reference operator=(const reference value) { *ptr=value.ptr; index=GetIndex(); return *this;};

                    reference operator*(){ return *ptr; }
                    pointer operator->() { return ptr; }

                    Iterator operator++() { ptr++; ++index; return *this; };
                    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; };
                    Iterator operator--() { ptr--; --index; return *this; };
                    Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; };

                    Iterator& operator+=(int value) {ptr+=value; index+=value; return *this;};
                    Iterator operator+(int value) const {return Iterator(index+value, *m_container);};
                    Iterator& operator-=(int value) {ptr-=value; index-=value; return *this;};
                    Iterator operator-(int value) const {return Iterator(index-value, *m_container);};
                    
                    bool operator== (const Iterator& b) const { return ptr == b.GetPointer(); };
                    bool operator!= (const Iterator& b) const { return ptr != *b; }; 
                    bool operator<(const Iterator& other) const { return other.GetIndex()<other.GetIndex();};
                    bool operator<=(const Iterator& other) const { return other.GetIndex()<=other.GetIndex();};
                    bool operator>=(const Iterator& other) const { return other.GetIndex()>=other.GetIndex();};
                    bool operator>(const Iterator& other) const { return other.GetIndex()>other.GetIndex();};
                    
                    ~Iterator(){};
            };
            Iterator begin(){
                return Iterator(&list[0]);
            }
            Iterator end(){
                return Iterator(&list[length-1]);
            }

            NContainer& BubbleSort(){
                for(int i=0; i<GetLength(); ++i){
                    for(int j=0; j<GetLength(); ++j){
                        if(list[j]>list[i]){
                            T temp = list[i];
                            list[i] = list[j];
                            list[j] = temp;
                        }
                    }
                }
                return *this;
            }
            NContainer& InsertionSort(){
                for(int i=0;i<GetLength(); ++i){
                    for(int j=0; j<i; ++j){
                        if(list[i]<list[j]){
                            T temp = list[i];
                            list[i] = list[j];
                            list[j] = temp;
                            break;
                        }
                    }
                }
                return *this;
            }
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
    void NContainer<T>::ReallocateSave(const size_t length){
        if(list==nullptr){
            SetLength(length);
            list = new T[capacity];
            return;
        }
        size_t oldSize = GetLength();
        T* save = new T[oldSize];
        for (size_t i=0; i<oldSize; ++i){
            save[i] = list[i];
        }
        SetLength(length);
        delete[] list;
        list = new T[capacity];
        for (size_t i=0; i<(oldSize < length)?oldSize:length; ++i){
            list[i] = save[i];
        }
        delete[] save;
    }
    // --- Почему в реаллокации не сохраняются данные? ---
    // Фукнции, расположеные в приватной секции, идут в порядке "увеличения безопасности".
    // Функция SetLength() обязывает разработчика реаллоцировать данные, передавая все в его
    // управление. Функция Reallocate() упрощает это и сама их реаллоцирует, оставляя воз-
    // можность разработчику или сохранить данные через CopyData(), или все очистить.
    // Upd.: Я решил реализовать сохранение данных при реаллокации в protected, так что
    // при БОЛЬШОМ желании, это все можно переписать.
    template<typename T>
    void NContainer<T>::CopyData(const T* value,const size_t length){
        Reallocate(length);
        for(size_t i = 0; i<length; ++i){
            this->list[i] = value[i];
        }
    }

    template<typename T>
    int NContainer<T>::Contains(const T value) const {
        for(int i=0; i<GetLength(); ++i){
            if(list[i] == value){
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    int NContainer<T>::Contains(const T* value) const {
        for(int i=0; i<GetLength(); ++i){
            if(&list[i] == value){
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    bool NContainer<T>::Contains(const NContainer<T> value) const {
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
        delete[] result;
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
    
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const NContainer<T>& cont) {
        os << "{ ";
        for(size_t i=0; i<cont.GetLength(); ++i){
            os << cont[i] << " ";
        }
        os << "}";
        return os;
    }
}


#endif //N_CONTAINER_HPP