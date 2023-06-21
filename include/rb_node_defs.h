#ifndef RB_NODE_DEFS_H
#define RB_NODE_DEFS_H

#include <stdint.h>

#define RB_NODE_MAX_CHILD (0x2u)
#define RB_LEFT_CHILD     (0x0u)
#define RB_RIGHT_CHILD    (0x1u)
#define RB_NODE_RED		  (0x0u)
#define RB_NODE_BLACK	  (0x1u)

typedef struct rb_node_struct
{
	int data;
	uintptr_t parent_color;

	union
	{
		struct rb_node_struct* child[RB_NODE_MAX_CHILD];
		struct
		{
			struct rb_node_struct* left;
			struct rb_node_struct* right;
		};
	};
	
} rb_node;

#endif
