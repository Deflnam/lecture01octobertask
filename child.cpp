#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstdlib>

size_t recv_msg(int& err, int rd, char* b, size_t k)
{
    err = 0;
    size_t total = 0;
    while(total < k)
    {
        ssize_t n = read(rd, b + total, k - total);
        if( n <= 0 )
        {
          err = -1;
          return total;
        
        }
        total += (size_t)n;
    }
    return total;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
      std::cerr << "Usage: child <fd>" << std::endl;
      return 1;
    }
    
    int fd = std::atoi(argv[1]);
    if (fd <= 0)
    {
      std::cerr << "Invalid fd" << std::endl;
      return 1;
    }
    
    int err = 0;
    size_t size = 0;
    
    size_t n = recv_msg(err, fd, (char*)(&size), sizeof(size));
    if (err < 0 || n != sizeof(size))
    {
        std::cerr << "Failed to read size!" << std::endl;
        close(fd);
        return 1;
    }
    
    std::vector<char> msg(size + 1);
    n = recv_msg(err, fd, msg.data(), size);
    if (err < 0)
    {
        std::cerr << "Failed to read message!" << std::endl;
        close(fd);
        return 1;
    }
    
    msg[size] = '\0';
    std::cout << "The child process received: " << msg.data() << std::endl;
    
    close(fd);
    return 0;
}