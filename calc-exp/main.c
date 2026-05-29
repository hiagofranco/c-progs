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

int main(void) {

	struct token *t_list = NULL;
	struct token *t_tmp;

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
			t_tmp = realloc(t_list, (t_list_count + 1) * sizeof(struct token));
			if (!t_tmp) {
				if (t_list)
					free(t_list);
				fprintf(stderr, "ERROR: could not allocate token \'%d\'.\n", number);
				return -1;
			}
			t_list = t_tmp;
			t_list[t_list_count].data = number;
			t_list[t_list_count].is_operator = 0;
			t_list_count++;
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
					t_tmp = realloc(t_list, (t_list_count + 1) * sizeof(struct token));
					if (!t_tmp) {
						if (t_list)
							free(t_list);
						fprintf(stderr, "ERROR: could not allocate token \'%c\'.\n", input[i]);
						return -1;
					}

					t_list = t_tmp;
					t_list[t_list_count].data = input[i];
					t_list[t_list_count].is_operator = 1;
					t_list_count++;

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
			t_tmp = realloc(t_output, (t_output_count + 1) * sizeof(struct token));
			if (!t_tmp) {
				fprintf(stderr, "ERROR: could not allocate token for output list.\n");
				goto error_out;
			}
			t_output = t_tmp;
			t_output[t_output_count].data = t_list[i].data;
			t_output[t_output_count].is_operator = 0;
			t_output_count++;
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
					popped_op = s_pop(&stack);
					if (popped_op < 0) {
						fprintf(stderr, "ERROR: trying to pop empty stack.\n");
						goto error_out;
					}
					if (popped_op == '(')
						break;

					t_tmp = realloc(t_output, (t_output_count + 1) * sizeof(struct token));
					if (!t_tmp) {
						fprintf(stderr, "ERROR: could not allocate token for output list.\n");
						goto error_out;
					}
					t_output = t_tmp;
					t_output[t_output_count].data = popped_op;
					t_output[t_output_count].is_operator = 1;
					t_output_count++;
				}
			}
			else {
				peek = s_peek(&stack);

				if (c_weight(peek) >= c_weight(t_list[i].data)) {
					popped_op = s_pop(&stack);
					if (popped_op < 0) {
						fprintf(stderr, "ERROR: trying to pop empty stack.\n");
						goto error_out;
					}

					t_tmp = realloc(t_output, (t_output_count + 1) * sizeof(struct token));
					if (!t_tmp) {
						fprintf(stderr, "ERROR: could not allocate token for output list.\n");
						goto error_out;
					}
					t_output = t_tmp;
					t_output[t_output_count].data = popped_op;
					t_output[t_output_count].is_operator = 1;
					t_output_count++;
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
		popped_op = s_pop(&stack);
		if (popped_op < 0) {
			fprintf(stderr, "ERROR: trying to pop empty stack.\n");
			goto error_out;
		}

		t_tmp = realloc(t_output, (t_output_count + 1) * sizeof(struct token));
		if (!t_tmp) {
			fprintf(stderr, "ERROR: could not allocate token for output list.\n");
			goto error_out;
		}
		t_output = t_tmp;
		t_output[t_output_count].data = popped_op;
		t_output[t_output_count].is_operator = 1;
		t_output_count++;
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
			operand2 = s_pop(&stack);
			operand1 = s_pop(&stack);
			if (operand1 < 0 || operand2 < 0) {
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
					break;
			}
			if (s_push(&stack, result) < 0) {
				fprintf(stderr, "ERROR: could not allocate stack node.\n");
				goto error_out;
			}
		}
	}

	printf("Result = %d\n", s_pop(&stack));

	if (t_output)
		free(t_output);
	if (t_list)
		free(t_list);
	s_free(&stack);

	return 0;

error_out:
	if (t_output)
		free(t_output);
	if (t_list)
		free(t_list);
	s_free(&stack);
	return -1;
}
