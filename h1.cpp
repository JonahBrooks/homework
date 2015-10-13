#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#define BLOCK_SIZE 30

int main(int argc, char *args[]) {
    
    int fd;
    int tmp_fd;
    int out_fd;

    int number_read;

    char buffer[BLOCK_SIZE];
    
    fd = open( "h1.cpp", O_RDONLY );

    tmp_fd = open( "copy", O_RDONLY );
    close( tmp_fd );
    if ( tmp_fd != -1 ) {
        std::cout << "Error, file already exists!" << std::endl;
    } else {
        out_fd = creat( "copy", S_IRWXO | S_IRWXU );
        
        number_read = read(fd, buffer, BLOCK_SIZE);
        
        while ( number_read != 0 ) {
            write( out_fd, buffer, BLOCK_SIZE );
            std::cout << buffer;
            number_read = read(fd, buffer, BLOCK_SIZE);
            if ( number_read < BLOCK_SIZE ) {
                buffer[number_read] = '\0';
            }
        }

    }

    close( out_fd );    
    close(fd);

    std::cout << std::endl << "Moo" << std::endl << std::endl;
    std::cout << buffer << std::endl;
    return 0;
}
