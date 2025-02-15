constexpr char path_separator = std::filesystem::path::preferred_separator;

size_t get_remaining_ifstream_size(std::ifstream &fp);

template<typename access_type>
inline void handle_write(std::vector<u8> &buf,u32 addr,access_type v)
{
	#ifdef BOUNDS_CHECK // bounds check the memory access (we are very screwed if this happens)
		if(buf.size() < addr + sizeof(access_type))
		{
			auto err = fmt::format("out of range handle write at: {:08x}:{:08x}\n",addr,buf.data());
			throw std::runtime_error(err);
		}
	#endif



	//(*(access_type*)(buf.data()+addr)) = v;
    handle_write(&buf[addr],v);
}


// need checks for endianess here for completeness
template<typename access_type>
inline access_type handle_read(std::vector<u8> &buf,u32 addr)
{

	#ifdef BOUNDS_CHECK // bounds check the memory access (we are very screwed if this happens)
		if(buf.size() < addr + sizeof(access_type))
		{
			auto err = fmt::format("out of range handle read at: {:08x}:{:08x}\n",addr,buf.data());
			throw std::runtime_error(err);
		}
	#endif


	//return(*(access_type*)(buf.data()+addr));
    return handle_read<access_type>(&buf[addr]);
}


// helpers for dumping data to and from binary files
template<typename T>
inline void file_write_var(std::ofstream &fp, const T &data)
{
	fp.write(reinterpret_cast<const char*>(&data),sizeof(T));
}

template<typename T>
inline void file_read_var(std::ifstream &fp, T &data)
{
	auto sz = get_remaining_ifstream_size(fp);
	if(sz < sizeof(T))
	{
		throw std::runtime_error("file_read_var error");
	}
	fp.read(reinterpret_cast<char*>(&data),sizeof(T));
}

template<typename T>
inline void file_write_arr(std::ofstream &fp, const T *data,size_t size)
{
	fp.write(reinterpret_cast<const char*>(data),size);
}

template<typename T>
inline void file_read_arr(std::ifstream &fp, T *data, size_t size)
{
	auto sz = get_remaining_ifstream_size(fp);
	if(sz < size)
	{
		throw std::runtime_error("file_read_arr error");
	}	
	fp.read(reinterpret_cast<char*>(data),size);
}

inline b32 file_exists(const char* name)
{
	FILE* fp = fopen(name,"r");

	if(!fp)
	{
		return false;
	}

	fclose(fp);
	return true;
}

template<typename T>
inline void file_write_vec(std::ofstream &fp, const std::vector<T> &buf)
{
	fp.write(reinterpret_cast<const char*>(buf.data()),sizeof(T)*buf.size());
}

template<typename T>
inline void file_read_vec(std::ifstream &fp,std::vector<T> &buf)
{
	auto sz = get_remaining_ifstream_size(fp);
	if(sz < sizeof(T) * buf.size())
	{
		throw std::runtime_error("file_read_vec error");
	}		
	fp.read(reinterpret_cast<char*>(buf.data()),sizeof(T)*buf.size());
}
