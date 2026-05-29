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

int s_pop(struct node **top)
{
	int ret_data;
	struct node *tmp;

	if (!*top)
		return -1;

	ret_data = (*top)->data;
	tmp = *top;
	*top = (*top)->next;

	free(tmp);
	return ret_data;
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

int s_peek(struct node **top)
{
	if (!*top)
		return -1;
	return (*top)->data;
}

void s_free(struct node **top)
{
	/* Pop already frees the node, so pop everything until reach -1 */
	while (s_pop(top) != -1);
}
