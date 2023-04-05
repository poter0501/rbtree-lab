#include "rbtree.h"

#include <stdlib.h>

typedef enum
{
  RED_AND_BLACK,
  CASE1_BASIC,
  CASE1_REVERSE,
  CASE2,
  CASE3_BASIC,
  CASE3_REVERSE,
  CASE4_BASIC,
  CASE4_REVERSE,
} delete_case;

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
void node_free_postorder(rbtree *t, node_t *curr)
{
  if (curr->left != NULL)
  {
    node_free_postorder(t, curr->left);
  }
  if (curr->right != NULL)
  {
    node_free_postorder(t, curr->right);
  }
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
  }
  else
  {
    t->root = NULL;
  }
  free(curr);
}
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  node_t *curr = t->root;
  if (curr != NULL)
  {
    // node_free_inorder(curr);
    node_free_postorder(t, curr);
  }
  // free(t->nil);
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
  if (t->root != NULL)
  {
    t->root->color = RBTREE_BLACK;
  }
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
  if (t->root != NULL)
  {
    t->root->color = RBTREE_BLACK;
  }
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
  node_t *node = (node_t *)calloc(1, sizeof(node_t));
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

node_t *find_successor(node_t *curr)
{
  curr = curr->right;
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

node_t *bst_delete(rbtree *t, node_t *p, node_t *extra_black)
{
  // node_t *target = rbtree_find(t, key);
  node_t *target = p;
  if (target != NULL)
  {
    // node_t *extra_black = (node_t *)calloc(1, sizeof(node_t));
    // extra_black->color = RBTREE_BLACK;

    // 삭제하려는 노드의 자식 노드가 없는 경우
    // extra_black의 부모 노드를 삭제 노드의 부모를 가르키고 삭제하려는 노드의 부모의 자식을 extra blcak으로
    if (target->left == NULL && target->right == NULL)
    {
      if (target == t->root)
      {
        t->root = NULL;
        free(target);
        // free(extra_black);
        return NULL;
      }

      // RED면 바로 삭제
      if (target->color == RBTREE_RED)
      {
        // 부모 노드와의 연결 해제
        if (target == target->parent->left)
        {
          target->parent->left = NULL;
          free(target);
        }
        else
        {
          target->parent->right = NULL;
          free(target);
        }
        // free(extra_black);
        return NULL;
      }
      else // black이면 extra black 부여
      {
        extra_black->right = target->parent; // nil 노드가 extra black을 가지는 경우
        if (target == target->parent->left)
        {
          target->parent->left = NULL;
          free(target);
        }
        else
        {
          target->parent->right = NULL;
          free(target);
        }
      }
    }
    else if (target->left != NULL && target->right == NULL)
    {
      if (target == t->root)
      {
        t->root = target->left;
        t->root->color = RBTREE_BLACK;
        t->root->parent = NULL;
        free(target);
        // free(extra_black);
        return NULL;
      }
      else
      {
        if (target->color == RBTREE_BLACK) // extra blcak 부여
        {
          extra_black->parent = target->left;
        }

        if (target == target->parent->left)
        {
          target->parent->left = target->left;
          target->left->parent = target->parent;
        }
        else
        {
          target->parent->right = target->left;
          target->left->parent = target->parent;
        }
      }
      free(target);
    }
    else if (target->left == NULL && target->right != NULL)
    {
      if (target == t->root)
      {
        t->root = target->right;
        t->root->color = RBTREE_BLACK;
        t->root->parent = NULL;
        free(target);
        // free(extra_black);
        return NULL;
      }
      else
      {
        if (target->color == RBTREE_BLACK) // extra blcak 부여
        {
          extra_black->parent = target->right;
        }

        if (target == target->parent->left)
        {
          target->parent->left = target->right;
          target->right->parent = target->parent;
        }
        else
        {
          target->parent->right = target->right;
          target->right->parent = target->parent;
        }
      }
      free(target);
    }
    else // left, right 가 둘다 있는 경우
    {
      node_t *successor = find_successor(target);
      target->key = successor->key;
      color_t deleted_color = successor->color;
      if (successor == target->right)
      {
        if (successor->right != NULL)
        {
          successor->parent->right = successor->right;
          successor->right->parent = successor->parent;
          if (deleted_color == RBTREE_BLACK) // successor 노드의 위치를 대체한 노드에 extra black 부여
          {
            extra_black->parent = successor->right;
          }
          else // red 인 경우 바로 삭제
          {
            // free(extra_black);
            free(successor);
            return NULL;
          }
          free(successor);
        }
        else
        {
          successor->parent->right = NULL;
          if (deleted_color == RBTREE_BLACK) // nil 노드에 extra black 부여
          {
            // extra_black->parent = successor->parent;
            extra_black->right = successor->parent; // nil 노드가 extra black인 경우를 구분하기 위해 사용
          }
          free(successor);
        }
      }
      else
      {
        if (successor->right != NULL)
        {
          successor->parent->left = successor->right;
          successor->right->parent = successor->parent;
          if (deleted_color == RBTREE_BLACK) // successor 노드의 위치를 대체한 노드에 extra black 부여
          {
            extra_black->parent = successor->right;
          }
          else // red 인 경우 바로 삭제
          {
            // free(extra_black);
            free(successor);
            return NULL;
          }
          free(successor);
        }
        else
        {
          successor->parent->left = NULL;
          if (deleted_color == RBTREE_BLACK) // nil 노드에 extra black 부여
          {
            // extra_black->parent = successor->parent;
            extra_black->right = successor->parent; // nil 노드가 extra black인 경우를 구분하기 위해 사용
          }
          else // red 인 경우 바로 삭제
          {
            // free(extra_black);
            free(successor);
            return NULL;
          }
          free(successor);
        }
      }
    }
    return extra_black;
  }
  free(target);
  return NULL;
}
delete_case check_delete_case(node_t *extra_black)
{
  node_t *parent;
  node_t *bro;
  //  부모 형제를 찾는다.
  if (extra_black->parent != NULL)
  {
    parent = extra_black->parent->parent;
    if (extra_black->parent->color == RBTREE_RED)
    {
      return RED_AND_BLACK;
    }
    if (extra_black->parent == parent->left)
    {
      bro = parent->right;
    }
    else
    {
      bro = parent->left;
    }
  }
  else // extra black 이 nil 노드에 있는 경우
  {
    parent = extra_black->right;
    if (parent->left == NULL)
    {
      bro = parent->right;
    }
    else
    {
      bro = parent->left;
    }
  }
  // case 판별
  if (bro->color == RBTREE_BLACK) // 형제가 블랙인 경우 case2, 3, 4
  {
    if (bro->left == NULL && bro->right == NULL) // 형제의 자식이 모두 nil 이기 때문에 black
    {
      return CASE2;
    }
    if (bro == parent->right) // basic case
    {
      if (bro->right == NULL) // 형제의 오른쪽 자식이 black
      {
        if (bro->left == NULL) // 형제의 왼쪽 자식이 black
        {
          return CASE2;
        }
        else if (bro->left->color == RBTREE_BLACK) // 형제의 왼쪽 자식이 black
        {
          return CASE2;
        }
        else // 형제의 왼쪽 자식이 red
        {
          return CASE3_BASIC;
        }
      }
      else if (bro->right->color == RBTREE_BLACK) // 형제의 오른쪽 자식이 black
      {
        if (bro->left == NULL) // 형제의 왼쪽 자식이 black
        {
          return CASE2;
        }
        else if (bro->left->color == RBTREE_BLACK) // 형제의 왼쪽 자식이 black
        {
          return CASE2;
        }
        else // 형제의 왼쪽 자식이 red
        {
          return CASE3_BASIC;
        }
      }
      else // 형제의 오른쪽 자식이 red
      {
        return CASE4_BASIC;
      }
    }
    else // reverse case
    {
      if (bro->left == NULL) // 형제의 왼쪽 자식이 black
      {
        if (bro->right == NULL) // 형제의 오른쪽 자식이 black
        {
          return CASE2;
        }
        else if (bro->right->color == RBTREE_BLACK) // 형제의 오른쪽 자식이 black
        {
          return CASE2;
        }
        else // 형제의 오른쪽 자식이 red
        {
          return CASE3_REVERSE;
        }
      }
      else if (bro->left->color == RBTREE_BLACK) // 형제의 왼쪽 자식이 black
      {
        if (bro->right == NULL) // 형제의 오른쪽 자식이 black
        {
          return CASE2;
        }
        else if (bro->right->color == RBTREE_BLACK) // 형제의 오른쪽 자식이 black
        {
          return CASE2;
        }
        else // 형제의 오른쪽 자식이 red
        {
          return CASE3_REVERSE;
        }
      }
      else // 형제의 왼쪽 자식이 red
      {
        return CASE4_REVERSE;
      }
    }
  }
  else // case1 인 상황
  {
    if (bro == parent->right)
    {
      return CASE1_BASIC;
    }
    else
    {
      return CASE1_REVERSE;
    }
  }
}
int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *temp = (node_t *)calloc(1, sizeof(node_t));
  temp->color = RBTREE_BLACK;
  node_t *extra_black = bst_delete(t, p, temp);
  while (extra_black != NULL)
  {
    node_t *parent;
    node_t *bro;
    //  부모, 형제를 찾는다.
    if (extra_black->parent != NULL)
    {
      if (extra_black->parent == t->root)
      {
        t->root->color = RBTREE_BLACK;
        free(temp);
        return 0;
      }

      parent = extra_black->parent->parent;
      if (extra_black->parent->color == RBTREE_RED)
      {
        extra_black->parent->color = RBTREE_BLACK;
        // extra_black = NULL;
        free(temp);
        return 0;
        // return RED_AND_BLACK;
      }
      if (extra_black->parent == parent->left)
      {
        bro = parent->right;
      }
      else
      {
        bro = parent->left;
      }
    }
    else // extra black 이 nil 노드에 있는 경우
    {
      parent = extra_black->right;
      if (parent->left == NULL)
      {
        bro = parent->right;
      }
      else
      {
        bro = parent->left;
      }
    }

    delete_case ce = check_delete_case(extra_black);
    switch (ce)
    {
    case RED_AND_BLACK:
      extra_black->parent->color = RBTREE_BLACK;
      free(temp);
      return 0;
      break;
    case CASE4_BASIC:
      bro->color = parent->color;
      if (bro->right != NULL)
      {
        bro->right->color = RBTREE_BLACK;
      }
      parent->color = RBTREE_BLACK;
      rotate_left(t, parent);
      free(temp);
      return 0;
      break;
    case CASE4_REVERSE:
      bro->color = parent->color;
      if (bro->left != NULL)
      {
        bro->left->color = RBTREE_BLACK;
      }
      parent->color = RBTREE_BLACK;
      rotate_right(t, parent);
      free(temp);
      return 0;
      break;
    case CASE3_BASIC:
      bro->color = RBTREE_RED;
      bro->left->color = RBTREE_BLACK;
      rotate_right(t, bro);
      break;
    case CASE3_REVERSE:
      bro->color = RBTREE_RED;
      bro->right->color = RBTREE_BLACK;
      rotate_left(t, bro);
      break;
    case CASE2:
      if (extra_black->parent != NULL)
      {
        extra_black->parent = parent;
      }
      else // extra black이 nil 노드인 경우
      {
        extra_black->parent = extra_black->right;
        extra_black->right = NULL;
      }
      bro->color = RBTREE_RED;
      break;
    case CASE1_BASIC:
      parent->color = RBTREE_RED;
      bro->color = RBTREE_BLACK;
      rotate_left(t, parent);
      break;
    case CASE1_REVERSE:
      parent->color = RBTREE_RED;
      bro->color = RBTREE_BLACK;
      rotate_left(t, parent);
      break;
    }
  }
  free(temp);
  return 0;
}
void traversal_inorder(node_t *curr, key_t *arr, const size_t n, size_t *idx)
{
  if (curr->left != NULL)
  {
    traversal_inorder(curr->left, arr, n, idx);
  }
  if (*idx < n)
  {
    *(arr + *idx) = curr->key;
    *idx += 1;
  }
  // else
  // {
  //   *(arr + *idx) = NULL;
  // }
  if (curr->right != NULL)
  {
    traversal_inorder(curr->right, arr, n, idx);
  }
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  size_t idx = 0;
  traversal_inorder(t->root, arr, n, &idx);
  return 0;
}
