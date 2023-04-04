#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  // TODO: initialize struct if needed
  return p;
}
void node_free_inorder(node_t *curr)
{
  if (curr->left == NULL && curr->right == NULL)
  {
    if (curr->parent != NULL)
    {
      if (curr == curr->parent->left)
      {
        curr->parent->left = NULL;
      }
      else
      {
        curr->parent->right = NULL;
      }
      free(curr);
    }
    return;
  }
  if (curr->left != NULL)
  {
    node_free_inorder(curr->left);
  }
  if (curr->right != NULL)
  {
    node_free_inorder(curr->right);
  }
}
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  node_t *curr = t->root;
  if (curr != NULL)
  {
    node_free_inorder(curr);
  }
  free(t);
}
void rotate_right(rbtree *t, node_t *curr)
{
  // 입력 받은 노드를 기준으로 right 회전
  // 1. 회전 기준 노드의 왼쪽 자식 노드가 root가 되고,
  // 2. 새로운 root 노드의 오른쪽 자식 노드를 임시로 저장
  // 3. 회전 기준 노드가 새로운 root의 오른쪽 자식 노드가 되고,
  // 4. 임시 저장된 노드는 회전 기준 노드의 왼쪽 자식 노드가 된다.
  // 회전 후 root 노드 return(예정)
  curr->left->parent = curr->parent;
  if (curr->parent != NULL)
  {
    if (curr == curr->parent->left)
    {
      curr->parent->left = curr->left;
    }
    else
    {
      curr->parent->right = curr->left;
    }
  }

  curr->parent = curr->left;
  curr->left = curr->parent->right;
  if (curr->left != NULL)
  {
    curr->left->parent = curr;
  }
  curr->parent->right = curr;

  // 회전으로 인해 rbtree의 루트가 변경되었는지 확인하고,
  // 변경되었다면 rbtree의 루트 정보 변경
  node_t *root_new = curr->parent;
  if (root_new->parent == NULL) // 회전의 기준점이 트리의 루트 노드였던 경우
  {
    t->root = curr->parent;
  }
  // else // 회전 시작전 회전 기준 노드의 부모 노드가 존재한 경우
  // {
  //   if (curr == curr->parent->parent->left)
  //   {
  //     curr->parent->parent->left = curr->parent;
  //   }
  //   else
  //   {
  //     curr->parent->parent->right = curr->parent;
  //   }
  // }
}
void rotate_left(rbtree *t, node_t *curr)
{
  // 입력 받은 노드를 기준으로 right 회전
  // 1. 회전 기준 노드의 왼쪽 자식 노드가 root가 되고,
  // 2. 새로운 root 노드의 오른쪽 자식 노드를 임시로 저장
  // 3. 회전 기준 노드가 새로운 root의 오른쪽 자식 노드가 되고,
  // 4. 임시 저장된 노드는 회전 기준 노드의 왼쪽 자식 노드가 된다.
  // 회전 후 root 노드 return(예정)
  curr->right->parent = curr->parent;
  if (curr->parent != NULL)
  {
    if (curr == curr->parent->left)
    {
      curr->parent->left = curr->right;
    }
    else
    {
      curr->parent->right = curr->right;
    }
  }
  curr->parent = curr->right;
  curr->right = curr->parent->left;
  if (curr->right != NULL)
  {
    curr->right->parent = curr;
  }
  curr->parent->left = curr;

  // 회전으로 인해 rbtree의 루트가 변경되었는지 확인하고,
  // 변경되었다면 rbtree의 루트 정보 변경
  node_t *root_new = curr->parent;
  if (root_new->parent == NULL) // 회전의 기준점이 트리의 루트 노드였던 경우
  {
    t->root = curr->parent;
  }
  // else // 회전 시작전 회전 기준 노드의 부모 노드가 존재한 경우
  // {
  //   if (curr == curr->parent->parent->left)
  //   {
  //     curr->parent->parent->left = curr->parent;
  //   }
  //   else
  //   {
  //     curr->parent->parent->right = curr->parent;
  //   }
  // }
}
void arrange_rbtree_insert(rbtree *t, node_t *curr)
{
  if (curr->parent == NULL || curr->parent->parent == NULL)
  {
    return;
  }
  if (curr->color == RBTREE_BLACK) // Double-red 가 아닌 상황
  {
    return;
  }
  if (curr->color == RBTREE_RED && curr->parent->color == RBTREE_BLACK) // Double-red 가 아닌 상황
  {
    return;
  }

  // 위 조건을 모두 통과 하면 Double-red 인 상황
  node_t *u;
  if (curr->parent == curr->parent->parent->left)
  {
    u = curr->parent->parent->right;
  }
  else
  {
    u = curr->parent->parent->left;
  }
  color_t u_color;
  if (u == NULL) // nil 노드, color=black
  {
    u_color = RBTREE_BLACK;
  }
  else
  {
    u_color = u->color;
  }
  if (u_color != curr->parent->color) // restructuring
  {
    if (curr->parent == curr->parent->parent->left)
    {
      if (curr == curr->parent->left)
      {
        color_t p_color = curr->parent->color;
        curr->parent->color = curr->parent->parent->color;
        curr->parent->parent->color = p_color;

        rotate_right(t, curr->parent->parent);
      }
      else
      {
        rotate_left(t, curr->parent);

        // color_t p_color = curr->parent->color;
        // curr->parent->color = curr->parent->parent->color;
        // curr->parent->parent->color = p_color;

        // rotate_right(t, curr->parent->parent);

        node_t *new_curr = curr->left;
        color_t p_color = new_curr->parent->color;
        new_curr->parent->color = new_curr->parent->parent->color;
        new_curr->parent->parent->color = p_color;

        rotate_right(t, new_curr->parent->parent);
      }
    }
    else if (curr->parent == curr->parent->parent->right)
    {
      if (curr == curr->parent->right)
      {
        color_t p_color = curr->parent->color;
        curr->parent->color = curr->parent->parent->color;
        curr->parent->parent->color = p_color;

        rotate_left(t, curr->parent->parent);
      }
      else
      {
        rotate_right(t, curr->parent);

        // color_t p_color = curr->parent->color;
        // curr->parent->color = curr->parent->parent->color;
        // curr->parent->parent->color = p_color;

        // rotate_left(t, curr->parent->parent);

        node_t *new_curr = curr->right;
        color_t p_color = new_curr->parent->color;
        new_curr->parent->color = new_curr->parent->parent->color;
        new_curr->parent->parent->color = p_color;

        rotate_left(t, new_curr->parent->parent);
      }
    }
  }
  else // recoloring
  {
    if (curr->parent->parent == t->root)
    {
      t->root->color = RBTREE_BLACK;
      t->root->left->color = RBTREE_BLACK;
      t->root->right->color = RBTREE_BLACK;
    }
    else
    {
      curr->parent->parent->color = RBTREE_RED;
      if (curr->parent->parent->left != NULL)
      {
        curr->parent->parent->left->color = RBTREE_BLACK;
      }
      if (curr->parent->parent->right != NULL)
      {
        curr->parent->parent->right->color = RBTREE_BLACK;
      }
      arrange_rbtree_insert(t, curr->parent->parent);
    }
  }
}
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->color = RBTREE_RED;
  node->key = key;
  if (t->root == NULL) // root 노드 삽입
  {
    t->root = node;
    t->root->color = RBTREE_BLACK;
    return t->root;
  }
  node_t *curr = t->root;
  while (1)
  {
    if (key <= curr->key)
    {
      if (curr->left == NULL)
      {
        curr->left = node;
        curr->left->parent = curr;
        curr = curr->left;
        break;
      }
      else
      {
        curr = curr->left;
      }
    }
    else
    {
      if (curr->right == NULL)
      {
        curr->right = node;
        curr->right->parent = curr;
        curr = curr->right;
        break;
      }
      else
      {
        curr = curr->right;
      }
    }
  }

  arrange_rbtree_insert(t, curr);
  // if (curr->parent->color == RBTREE_RED) // Double-red 상황
  // {
  //   arrange_rbtree_insert(t, curr);
  // }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *curr = t->root;
  while (curr != NULL)
  {
    if (key < curr->key)
    {
      if (curr->left == NULL)
      {
        return NULL;
      }
      else
      {
        curr = curr->left;
      }
    }
    else if (key > curr->key)
    {
      if (curr->right == NULL)
      {
        return NULL;
      }
      else
      {
        curr = curr->right;
      }
    }
    else
    {
      return curr;
    }
  }

  return curr;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *curr = t->root;

  while (curr != NULL)
  {
    if (curr->left != NULL)
    {
      curr = curr->left;
    }
    else
    {
      return curr;
    }
  }

  return curr;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *curr = t->root;

  while (curr != NULL)
  {
    if (curr->right != NULL)
    {
      curr = curr->right;
    }
    else
    {
      return curr;
    }
  }

  return curr;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}
int idx = 0;
void traversal_inorder(node_t *curr, key_t *arr, const size_t n)
{
  if (curr->left != NULL)
  {
    traversal_inorder(curr->left, arr, n);
  }
  if (idx < n)
  {
    *(arr + idx) = curr->key;
    idx += 1;
  }
  else
  {
    return;
  }
  if (curr->right != NULL)
  {
    traversal_inorder(curr->right, arr, n);
  }
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  traversal_inorder(t->root, arr, n);
  if (idx < n)
  {
    for (idx; idx < n ; idx++)
    {
      *(arr + idx) = NULL;
    }
  }
  idx = 0;
  return 0;
}
