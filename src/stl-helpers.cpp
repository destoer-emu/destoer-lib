
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