#include <rb_node.h>

bool rb_node_insert(int item, rb_node** root_pptr)
{
    RB_ASSERT(root_pptr);

    rb_node** link = root_pptr;
    rb_node* parent = NULL;

    rb_node* new_node = rb_node_make(item);
    size_t child_pos = RB_LEFT_CHILD;
    RB_ASSERT(new_node);
    
    while (*link)
    {
        parent = *link;

        if (new_node->data == parent->data)
        {
            free(new_node);
            return false;
        }
        else
        {
            child_pos = !!(new_node->data > parent->data);
            link = &parent->child[child_pos];
        }
    }
  
    rb_node_link(new_node, parent, link);
    rb_node_insert_fixup(new_node, root_pptr);

    return true;
}

rb_node* rb_node_search(int item, rb_node** root_pptr)
{
    RB_ASSERT(root_pptr);
    rb_node** link = root_pptr;
    
    while (*link)
    {
        rb_node* curr = *link;

        if (curr->data == item)
            return curr;
        else
            link = &curr->child[item > curr->data];
    }

    return NULL;
}

void rb_node_remove(rb_node* target, rb_node** root_pptr)
{
    RB_ASSERT(target && root_pptr);
    rb_node* child = (target->left) ? target->left : target->right;
    rb_node* parent = rb_node_parent(target);
    uint8_t color = RB_NODE_BLACK;

    if (child && rb_node_sibling(child))
    {
        rb_node* old = target;
        target = rb_node_first(target->right);

        child = target->right;
		parent = rb_node_parent(target);
		color = rb_node_color(target);
        
        rb_node_transplant(old, target, root_pptr);

        if (parent == old)
            parent = target;
        else
        {
            if (child)
                rb_node_set_parent(child, parent);
            
            parent->left = child;
            target->right = old->right;
            rb_node_set_parent(old->right, target);
        }

        rb_node_set_color(target, rb_node_color(old));
		target->left = old->left;
		rb_node_set_parent(old->left, target);
    }
    else
    {
        color = rb_node_color(target);
        rb_node_transplant(target, child, root_pptr);
    }

    if (color == RB_NODE_BLACK)
        rb_node_remove_fixup(child, parent, root_pptr);
}

size_t rb_node_count(rb_node* root_ptr)
{
    if (!root_ptr)
        return 0u;
    else
        return 1u + rb_node_count(root_ptr->left) 
                  + rb_node_count(root_ptr->right);
}

void rb_node_release(rb_node** root_pptr)
{
    if (root_pptr && *root_pptr)
	{
		rb_node* curr = *root_pptr;
		rb_node_release(&(curr->left));
		rb_node_release(&(curr->right));

		free(curr);
        curr = NULL;
	}
}

bool rb_is_valid(rb_node* root)
{
    return rb_count_black_height(root) != -1;
}
