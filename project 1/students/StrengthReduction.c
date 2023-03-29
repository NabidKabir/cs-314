/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author:                                  *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "InstrUtils.h"
#include "Utils.h"

#define SUBSET_SIZE 2
#define EMPTY_FIELD 0xFFFFF

size_t get_ilist_length(Instruction *head);
int is_reduction_valid(Instruction *head);

/*
* Will return log2(n) if it is an integer (meaning n is a power of 2), and -1 otherwise
*/
int int_log2(int n) {

	int s = 1, i = 0;

	while (s < n) {
		s = s << 1;
		i++;
	}

	if (s == n)
		return i;
	else
		return -1;

}

/*
* Will traverse instructions in groups of two, searching for a patten of LOADI and MUL | DIV instructions 
* where LOADI is a power of 2, and collapse these instructions into on shift operation of LSHIFTI | RSHIFTI
*/
Instruction *strength_reduction(Instruction *head) {

		/* STUDENTS - BEGIN */
	for (Instruction *window_head = head; get_ilist_length(window_head) >= SUBSET_SIZE; window_head = window_head->next) {
		if (!is_reduction_valid(window_head)) {
			continue;
		}

		Instruction *window[SUBSET_SIZE] = {
			window_head,
			window_head->next
		};

		int constant = window[0]->field1;
		int shift_constant = int_log2(constant);

		switch (window[1]->opcode) {
			case MUL:
			{
				int reg_a = window[0]->field2;
				int op_reg_b = window[1]->field1;
				if (reg_a == op_reg_b) {
					window[1]->field1 = window[1]->field2;
				}
				window[1]->field2 = shift_constant;
				window[1]->opcode = LSHIFTI;
				break;
			}
			case DIV:
				window[1]->field2 = shift_constant;
				window[1]->opcode = RSHIFTI;
				break;
			default:
				break;
		}
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

int is_reduction_valid(Instruction *subset_head)
{
	if(subset_head->opcode != LOADI)
	{
		return 0;
	}
	if(int_log2(subset_head->field1) == -1)
	{
		return 0;
	}
	int register_a = subset_head->field2;

	subset_head = subset_head->next;

	if(subset_head->opcode != MUL && subset_head->opcode != DIV)
	{
		return 0;
	}

	int operation_register_a = subset_head->field2;
	int operation_register_b = subset_head->field1;

	if(subset_head->opcode == DIV)
	{
		if(register_a != operation_register_a)
		{
			return 0;
		}
	}
	else if(subset_head->opcode == MUL)
	{
		if(register_a != operation_register_a && register_a != operation_register_b)
		{
			return 0;
		}
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

	head = strength_reduction(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

