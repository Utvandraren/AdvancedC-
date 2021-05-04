

#define CHECK assert(Invariant());


template<class T>
class SharedPtr 
{
	T* _ptr;
	int* counter;


public:
	~SharedPtr(){
		if (*counter == 1) {
			delete _ptr;
			delete counter;
			_ptr = nullptr;
			counter = nullptr;
		}
		else
		{
			_ptr = nullptr;
			counter = nullptr;
			--counter;
		}
	}

	SharedPtr(){
		_ptr = nullptr;
		counter = new int(1);
	}

	explicit SharedPtr(T* ptr){
		ptr = ptr;
		counter = new int(1);
	}

	SharedPtr(nullptr_t ptr) {
		_ptr = ptr;
		counter = new int(1);
	}

	SharedPtr( SharedPtr& rhs) {
		_ptr = rhs._ptr;
		counter = rhs.counter;
		++* counter;

	}

	SharedPtr( SharedPtr&& rhs) {
		_ptr = rhs._ptr;
		counter = rhs.counter;
		rhs._ptr = nullptr;
		rhs.counter = new int(1);
	}

	SharedPtr& operator=(const SharedPtr& other) {
		if (_ptr == other._ptr) {
			return *this;
        }		
		delete[] _ptr;
		_ptr = other._ptr;
		counter = other.counter;
		++* counter;
		return *this;
	}

	T& operator*() {   
		return *_ptr;
	}

	T* operator->() {     
		return _ptr;
	}

	explicit operator bool() const{
		return _ptr != nullptr;
	}

	friend auto operator<=>(const SharedPtr& lhs, const SharedPtr& rhs) {
		auto lIt = lhs._ptr; auto rIt = rhs._ptr;		
		if (*lIt < *rIt)
			return std::strong_ordering::less;
		else if (*lIt > *rIt)
			return std::strong_ordering::greater;
		else
			return std::strong_ordering::equivalent;
	}

	friend auto operator<=>(const SharedPtr& lhs, const nullptr_t rhs) {
		auto lIt = lhs._ptr; auto rIt = rhs.ptr;
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
	bool operator==(const nullptr_t rhs) {
		return (this <=> rhs) == 0;
	}
	bool operator!=(const SharedPtr& rhs) {		
		return (this <=> rhs) != 0;
	}
	bool operator!=( const nullptr_t rhs) {
		return (this <=> rhs) != 0;
	}

	void reset() {
		delete _ptr;
		_ptr = nullptr;
	}

	T* get() const{
		return _ptr;
	}

	size_t use_count() {
		return *counter;
	}


	bool Invariant() {
		assert(*counter != nullptr);
	}

#pragma region nonmembers

	template<class T>
	friend void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept
	{
		std::swap(lhs._ptr, rhs._ptr);
		std::swap(lhs.counter, rhs.counter);
	}

#pragma endregion nonmembers

};

template<class T, class...Args>
static SharedPtr<T>MakeShared(Args&&...args)
{	
	return SharedPtr<T>(new T(args...));
}
