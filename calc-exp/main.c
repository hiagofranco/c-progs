#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define MAX_INPUT_SIZE 100

struct token {
	int data;
	char is_operator;
};

int c_weight(int operator)
{
	switch (operator) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		default:
			return 0;
	}
}

int add_token_to_list(struct token **list, int *list_count, int number, char is_operator)
{
	struct token *t_tmp;

	t_tmp = realloc(*list, (*list_count + 1) * sizeof(struct token));
	if (!t_tmp)
		return -1;

	*list = t_tmp;
	(*list)[*list_count].data = number;
	(*list)[*list_count].is_operator = is_operator;
	(*list_count)++;

	return 0;
}

int main(void)
{
	struct token *t_list = NULL;
	struct token *t_output = NULL;
	struct node *stack = NULL;

	int i = 0, t_list_count = 0, number = 0;

	char input[MAX_INPUT_SIZE];

	/* Step1: Get input expression from stdin */
	printf("> ");
	if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
		fprintf(stderr, "ERROR: could not read from stdin.\n");
		return -1;
	}

	/* Step2: Convert input to list of tokens */
	while(input[i] != '\n' && input[i] != '\0') {
		if (input[i] == ' ') {
			i++;
			continue;
		}
		if (input[i] >= '0' && input[i] <= '9') {
			while (input[i] >= '0' && input[i] <= '9') {
				number = number * 10 + (input[i] - '0');
				i++;
			}
			if (add_token_to_list(&t_list, &t_list_count, number, 0) < 0) {
				fprintf(stderr, "ERROR: could not allocate token \'%d\'.\n", number);
				free(t_list);
				return -1;
			}
			number = 0;
		}
		else {
			switch (input[i]) {
				case '+':
				case '-':
				case '*':
				case '/':
				case '(':
				case ')':
					if (add_token_to_list(&t_list, &t_list_count, input[i], 1) < 0) {
						free(t_list);
						fprintf(stderr, "ERROR: could not allocate token \'%c\'.\n", input[i]);
						return -1;
					}
					break;
				default:
					fprintf(stderr, "ERROR: invalid char \'%c\'.\n", input[i]);
					if (t_list)
						free(t_list);
					return -1;
					break;
			}
			i++;
		}
	}

	int t_output_count = 0, peek, popped_op;

	/*  Step 3: Convert tokens list to RPN (reverse polish notation) */
	for (i = 0; i < t_list_count; i++) {
		if (!t_list[i].is_operator) {
			if (add_token_to_list(&t_output, &t_output_count, t_list[i].data, 0) < 0) {
				fprintf(stderr, "ERROR: could not allocate output token.\n");
				goto error_out;
			}
		}
		else {
			if (t_list[i].data == '(') {
				if (s_push(&stack, '(') < 0) {
					fprintf(stderr, "ERROR: could not allocate stack node.\n");
					goto error_out;
				}
			}
			else if (t_list[i].data == ')') {
				while (1) {
					if (s_pop(&stack, &popped_op) < 0) {
						fprintf(stderr, "ERROR: trying to pop empty stack.\n");
						goto error_out;
					}
					if (popped_op == '(')
						break;

					if (add_token_to_list(&t_output, &t_output_count, popped_op, 1) < 0) {
						fprintf(stderr, "ERROR: could not allocate output token.\n");
						goto error_out;
					}
				}
			}
			else {
				while (!s_peek(&stack, &peek) && c_weight(peek) >= c_weight(t_list[i].data)) {
					if (s_pop(&stack, &popped_op) < 0) {
						fprintf(stderr, "ERROR: trying to pop empty stack.\n");
						goto error_out;
					}

					if (add_token_to_list(&t_output, &t_output_count, popped_op, 1) < 0) {
						fprintf(stderr, "ERROR: could not allocate output token.\n");
						goto error_out;
					}
				}

				if (s_push(&stack, t_list[i].data) < 0) {
					fprintf(stderr, "ERROR: could not allocate stack node.\n");
					goto error_out;
				}
			}
		}
	}

	/* If there is anything still left on stack, put in output list of tokens */
	while (stack != NULL) {
		if (s_pop(&stack, &popped_op) < 0) {
			fprintf(stderr, "ERROR: trying to pop empty stack.\n");
			goto error_out;
		}
		if (add_token_to_list(&t_output, &t_output_count, popped_op, 1) < 0) {
			fprintf(stderr, "ERROR: could not allocate output token.\n");
			goto error_out;
		}
	}

	/*  Step 4: Solve RPN */
	int result, operand1, operand2;

	for (i = 0; i < t_output_count; i++) {
		if (!t_output[i].is_operator) {
			if (s_push(&stack, t_output[i].data) < 0) {
				fprintf(stderr, "ERROR: could not allocate stack node.\n");
				goto error_out;
			}
		}
		else {
			if (s_pop(&stack, &operand2) < 0 || s_pop(&stack, &operand1) < 0) {
				fprintf(stderr, "ERROR: trying to pop empty stack.\n");
				goto error_out;
			}
			switch(t_output[i].data) {
				case '-':
					result = operand1 - operand2;
					break;
				case '+':
					result = operand1 + operand2;
					break;
				case '*':
					result = operand1 * operand2;
					break;
				default:
					fprintf(stderr, "ERROR: operator not supported.\n");
					goto error_out;
					break;
			}
			if (s_push(&stack, result) < 0) {
				fprintf(stderr, "ERROR: could not allocate stack node.\n");
				goto error_out;
			}
		}
	}

	if (s_pop(&stack, &result) < 0) {
		fprintf(stderr, "ERROR: trying to pop empty stack.\n");
		goto error_out;
	}
	printf("Result = %d\n", result);

	free(t_output);
	free(t_list);
	s_free(&stack);

	return 0;

error_out:
	free(t_output);
	free(t_list);
	s_free(&stack);

	return -1;
}
