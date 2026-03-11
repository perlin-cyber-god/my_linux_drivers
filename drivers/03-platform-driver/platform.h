#ifndef PLATFORM_H
#define PLATFORM_H

#define RDWR 0x11
#define WRONLY 0x10

struct pcdev_platform_data {
    int size;
    int perm;
    const char *serial_number;
};

#endif /* PLATFORM_H */
