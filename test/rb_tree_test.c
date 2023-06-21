#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include <rb_node.h>

#ifndef RBTREE_SMALL_CASE
	#define RBTREE_TEST_MAX_TREES (1000u)
	#define RBTREE_TEST_MAX_INSERTIONS (10000u)
	#define RBTREE_TEST_MAX_RANGE (100000u)
	#define RBTREE_TEST_MAX_REMOVES (1000u)
#else
	#define RBTREE_TEST_MAX_TREES (1u)
	#define RBTREE_TEST_MAX_INSERTIONS (100u)
	#define RBTREE_TEST_MAX_RANGE (1000u)
	#define RBTREE_TEST_MAX_REMOVES (10u)
#endif

int main()
{
	for (uint32_t i = 0; i < RBTREE_TEST_MAX_TREES; ++i)
	{
		int temp[RBTREE_TEST_MAX_INSERTIONS] = { 0 };
		int* p = temp;

		rb_node* root = NULL;
        size_t node_cnt = 0;

		srand(time(NULL));

		while (node_cnt < RBTREE_TEST_MAX_INSERTIONS)
		{
			int elem = rand() % RBTREE_TEST_MAX_RANGE;
			bool inserted = rb_node_insert(elem, &root);

			if (inserted)
			{
				*p++ = elem;
				++node_cnt;
			}
		}

		size_t tree_node_cnt = rb_node_count(root);
		if (tree_node_cnt && tree_node_cnt == node_cnt)
		{
			printf("[+] RB-TREE node count: %zu\n", tree_node_cnt);
		}
		else
		{
			fprintf(stderr, "[-] Erro: Invalid tree node count\n");
			exit(EXIT_FAILURE);
		}

		bool rbtree_valid = rb_is_valid(root);
		printf("[+] Valid RB-TREE: %s\n", rbtree_valid ? "true" : "false");

		if (!rbtree_valid)
			exit(EXIT_FAILURE);

		// second step: remove nodes
		uint32_t mid = RBTREE_TEST_MAX_INSERTIONS / 2;
		assert((mid + RBTREE_TEST_MAX_REMOVES) < RBTREE_TEST_MAX_INSERTIONS);

		for (uint32_t i = 0; i < RBTREE_TEST_MAX_REMOVES; ++i)
		{
            rb_node* victim = rb_node_search(temp[mid + i], &root);
            
            if (!victim)
            {
                fprintf(stderr, "[-] Error to remove avl tree node\n");
				exit(EXIT_FAILURE);
            }

			rb_node_remove(victim, &root);
			free(victim);
		}

		size_t new_tree_node_cnt = rb_node_count(root);
		if (new_tree_node_cnt && new_tree_node_cnt == (RBTREE_TEST_MAX_INSERTIONS - RBTREE_TEST_MAX_REMOVES))
		{
			printf("[+] RB-TREE node count (Pos-Remove): %zu\n", new_tree_node_cnt);
		}
		else
		{
			fprintf(stderr, "[-] Erro (Remove): Invalid tree node count\n");
			exit(EXIT_FAILURE);
		}

		rbtree_valid = rb_is_valid(root);
		printf("[+] Valid RB-TREE (Pos-Remove): %s\n", rbtree_valid ? "true" : "false");

		if (!rbtree_valid)
			exit(EXIT_FAILURE);

		rb_node_release(&root);
		putchar('\n');
	}

	printf("[+] Successfully\n");
}
