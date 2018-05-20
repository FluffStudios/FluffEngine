#pragma once

#define LUMINOS_ARRAYLIST_DEFAULT_CAPACITY 16

#include <iterator>
#include <debug.h>

namespace luminos
{
	template<class T>
	class LUMINOS_API iterator : public std::iterator<std::forward_iterator_tag, std::remove_cv_t<T>, std::ptrdiff_t, T*, T&>
	{
	public:
		explicit iterator(T* Pointer) : ptr_(Pointer) { }
		iterator() :ptr_(nullptr) { }
		void swap(iterator& other) noexcept
		{
			std::swap(ptr_, other.ptr_);
		}

		iterator& operator++()
		{
			++ptr_;
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp(*this);
			++ptr_;
			return tmp;
		}

		template<class OtherType>
		bool operator == (const iterator<OtherType> &right) const
		{
			return ptr_ == right.ptr_;
		}

		template<class OtherType>
		bool operator != (const iterator<OtherType> &right) const
		{
			return ptr_ != right.ptr_;
		}

		T& operator*() const
		{
			return *ptr_;
		}

		T& operator -> () const
		{
			return *ptr_;
		}

		operator iterator<const T>() const
		{
			return iterator<const T>(ptr_);
		}
	private:
		T* ptr_;
	};

	template<class Type>
	class LUMINOS_API arraylist
	{
		size_t Size_ = 0;
		size_t Capacity_ = LUMINOS_ARRAYLIST_DEFAULT_CAPACITY;
		Type *Data_;
	public:
		explicit arraylist()
		{
			Size_ = 0;
			Data_ = static_cast<Type*>(malloc(sizeof(Type) * Capacity_));
		}

		explicit arraylist(size_t InitialCapacity)
		{
			Capacity_ = InitialCapacity;
			Size_ = 0;
			Data_ = static_cast<Type*>(malloc(sizeof(Type) * Capacity_));
		}

		explicit arraylist(std::initializer_list<Type> Initializer)
		{
			Size_ = Initializer.size();
			while (Size_ > Capacity_) Capacity_ *= 2;
			Data_ = static_cast<Type*>(malloc(sizeof(Type) * Capacity_));
			memcpy(Data_, Initializer.begin(), sizeof(Type) * Initializer.size());
		}

		void insert(const Type& Value)
		{
			if (Size_ == Capacity_) Expand();
			Data_[Size_++] = Value;
		}

		void insert(Type&& Value)
		{
			if (Size_ == Capacity_) Expand();
			Data_[Size_++] = std::move(Value);
		}

		void insert(const Type& Value, size_t Index)
		{
			LUMINOS_ASSERT(Index < Size_)
				if (Size_ == Capacity_) Expand();
			MakeHole(Index);
			Data_[Index] = Value;
			Size_++;
		}

		void insert(Type&& Value, size_t Index)
		{
			LUMINOS_ASSERT(Index < Size_)
				if (Size_ == Capacity_) Expand();
			MakeHole(Index);
			Data_[Index] = std::move(Value);
			Size_++;
		}

		void insert(const arraylist<Type>& Values)
		{
			for (auto i : Values)
			{
				insert(i);
			}
		}

		void insert(const arraylist<Type>& Values, size_t Index)
		{
			LUMINOS_ASSERT(Index < Size_)
				for (const auto i : Values)
				{
					insert(std::move(i), Index++);
				}
		}

		void insert(const Type* Values, size_t Count)
		{
			for (auto i = 0; i < Count; i++)
			{
				insert(std::move(*(Values + i)));
			}
		}

		void insert(const Type* Values, size_t Count, size_t Index)
		{
			LUMINOS_ASSERT(Index < Size_)
				for (auto i = 0; i < Count; i++)
				{
					insert(std::move(*(Values + i)), Index + i);
				}
		}

		Type remove(size_t Index)
		{
			LUMINOS_ASSERT(Index < Size_)
				Type tmp = Data_[Index];
			FillGap(Index);
			Size_--;
			return tmp;
		}

		bool remove(const Type& Value)
		{
			for (auto i = 0; i < Size_; i++)
			{
				if (Data_[i] == Value)
				{
					FillGap(i);
					Size_--;
					return true;
				}
			}
			return false;
		}

		void clear()
		{
			memset(Data_, 0, sizeof(Type) * Size_);
			Size_ = 0;
		}

		bool empty() const
		{
			return Size_ == 0;
		}

		inline Type* data() const { return Data_; }

		Type& operator[](size_t Index)
		{
			return Data_[Index];
		}

		iterator<Type> find(const Type& Object)
		{
			for (auto i = 0; i < Size_; i++)
			{
				if (i == Object) return iterator<Type>(&Data_[i]);
			}
			return end();
		}

		Type set(const Type& Object, size_t Index)
		{
			LUMINOS_ASSERT(Index < Size_)
				Type tmp = Data_[Index];
			Data_[Index] = Object;
			return tmp;
		}

		template<class... Args>
		void emplace(size_t Index, Args&&... args)
		{
			this->insert(static_cast<Type>(args)..., Index);
			Size_++;
		}

		inline size_t size() const { return Size_; }

		iterator<Type> begin() const
		{
			return iterator<Type>(&Data_[0]);
		}

		iterator<Type> end() const
		{
			iterator<Type> it(&Data_[Size_]);
			return it;
		}
	private:
		void Expand()
		{
			Type* tmp = Data_;
			Data_ = static_cast<Type*>(malloc(2 * Capacity_ * sizeof(Type)));
			memcpy(Data_, tmp, Capacity_ * sizeof(Type));
			Capacity_ *= 2;
			free(tmp);
		}

		void MakeHole(size_t Index)
		{
			memcpy(&Data_[Index + 1], &Data_[Index], (Size_ - Index) * sizeof(Type));
		}

		void FillGap(size_t Index)
		{
			memcpy(&Data_[Index], &Data_[Index + 1], (Size_ - Index) * sizeof(Type));
		}
	};

}
