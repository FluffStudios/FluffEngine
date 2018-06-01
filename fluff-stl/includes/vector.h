/*

	Copyright (c) 2018 Fluff Studios

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/

/*
    REVISION HISTORY

    AUTHOR          DATE            CHANGES
    Nick Clark      31-May-18       Initial define of vector object
 */

#include <iterator>

namespace fluff
{
    /**
     * Array backed vector implementation.  Supports insertion,
     * deletion, and modification of data values.
     * 
     * @param T             Type of stored object
     * @param Sz            Initial capacity of vector, defaults to 16
     * @param GrowthFactor  How much vector should increase in size 
     *                      reaching capacity, defaults to 2
     */
    template <typename T, size_t Sz = 16, float GrowthFactor = 2>
    class vector
    {
        size_t mCapacity_;
        size_t mSize_;
        T * mData_;
    public:
        /**
         * Internal iterator for vector
         */
        class iterator : public std::iterator<std::forward_iterator_tag, std::remove_cv_t<T>, std::ptrdiff_t, T*, T&>
        {
            T * mPtr_;
        public:
            explicit iterator(T* Pointer) : mPtr_(Pointer) { }

            iterator() : mPtr(nullptr) { }

            void swap(iterator & other) noexcept
            {
                std::swap(mPtr_, other.mPtr_);
            }

            iterator& operator++()
            {
                ++mPtr_;
                return *this;
            }

            iterator operator++(int)
            {
                iterator tmp(*this);
                ++mPtr_;
                return tmp;
            }

            template<class OtherType>
            bool operator == (const iterator<OtherType> & right) const
            {
                return mPtr_ == right.mPtr_;
            }

            template<class OtherType>
            bool operator != (const iterator<OtherType> & right) const
            {
                return mPtr_ != right.mPtr_;
            }

            T& operator*() const
            {
                return *mPtr_;
            }

            T& operator -> () const
            {
                return *mPtr_;
            }

            operator iterator<const Type>() const
            {
                return iterator<const Type>(mPtr_);
            }
        }

        /**
         * Creates new array backed storage object with default
         * capacity value
         */
        vector()
        {    
            this->mSize_ = 0;
            this->mCapacity_ = Capacity;
            this->mData_ = static_cast<T>(malloc(sizeof(Capacity * sizeof(T))));            
        }

        /**
         * Creates new array backed storage object with an initializer
         * list
         * 
         * @param Data      Initial data for vector
         */
        explicit vector(std::initializer_list<T> Data)
        {
            this->mSize_ = Data.size();
            while (this->mSize_ > this->mCapacity_) 
            {
                this->mCapacity_ *= GrowthFactor;
            }
            this->mData_ = static_cast<T*>(malloc(sizeof(Type) * this->mCapacity_));
            memcpy(mData_, Data.begin(), sizeof(T) * this->mSize_);
        }

        /**
         * Frees memory in vector and frees the values inside vector
         */
        ~vector()
        {
            clear();
        }

        /**
         * Inserts a new value into the vector
         * 
         * @param Value     Value to insert
         */
        void insert(const T & Value)
        {
            expand();
            this->mData_[this->mSize_++] = Value;
        }

        /**
         * Inserts a new value into the vector.  Uses move syntax
         * 
         * @param Value     Value to move into vector
         */
        void insert(T && Value)
        {
            expand();
            this->mData_[this->mSize_++] = std::move(Value);
        }

        /**
         * Inserts a new value into the vector at a given index.
         * 
         * @param Value     Value to insert
         * @param Index     Location in vector to insert into
         */
        void insert(const T & Value, size_t Index)
        {
            if (Index >= this->mSize_)
            {
                throw "Index out of bounds.";
            }
            else
            {
                expand();
                makehole(Index);
                this->mData_[this->mSize_++] = Value;
            }
        }

        /**
         * Inserts a new value into the vector at a given index. Uses move syntax
         * 
         * @param Value     Value to move 
         * @param Index     Location in vector to insert into
         */
        void insert(T && Value, size_t Index)
        {
            if (Index >= this->mSize_)
            {
                throw "Index out of bounds.";
            }
            else
            {
                expand();
                makehole(Index);
                this->mData_[this->mSize_++] = std::move(Value);
            }
        }

        /**
         * Removes a value from the vector by indexed value
         * 
         * @param Index     Index of element to remove
         * @return          Removed element 
         */
        T remove(size_t Index)
        {
            if (Index >= this->mSize_)
            {
                throw "Index out of bounds.";
            }
            else
            {
                T tmp = mData_[Index];
                fillgap(Index);
                this->mSize_--;
                return tmp;
            }
        }

        /**
         * Removes a value from the vector by value of element to remove
         * 
         * @param Value     Value to remove
         * @return          If successfully removed
         */
        bool remove(const T & Value)
        {
            for (auto i = 0; i < Size_; i++)
            {
                if (mData_[i] == Value)
                {
                    fillgap(i);
                    mSize_--;
                    return true;
                }
            }
            return false;
        }

        /**
         * Clears contents of vector
         */
        void clear()
        {
            for (size_t i = 0; i < mSize_; i++)
            {
                mData_[i].~T();
            }
            free(mData_);

            mSize_ = 0;
        }

        /**
         * Checks if vector's contents are empty
         * 
         * @return is empty
         */
        bool empty() const
        {
            return mSize_ == 0;
        }

        /**
         * Returns the contents of the vector
         * 
         * @return pointer to contents
         */
        T* data() const 
        { 
            return mData_; 
        }

        /**
         * Gets the number of elements in the vector
         * 
         * @return number of elements in vector
         */
        size_t size() const
        {
            return mSize_;
        }

        /**
         * Returns an iterator pointing to the first data value
         * in the vector
         * 
         * @return beginning data value
         */
        vector::iterator<T> begin() const
        {
            return iterator<T>(&mData_[0]);
        }

        /**
         * Returns an iterator pointing to the last data value
         * in the vector
         * 
         * @return ending data value
         */
        vector::iterator<T> end() const
        {
            return iterator<T>(&mData_[mSize_ - 1]);
        }
    private:
        void expand()
        {
            if (this->mSize_ == this->mCapacity_)
            {
                T* tmp = this->mData_;
                this->mData_ = static_cast<T*>(malloc(mCapacity_ * GrowthFactor));
                memcpy(mData_, tmp, mCapacity_);
                free(tmp);
                mCapacity_ *= GrowthFactor;
            }
        }

        void makehole(size_t Index)
        {
            memcpy(&mData_[Index + 1], &mData_[Index], (mSize_ - Index) * sizeof(T));
        }

        void fillgap(size_t Index)
        {
            memcpy(&mData_[Index], &mData_[Index + 1], (mSize_ - Index) * sizeof(T));
        }
    };
}