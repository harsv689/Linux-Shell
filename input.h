#ifndef _INPUT_H_
#define _INPUT_H_

void die(const char *s);
void disableRawMode();
void enableRawMode();
char *read_input();
void predict(char *input, int *pt);

#endif