




template<class T>
class SharedPtr 
{
	T* ptr;
	int* counter;

public:
	~SharedPtr(){
		if (*counter == 1) {
			delete ptr;
			delete counter;
			ptr = nullptr;
			counter = nullptr;
		}
		else
		{
			ptr = nullptr;
			counter = nullptr;
			--counter;
		}
	}

	SharedPtr(){
		ptr = nullptr;
		counter = new int(1);
	}

	explicit SharedPtr(T* ptr){
		this.ptr = ptr;
		counter = new int(1);
	}

	SharedPtr(const std::nullptr_t ptr) {
		this->ptr = ptr;
		counter = new int(1);
	}

	SharedPtr(const SharedPtr& rhs) {
		ptr = rhs.ptr;
		counter = rhs.counter;
		++*counter;
	}

	SharedPtr(const SharedPtr&& rhs) {
		ptr = rhs.ptr;
		counter = rhs.counter;
		rhs.ptr = nullptr;
		rhs.counter = new int(1);
	}

	SharedPtr& operator=(const SharedPtr& other) {
		if (ptr == other.ptr) {
			return *this;
        }
		
		delete[] ptr;
		ptr = other.ptr;
		counter = other.counter;
		++* counter;
		return *this;
	}

	T& operator*() {   
		return *ptr;
	}

	T* operator->() {     
		return ptr;
	}

	explicit operator bool() const{
		return ptr != nullptr;
	}

	friend auto operator<=>(const SharedPtr& lhs, const SharedPtr& rhs) {
		auto lIt = lhs.ptr; auto rIt = rhs.ptr;		
		if (*lIt < *rIt)
			return std::strong_ordering::less;
		else if (*lIt > *rIt)
			return std::strong_ordering::greater;
		else
			return std::strong_ordering::equivalent;
	}

	friend auto operator<=>(const SharedPtr& lhs, const std::nullptr_t rhs) {
		auto lIt = lhs.ptr; auto rIt = rhs.ptr;
		if (*lIt < *rIt)
			return std::strong_ordering::less;
		else if (*lIt > *rIt)
			return std::strong_ordering::greater;
		else
			return std::strong_ordering::equivalent;
	}

	bool operator==(const SharedPtr& rhs) {
		return (this <=> rhs) == 0;
	}
	bool operator==(const std::nullptr_t rhs) {
		return (this <=> rhs) == 0;
	}
	bool operator!=(const SharedPtr& rhs) {		
		return (this <=> rhs) != 0;
	}
	bool operator!=( const std::nullptr_t rhs) {
		return (this <=> rhs) != 0;
	}

	void reset() {
		delete ptr;
		ptr = nullptr;
	}

	T* get() const{
		return ptr;
	}

	size_t use_count() {
		return *counter;
	}


	bool Invariant() {

	}








#pragma region nonmembers

	template<class T>
	friend void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs)noexcept
	{
		std::swap(lhs.ptr, rhs.ptr);
		std::swap(lhs.counter, rhs.counter);
	};

	template<class T, class...Args>
	friend SharedPtr<T>MakeShared(Args&&...args)
	{

	}

	
#pragma endregion nonmembers


};