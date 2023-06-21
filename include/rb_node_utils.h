#ifndef RB_NODE_UTILS_H
#define RB_NODE_UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include <rb_node_defs.h>

#define RB_ASSERT(cond)   (assert((cond)))
#ifndef RB_FORCE_INLINE
	#ifdef __GNUC__
		#define RB_FORCE_INLINE inline __attribute__((always_inline))
	#else
		#define RB_FORCE_INLINE inline
	#endif
#endif

static RB_FORCE_INLINE uint8_t rb_node_color(rb_node* target)
{
	return target->parent_color & RB_NODE_BLACK;
}

static RB_FORCE_INLINE bool rb_node_is_red(rb_node* target)
{
	return !rb_node_color(target);
}

static RB_FORCE_INLINE bool rb_node_is_black(rb_node* target)
{
	return rb_node_color(target);
}

static RB_FORCE_INLINE rb_node* rb_node_parent(rb_node* target)
{
	return (rb_node *)(target->parent_color & ~rb_node_color(target));
}

static RB_FORCE_INLINE void rb_node_set_black(rb_node* target)
{
	target->parent_color |= RB_NODE_BLACK;
}

static RB_FORCE_INLINE void rb_node_set_red(rb_node* target)
{
	target->parent_color &= ~rb_node_color(target);
}

static RB_FORCE_INLINE void rb_node_set_parent(rb_node* target, rb_node* new_parent)
{
	target->parent_color = rb_node_color(target) | (uintptr_t)(new_parent);
}

static RB_FORCE_INLINE void rb_node_set_color(rb_node* target, uint8_t color)
{    
	target->parent_color = (target->parent_color & ~rb_node_color(target)) | color;
}

static RB_FORCE_INLINE void rb_node_clear_black(rb_node* target)
{
	target->parent_color = RB_NODE_BLACK;
}

static RB_FORCE_INLINE void rb_node_link(rb_node* target, 
										 rb_node* parent, 
										 rb_node** link)
{
	rb_node_set_parent(target, parent);
	target->left = target->right = NULL;
	*link = target;
}

static RB_FORCE_INLINE rb_node* rb_node_make(int item)
{
	rb_node* result = (rb_node *) malloc(sizeof(rb_node));
	if (result)
		*result = (rb_node){ .data = item, .parent_color = RB_NODE_RED };
	return result;
}

static RB_FORCE_INLINE void rb_node_rotate(rb_node* target, 
                                           rb_node** root_pptr, 
                                           uint8_t orient)
{
    rb_node* target_child = target->child[!orient];
	rb_node* parent = rb_node_parent(target);

	if ((target->child[!orient] = target_child->child[orient]))
		rb_node_set_parent(target_child->child[orient], target);

	target_child->child[orient] = target;
	rb_node_set_parent(target_child, parent);

	if (parent)
    {
        if (target == parent->child[orient])
            parent->child[orient] = target_child;
        else
            parent->child[!orient] = target_child;
    }
	else
        *root_pptr = target_child;

	rb_node_set_parent(target, target_child);
}

static void rb_node_insert_fixup(rb_node* new_node, rb_node** root_pptr)
{
    rb_node_set_red(new_node);

    rb_node* parent = NULL;
    rb_node* grandparent = NULL;

    while ((parent = rb_node_parent(new_node)) && rb_node_is_red(parent))
    {
        grandparent = rb_node_parent(parent);
        uint8_t orient = (grandparent->child[RB_RIGHT_CHILD] == parent);
        rb_node* uncle = grandparent->child[!orient];

        if (uncle && rb_node_is_red(uncle)) // Case 1
        {
            rb_node_set_black(uncle);
            rb_node_set_black(parent);
            rb_node_set_red(grandparent);
            new_node = grandparent;
            continue;
        }

        if (new_node == parent->child[!orient]) // Case 2
        {
            rb_node_rotate(parent, root_pptr, orient);

            // swap parent and new_node
            rb_node* tmp = parent;
			parent = new_node;
			new_node = tmp;
        }

        // Case 3
        rb_node_set_black(parent);
        rb_node_set_red(grandparent);
        rb_node_rotate(grandparent, root_pptr, !orient);
    }

    rb_node_clear_black(*root_pptr);
}

static RB_FORCE_INLINE void rb_node_transplant(rb_node* u, rb_node* v, rb_node** root_pptr)
{
    rb_node* parent = rb_node_parent(u);
    
    if (!parent)
        *root_pptr = v;
    else if (u == parent->left)
        parent->left = v;
    else
        parent->right = v;

    if (v)
        rb_node_set_parent(v, parent);
}

static RB_FORCE_INLINE rb_node* rb_node_sibling(rb_node* target)
{
    RB_ASSERT(target);
    rb_node* parent = rb_node_parent(target);
    RB_ASSERT(parent);

    return (target == parent->left) ? parent->right : parent->left;
}

static RB_FORCE_INLINE rb_node* __rb_node_iter_first_last(rb_node* target, uint8_t orient)
{
    if (!target)
        return NULL;

    while (target->child[orient])
        target = target->child[orient];
    
    return target;
}

static RB_FORCE_INLINE rb_node* rb_node_first(rb_node* target)
{
    return __rb_node_iter_first_last(target, RB_LEFT_CHILD);
}

static RB_FORCE_INLINE rb_node* rb_node_last(rb_node* target)
{
    return __rb_node_iter_first_last(target, RB_RIGHT_CHILD);
}

static RB_FORCE_INLINE rb_node* __rb_node_iter_next_prev(rb_node* target, uint8_t orient)
{
    if (!target)
        return NULL;
    else if (!target->child[orient])
    {
        rb_node* parent = NULL;
        while ((parent = rb_node_parent(target)) && parent->child[orient] == target)
            target = parent;
        return parent;
    }
    else
        return rb_node_first(target->child[orient]);
}

static RB_FORCE_INLINE rb_node* rb_node_next(rb_node* target)
{
    return __rb_node_iter_next_prev(target, RB_RIGHT_CHILD);
}

static RB_FORCE_INLINE rb_node* rb_node_prev(rb_node* target)
{
    return __rb_node_iter_next_prev(target, RB_LEFT_CHILD);
}

static void rb_node_remove_fixup(rb_node* target, rb_node* parent, rb_node** root_pptr)
{
    while ((!target || rb_node_is_black(target)) && (target != *root_pptr))
    {
        uint8_t orient = (target == parent->child[RB_RIGHT_CHILD]);
        
        if (target == parent->child[orient])
        {
            rb_node* sibling = parent->child[!orient];

            if (sibling && rb_node_is_red(sibling))
            {
                rb_node_set_black(sibling);
                rb_node_set_red(parent);
                rb_node_rotate(parent, root_pptr, orient);
                sibling = parent->child[!orient];
            }

            if ((!sibling->child[orient] || rb_node_is_black(sibling->child[orient])) &&
                (!sibling->child[!orient] || rb_node_is_black(sibling->child[!orient])))
            {
                rb_node_set_red(sibling);
                target = parent;
                parent = rb_node_parent(target);
            }
            else
            {
                if (!sibling->child[!orient] || rb_node_is_black(sibling->child[!orient]))
                {
                    rb_node_set_black(sibling->child[orient]);
                    rb_node_set_red(sibling);
                    rb_node_rotate(sibling, root_pptr, !orient);
                    sibling = parent->child[!orient];
                }
                
                rb_node_set_color(sibling, rb_node_color(parent));
                rb_node_set_black(parent);

                rb_node_set_black(sibling->child[!orient]);
                rb_node_rotate(parent, root_pptr, orient);
                target = *root_pptr;
                break;
            }
        }
    }

    if (target)
        rb_node_set_black(target);
}

/**
 * @brief Conta a quantidade de nós negros de uma red black tree.
 * 
 * @param target 
 * @return Retorna o número de nós negros em uma subárvore de um dado nó ou 
 *         -1 se não for uma red black tree.
 */
static inline int32_t rb_count_black_height(rb_node* target) 
{
    if (!target)
        return 0; 

    int32_t hleft = rb_count_black_height(target->left);
    int32_t hright = rb_count_black_height(target->right);

    if (hleft == -1 || hright == -1 || hleft != hright)
        return -1; 
    else
        return hleft + rb_node_color(target);
}

#endif
