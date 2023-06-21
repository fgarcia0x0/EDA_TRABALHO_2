#ifndef RB_NODE_H
#define RB_NODE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <rb_node_defs.h>
#include <rb_node_utils.h>

/**
 * @brief Insere um determinado elemento dentro da red-black-tree
 * 
 * @param item O elemento a ser inserido
 * @param root_pptr Um ponteiro para a raiz da árvore
 * @return true se foi inserido com sucesso, 
 *         false se o elemento a ser inserido ja existe na árvore
 */
bool rb_node_insert(int item, rb_node** root_pptr);

/**
 * @brief Busca um determinado elemento na red-black-tree
 * 
 * @param item O item a ser buscado
 * @param root_pptr Um ponteiro para a raiz da árvore
 * @return Se o elemento estiver na árvore, retorna o nó com o determinado 
 *         elemento, caso contrário retorna NULL.
 */
rb_node* rb_node_search(int item, rb_node** root_pptr);

/**
 * @brief Remove um determinado nó na red-black-tree
 * @details Observe que esta função não libera o nó da memória, quem tem que
 *          fazer isso é o usuário da API, ela apenas disconecta o nó removido 
 *          da red-black-tree
 * 
 * @param target O nó a ser removido
 * @param root_pptr Um ponteiro para a raiz da árvore
 */
void rb_node_remove(rb_node* target, rb_node** root_pptr);

/**
 * @brief Verifica se uma red-black-tree é válida
 * 
 * @param root_ptr Um ponteiro para a raiz da árvore
 * @return true se for válida, false caso contrário
 */
bool rb_is_valid(rb_node* root_ptr);

/**
 * @brief Conta a quantidade de nós em uma red-black-tree
 * 
 * @param root_ptr Um ponteiro para a raiz da árvore
 * @return A quantidade de nós na árvore 
 */
size_t rb_node_count(rb_node* root_ptr);

/**
 * @brief Libera da memória todos os nós apartir da subárvore dada
 * 
 * @param root_pptr Um ponteiro para uma subárvore da red-black-tree
 */
void rb_node_release(rb_node** root_pptr);

#endif
