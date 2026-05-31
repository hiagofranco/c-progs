#include <stdio.h>
#include <stdlib.h>

/* Generic linked list - Doubly linked list with void* to store any generic
 * type, with insertion, remove and search functions */

struct element {
	void *data;
	struct element *next;
	struct element *prev;
};

int size_of_list(struct element *list)
{
	int count = 0;
	struct element *tmp = list;

	while (tmp != NULL) {
		tmp = tmp->next;
		count++;
	}
	return count;
}

int insert_list(struct element **list, void *data, int pos)
{
	struct element *new, *aux;
	int size = size_of_list(*list);

	/* "pos" can not be greater than the size of the list, otherwise the
	 * position is invalid */
	if (pos > size)
		return -1;

	new = malloc(sizeof(struct element));
	if (!new)
		return -1;
	new->data = data;

	/* Empty list */
	if (size == 0) {
		new->next = NULL;
		new->prev = NULL;
		*list = new;
	}
	/* Start of list */
	else if (pos == 0) {
		new->next = *list;
		new->prev = NULL;
		(*list)->prev = new;
		*list = new;
	}
	/* End of list */
	else if (pos == size) {
		aux = *list;
		while (aux->next != NULL)
			aux = aux->next;
		new->next = NULL;
		new->prev = aux;
		aux->next = new;
	}
	/* Elsewhere */
	else {
		aux = *list;
		for (int i = 0; i < pos; i++)
			aux = aux->next;
		new->next = aux;
		new->prev = aux->prev;
		aux->prev = new;
		(new->prev)->next = new;
	}
	return 0;
}

int search_list(struct element *list, void **data, int pos)
{
	int size = size_of_list(list);
	if (pos >= size)
		return -1;

	struct element *aux = list;
	for (int i = 0; i < pos; i++)
		aux = aux->next;
	*data = aux->data;
	return 0;
}

int remove_from_list(struct element **list, int pos)
{
	struct element *aux;
	int size = size_of_list(*list);

	if (pos > size)
		return -1;

	aux = *list;
	if (pos == 0) {
		*list = (*list)->next;
		(*list)->prev = NULL;
	}
	else if (pos == size - 1) {
		while (aux->next != NULL)
			aux = aux->next;
		(aux->prev)->next = NULL;
	}
	else {
		for (int i = 0; i < pos; i++)
			aux = aux->next;
		(aux->next)->prev = aux->prev;
		(aux->prev)->next = aux->next;
	}
	free(aux);
	return 0;
}

void print_list(struct element *list)
{
	/* Since these are generic types, let's just print the elements as
	 * 'EX', since we do not know what we are storing  */
	int size = size_of_list(list);

	if (size == 0) {
		printf("[ ]\n");
		return;
	}

	printf("[ ");
	for (int i = 0; i < size - 1; i++)
		printf("E%d->", i);
	printf("E%d ]\n", size - 1);
}

void free_list(struct element *list)
{
	/* Here we are ignoring the return value from remove_from_list since
	 * i is already going until size_of_list */
	for (int i = 0; i < size_of_list(list); i++)
		remove_from_list(&list, i);
}

int main (void)
{
	struct element *list = NULL;

	/* Testing */
	int data0 = 0;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;
	int data5 = 5;
	int data6 = 6;
	int data7 = 7;
	int data8 = 8;
	int data9 = 9;

	if (insert_list(&list, &data0, 0) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data1, 1) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data2, 2) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data3, 3) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data4, 4) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data5, 0) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data6, 1) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data7, 2) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data8, 3) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}
	if (insert_list(&list, &data9, 4) < 0) {
		printf("ERROR: Could not allocate new element.\n");
		return -1;
	}

	void *data_out;
	for (int i = 0; i < 10; i++) {
		if (search_list(list, &data_out, i) < 0) {
			printf("ERROR: search_list error.\n");
			break;
		}
		/* derefence first, and then access the value with the
		 * first '*' */
		printf("%d\n", *(int *) data_out);
	}

	free_list(list);
	list = NULL;

	return 0;
}
