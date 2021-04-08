// #includes omitted!
// Please read the man pages for the functions.
#include "secret_headers.h"

void getAndPrintGroup(gid_t grpNum)
{
    struct group *grp = getgrgid(grpNum);

    if (grp) {
        printf(" %-8s", grp->gr_name);
    } else {
        printf(" ERR: GROUP");
    }
}

void getAndPrintUserName(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        printf(" %-8s", pw->pw_name);
    } else {
        perror(" ERR: USER");
    }
}

// int main()
// {
//     getAndPrintGroup(1001);
//     getAndPrintGroup(514378);
//     getAndPrintGroup(103);
//     getAndPrintGroup(1000);

//     getAndPrintUserName(59894);
//     getAndPrintUserName(23524);
//     getAndPrintUserName(20746);
//     getAndPrintUserName(5970);
//     getAndPrintUserName(10485);

//     return 0;
// }
