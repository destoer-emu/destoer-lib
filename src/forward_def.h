#pragma once 

// provided for back-compat with older projects

// stl helpers
b32 read_bin(const std::string& filename, std::vector<u8>& buf);
b32 write_bin(const std::string &filename, std::vector<uint8_t> &buf);
std::pair<std::vector<std::string>,b32> read_dir_tree(const std::string &file_path);
std::string name_from_path(const std::string& name);
std::vector<std::string> filter_ext(const std::vector<std::string> &files,const std::string &str);

// is there a nicer way to do this?
size_t get_remaining_ifstream_size(std::ifstream &fp);


std::string remove_ext(const std::string &str);
std::string get_save_file_name(const std::string &filename);


#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void unimplemented(const char *fmt, ...);

#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void crash_and_burn(const char *fmt, ...);
void log(b32 cond,const char *fmt, ...);
