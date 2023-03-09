
b32 read_bin(const std::string& filename, std::vector<u8>& buf)
{
    std::ifstream fp(filename,std::ios::binary);

    if(!fp)
    {
        return true;
    }



    fp.seekg(0,std::ios::end);
    size_t size = fp.tellg();
    fp.seekg(0,std::ios::beg);

    fp.clear();

    buf.resize(size);

    fp.read((char*)buf.data(),size);

    return false;
}

b32 write_bin(const std::string &filename, std::vector<uint8_t> &buf)
{
    std::fstream fp(filename,std::ios::out | std::ios::binary);

    if(!fp)
    {
        return true;
    }


    fp.write((char*)buf.data(),buf.size());

    return false;
}

// pull every file from every directory recursively
std::pair<std::vector<std::string>,b32> read_dir_tree(const std::string &file_path)
{
    std::vector<std::string> tree;

    if(!std::filesystem::is_directory(file_path))
    {
        return std::pair<std::vector<std::string>,b32>(tree,true);
    }


    std::vector<std::string> path_list;
    path_list.push_back(file_path);

    while(path_list.size() != 0)
    {
        auto dir = path_list.back(); path_list.pop_back();
        for(const auto &x: std::filesystem::directory_iterator(dir))
        {
            const auto path_str = x.path().string();

            if(std::filesystem::is_regular_file(x))
            {
                tree.push_back(path_str);
            }

            else if(std::filesystem::is_directory(x))
            {
                path_list.push_back(path_str);
            }
        }
    }

    return std::pair<std::vector<std::string>,b32>(tree,false);
}

std::string name_from_path(const std::string& name)
{
    return std::filesystem::path(name).filename().string();
}

std::vector<std::string> filter_ext(const std::vector<std::string> &files,const std::string &str)
{
    std::vector<std::string> res;

    for(const auto &x : files)
    {
        size_t ext_idx = x.find_last_of("."); 
        if(ext_idx != std::string::npos)
        {
            std::string ext = x.substr(ext_idx+1);

            if(ext == str)
            {
                res.push_back(x);
            }

        }
    }

    return res;
}


// is there a nicer way to do this?
size_t get_remaining_ifstream_size(std::ifstream &fp)
{
	auto cur = fp.tellg();
	fp.seekg(0,fp.end);
	auto sz = fp.tellg();
	fp.clear();
	fp.seekg(cur,fp.beg);
	return sz - cur;
}



std::string remove_ext(const std::string &str)
{
	std::string ans = str;

	size_t ext_idx = ans.find_last_of("."); 
	if(ext_idx != std::string::npos)
	{
		ans = ans.substr(0, ext_idx); 	
	}
	return ans;	
}

std::string get_save_file_name(const std::string &filename)
{
	return remove_ext(filename) + ".sav";
}

#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void unimplemented(const char *fmt, ...)
{
    printf("unimplemented: ");
    va_list args; 
    va_start(args, fmt);
    vprintf(fmt,args);
    putchar('\n');
    va_end(args);
    exit(1);
}

#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void crash_and_burn(const char *fmt, ...)
{
    printf("panic: ");
    va_list args; 
    va_start(args, fmt);
    vprintf(fmt,args);
    va_end(args);
    putchar('\n');
    exit(1);     
}


void log(b32 cond,const char *fmt, ...)
{
    if(cond)
    {
        va_list args; 
        va_start(args, fmt);
        vprintf(fmt,args);
        va_end(args);
    }
}
