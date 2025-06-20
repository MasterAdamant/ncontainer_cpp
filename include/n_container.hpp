#ifndef N_CONTAINER_HPP
#define N_CONTAINER_HPP

constexpr unsigned FOR_LIMIT = 65535;

#include <iostream>
#include <stdexcept>

namespace NE {

    using size_t = decltype(sizeof(0));

    template<typename T>
    class NContainer{
        private:
            size_t capacity = 0;
            size_t length = 0;
            T* list = nullptr;

            void SetLength(size_t);
            void RawAllocate();
            void SafeAllocate(size_t);
            void Deallocate();
            void Reallocate();
            void Reallocate(size_t);
            void CopyData(const T*, size_t);
        
        protected:
            void ReallocateSafe();
            void ReallocateSafe(size_t);

        public:
            NContainer();
            template<size_t N>
            NContainer(const T (&value)[N]);
            NContainer(size_t);
            NContainer(const NContainer&);

            ~NContainer();

            template <size_t N>
            NContainer& Assign(const T (&value)[N]);
            NContainer& Assign(const NContainer&);

            const T* GetValue() const { return this->list; };
            const size_t GetLength() const { return this->length; };

            int Contains(const T&) const;
            int Contains(const T*) const;
            bool Contains(const NContainer) const;

            NContainer& Append(const T&, size_t);
            NContainer& Remove(size_t);
            NContainer& Replace(const T&, size_t);

            NContainer& Append(const NContainer<T>, size_t);
            NContainer& Remove(const NContainer<T>, bool = true);

            T& operator[](size_t index) const {return this->list[index];};
            operator T*() const {return this->list;};
            template<size_t N>
            NContainer& operator=(const T(&value)[N]) { this->CopyData(value, N); return *this; };
            NContainer& operator=(const NContainer& value) { return this->Assign(value); };
            
            bool operator==(const NContainer& other) const;
            
            class Iterator{
                using Iterator_category = std::forward_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = T;
                using pointer           = T*;  // or also value_type*
                using reference         = T&;
                
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
                return Iterator(0, *this);
            }
            Iterator end(){
                return Iterator(length, *this);
            }

            NContainer& BubbleSort();
            NContainer& InsertionSort();
    };
    
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const NContainer<T>& cont);
    
}

#include "n_container.inl"

#endif //N_CONTAINER_HPP