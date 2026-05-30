#ifndef STACK_H
#define STACK_H

struct node {
	int data;
	struct node *next;
};

int s_push(struct node **top, int data);
int s_pop(struct node **top, int *out);
void s_print(struct node **top);
int s_peek(struct node **top, int *out);
void s_free(struct node **top);

#endif
