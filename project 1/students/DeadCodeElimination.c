/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Nabid Kabir                      *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

void instruction_trace_back(Instruction *, int, int);
int is_register_match(Instruction *, int, int);
int get_last_field(Instruction *);

Instruction *dead_code_elimination(Instruction *head) {

	/* STUDENTS - BEGIN */
	Instruction *current_head = head;
	while(current_head->next != NULL) 
	{
		current_head = current_head->next;
	}

	// Mark critical instructions
	while(current_head != NULL)
	{
		if (current_head->opcode == OUTPUTAI) {
			current_head->critical = 1;
			instruction_trace_back(current_head, current_head->field1, current_head->field2);
		}
		current_head = current_head->prev;
	}

	for (current_head = head; current_head != NULL;) {
		if (!current_head->critical) {
			Instruction *temp = current_head->prev;

			if (current_head->prev != NULL) {
				current_head->prev->next = current_head->next;
			}
			if (current_head->next != NULL) {
				current_head->next->prev = temp;
				head = current_head->next;
			}

			temp = current_head;
			current_head = current_head->next;

			free(temp);
			continue;
		}

		head = current_head;
		current_head = current_head->next;
	}

	while(head->prev != NULL) 
	{
		head = head->prev;
	}
	/* STUDENTS - END */

	return head;
}

void instruction_trace_back(Instruction *current_head, int a, int b) {
	for (; current_head != NULL; current_head = current_head->prev) {
		if (current_head->opcode == OUTPUTAI) {
			continue;
		}

		if (is_register_match(current_head, a, b)) {
			current_head->critical = 1;

			switch (current_head->opcode) {
				case LOADAI:
				case LSHIFTI:
				case RSHIFTI:
					instruction_trace_back(current_head->prev, current_head->field1, current_head->field2);
					break;
				case ADD:
				case SUB:
				case MUL:
				case DIV:
					instruction_trace_back(current_head->prev, current_head->field1, -1);
					instruction_trace_back(current_head->prev, current_head->field2, -1);
					break;
				case STOREAI:
					instruction_trace_back(current_head->prev, current_head->field1, -1);
					instruction_trace_back(current_head->prev, current_head->field2, -1);
				case LOADI:
				default:
					break;
			}

			break;
		}
	}
}

int is_register_match(Instruction *instruction, int reg, int constant) {
	int register_given = reg != -1;
	int constant_given = constant != -1;

	if (register_given && constant_given) {
		if (
			instruction->opcode == STOREAI &&
			instruction->field2 == reg &&
			instruction->field3 == constant
		) {
			return 1;
		}
	} else if (register_given && !constant_given) {
		if (instruction->opcode == STOREAI || instruction->opcode == OUTPUTAI) {
			return 0;
		}
		if (instruction->opcode == LOADI) {
			if (instruction->field2 == reg) {
				return 1;
			}
		} else if (instruction->field3 == reg) {
			return 1;
		}
	}

	return 0;
}


int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = dead_code_elimination(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

