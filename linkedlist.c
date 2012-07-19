#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"

int main(int argc, char **argv)
{
	struct linked_list *l;
	/* struct linked_list_node *n; */

	l = linked_list_new();
	/* n = linked_list_new_string_node("Thomas"); */

	linked_list_insert(l, linked_list_new_string_node("Diana"));
	linked_list_insert(l, linked_list_new_string_node("Belea"));
	linked_list_insert(l, linked_list_new_string_node("Thomas"));
	linked_list_insert(l, linked_list_new_string_node("Bracht"));
	linked_list_insert(l, linked_list_new_string_node("Laumann"));
	linked_list_insert(l, linked_list_new_string_node("Jespersen"));
	linked_list_insert(l, linked_list_new_string_node("BITCHES!"));

	linked_list_print(l);

	return 0;
}

void linked_list_print(struct linked_list *l)
{
	struct linked_list_node  *n = l->first;
	printf("Size: %d\n[", l->size);

	for (;;) {
		printf("'%s'", (char *) n->elm);
		if (n->next == NULL)
			break;
		else
			printf(" <-> ");
		n = n->next;
	}

	printf("]\n");
}

struct linked_list *linked_list_new()
{
	struct linked_list *list;

	list = (struct linked_list *)malloc(sizeof(struct linked_list));

	list->first = NULL;
	list->size  = 0;

	return list;
}

struct linked_list_node *linked_list_new_node()
{
	struct linked_list_node *node;

	/* Allocate! */
	node = (struct linked_list_node *)malloc(sizeof(struct linked_list_node));

	node->next = NULL;
	node->prev = NULL;
	node->elm  = NULL;

	return node;
}

struct linked_list_node *linked_list_new_string_node(char *str)
{
	struct linked_list_node *node;

	node = linked_list_new_node();

	node->elm = (void *)malloc(sizeof(str));
	node->elm = str;

	return node;
}

int linked_list_insert(struct linked_list *list, struct linked_list_node *item)
{
	if (list->first == NULL) {
		list->first = item;
		list->size++;
		return 0;
	}

	struct linked_list_node *n, *last;
	n = list->first;

	for (last = n; n; last = n, n = n->next)
		;

	n = item;

	/* Make them point to each other */
	item->prev = last;
	last->next = item;
	list->size++;

	return 0;
}

/* Linear time pop */
struct linked_list_node *linked_list_pop(struct linked_list *l)
{
	struct linked_list_node *n = l->first;

	for (n = l->first; n->next; n = n->next)
		;

	return n;
}
