#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int s_push(struct node **top, int data)
{
	struct node *new = malloc(sizeof(struct node));

	if (!new)
		return -1;

	new->data = data;
	new->next = *top;
	*top = new;

	return 0;
}

int s_pop(struct node **top, int *out)
{
	struct node *tmp;

	if (!*top)
		return -1;

	*out = (*top)->data;
	tmp = *top;
	*top = (*top)->next;

	free(tmp);
	return 0;
}

void s_print(struct node **top)
{
	struct node *tmp = *top;
	printf("[ ");
	while (tmp != NULL) {
		printf("%d ", tmp->data);
		tmp = tmp->next;
	}
	printf("]\n");
}

int s_peek(struct node **top, int *out)
{
	if (!*top)
		return -1;
	*out = (*top)->data;
	return 0;
}

void s_free(struct node **top)
{
	int out;
	/* Pop already frees the node, so pop everything until returns -1 */
	while (!s_pop(top, &out));
}
