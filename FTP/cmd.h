#ifndef _CMD_H_
#define _CMD_H_

size_t parsecmd(char * args[], char cmd[]);

void freecmd(char * args[], size_t n);

void get(int connfd);

#endif // !_CMD_H_