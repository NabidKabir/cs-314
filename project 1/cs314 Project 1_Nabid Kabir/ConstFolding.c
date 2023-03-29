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

#define SUBSET_SIZE 3
#define EMPTY_FIELD 0xFFFFF


/*
* Will traverse instructions in groups of 3, searching for a patten of LOADI, LOADI, and ADD | SUB | MUL instructions 
*/

size_t get_ilist_length(Instruction *head);
int is_fold_valid(Instruction *head);

Instruction *constant_folding(Instruction *head) {

		/* STUDENTS - BEGIN */
	for (Instruction *subset_head = head; get_ilist_length(subset_head) >= SUBSET_SIZE; subset_head = subset_head->next) {
		if (!is_fold_valid(subset_head)) {
			continue;
		}

		int constant_1 = subset_head->field1;
		int constant_2 = subset_head->next->field1;
		int operation_constant;

		switch (subset_head->next->next->opcode) {
			case ADD:
				operation_constant = constant_1 + constant_2;
				break;
			case SUB:
				operation_constant = constant_1 - constant_2;
				break;
			case MUL:
				operation_constant = constant_1 * constant_2;
				break;
			default:
				break;
		}
		subset_head->next->next->field1 = operation_constant;
		subset_head->next->next->field2 = subset_head->next->next->field3;
		subset_head->next->next->field3 = EMPTY_FIELD;
		subset_head->next->next->opcode = LOADI;
	}
	/* STUDENTS - END */

	return head;
}

size_t get_ilist_length(Instruction *head)
{
	size_t len = 0;
	while(head)
	{
		++len;
		head = head->next;
	}
	return len;
}

int is_fold_valid(Instruction *subset_head)
{
	if (subset_head->opcode != LOADI) {
		return 0;
	}
	int register_a = subset_head->field2;
	subset_head = subset_head->next;

	if (subset_head->opcode != LOADI) {
		return 0;
	}
	int register_b = subset_head->field2;
	subset_head = subset_head->next;

	if (subset_head->opcode != ADD && subset_head->opcode != SUB && subset_head->opcode != MUL) 
	{
		return 0;
	}
	int operation_register_a = subset_head->field1;
	int operation_register_b = subset_head->field2;

	if (register_a == register_b) {
		return 0;
	}
	if (register_a != operation_register_a) {
		return 0;
	}
	if (register_b != operation_register_b) {
		return 0;
	}

	return 1;
	
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = constant_folding(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

