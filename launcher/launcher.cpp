#include"component.h"
#include "gebo.h"
#include<boost/filesystem.hpp> 
int main() {
    namespace bf = boost::filesystem;
    bf::path path("/tmp/test");
    bf::path current_cpath = bf::current_path(); //取得当前目录 
    return 0;
}
