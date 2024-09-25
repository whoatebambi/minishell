#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

int safe_write(int fd, const char *str, ...)
{
    va_list     args;
    const char *current_str;
    int         bytes_written;
    size_t      i;
    
    va_start(args, str);
    current_str = str;
    while (current_str)
    {
        i = 0;
        while (i < strlen(current_str))
        {
            bytes_written = write(fd, &current_str[i], 1);
            if (bytes_written == -1)
            {
                perror("write() failed");
                va_end(args);
                return (-1);
            }
            i++;
        }
        current_str = va_arg(args, const char *);
    }
    va_end(args);
    return (0);
}

int main() {
    int excode;
    int fd_out = STDOUT_FILENO;
    int fd_in = STDIN_FILENO;

    excode = safe_write(fd_out, "hello\n", NULL);
    printf("excode: %d\n", excode);

    excode = safe_write(fd_out, "hello", " world!\n", NULL);
    printf("excode: %d\n", excode);

    excode = safe_write(fd_out, "aaa ", "bbb", " ccc!\n", NULL);
    printf("excode: %d\n", excode);

    excode = safe_write(fd_out, "", NULL);
    printf("excode: %d\n", excode);

    return 0;
}