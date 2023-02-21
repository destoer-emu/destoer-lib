
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