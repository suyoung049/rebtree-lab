#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));

  // 모든 nil(leaf) 노드는 블랙
  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  // RB 트리에서 leaf 노드는 nil 노드
  p->root = nilNode;
  return p;
}

void left_rotation(rbtree *t, node_t *x){
  // x의 오른쪽 자식 노드 y 선언
  node_t *y = x->right;
  // y의 왼쪽 자식 노드를 x의 오른쪽 자식 노드로 변경
  x->right = y->left;
  //y의 왼족 자식 노드가 nil 노드가 아니면 y의 왼쪽 자식 노드의 부모 노드를 x로 변경
  if (y->left != t->nil)
    y->left->parent = x;
  // y의 부모 노드를 x의 부모 노드로 변경
  y->parent = x->parent;
  // x의 부모노드가 nil노드 이면, x가 root 노드, y를 root 노드로 변경
  if (x->parent == t->nil)
    t->root = y;
  // x가 부모 노드의 왼쪽 자식 노드이면 x의 부모 노드의 왼쪽 자식 노드를 y로 변경
  else if (x == x->parent->left)
    x->parent->left = y;
  // x가 부모 노드의 오른쪽 자식 노드이면 x의 부모 노드의 오른쪽 자식 노드를 y로 변경
  else 
    x->parent->right = y;
  // y의 왼쪽 자식 노드를 x로 변경
  y->left = x;
  // x의 부모 노드를 y로 변경
  x->parent = y;
}

// left_rotation 에서 right를 left로 변경
void right_rotation(rbtree *t, node_t *x){
  node_t *y =x->left;
  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  
  else
    x->parent->right = y;
  
  y->right = x;
  x->parent = y;
  
}
// 동적 메모리 할당 해제
// 동적 메모리 할당된 노드들의 메모리 할당 해제
void free_node(rbtree *t, node_t *x){
  // 노드의 왼족, 오른족 자식이 nill 노드일때 까지 함수 호출
  if (x->left != t->nil)
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  // 왼쪽, 오른쪽 자식이 nill 노드이면 끝 노드, 노드 메모리 할당 해제
  free(x);
  // x는 NULL로 초기화
  x = NULL;
}

// 동적 메모리 할당 해제
// 동적 메모리 할당된 트리의 nil노드와 트리의 메모리 할당 해제
void delete_rbtree(rbtree *t) {
  // t-> root가 t->nill 일때까지 free_node를 호출하면서 할당해재
  if (t->root != t->nil)
    free_node(t, t->root);  
  // 모든 노드 할당 해제 후에 t->null 해제
  free(t->nil);
  // 마지막으로 t를 해제
  free(t);
}

void rb_insert_fixup(rbtree *t, node_t *z) {
  //z의 색을 RED로 받았기 때문에 부모의 색이 RED가 아닐 때까지 반복문 실행
  while (z->parent->color == RBTREE_RED)
  { //만약 부모 노드가 할아버지 노드의 왼쪽이라면 삼촌 노드는 할아버지 노드의 오른쪽
    if (z->parent == z->parent->parent->left){
      node_t *uncle = z->parent->parent->right;
      // 만약 삼촌 노드의 칼라가 RED라면 case1 문제 
      if (uncle->color == RBTREE_RED){
        // 해결을 위해 부모노드와 삼촌 노드의 색은 검정으로 변경
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        // 할아버지 노드의 색은 빨강으로 변경
        z->parent->parent->color = RBTREE_RED;
        // 문제를 해결 한 후에는 할아버지 노드에서 부터 다시 확인을 시작해야 하므로 z 값을 할아버지 노드로 변경
        z = z->parent->parent;
      }
      else {
        // 만약 삼촌 노드의 칼라가 BLACK 이고, 내 위치가 부모노드의 오른쪽이라면 case 2 문제 
        if (z == z->parent->right){
          z = z->parent;
          // 해결을 위해 부모를 기준으로 왼쪽 회전 실행 -> case 3 문제로 변경
          left_rotation(t, z);
        }
        // 삼촌 노드의 칼라가 BLACK 이고, 내 위치가 부모노드의 왼쪽이라면 case 3 문제
        // 해결을 위해 부모 노드 칼라 BLACK으로 변경
        z->parent->color = RBTREE_BLACK;
        // 할아버지 노드 칼라 RED로 변경                
        z->parent->parent->color = RBTREE_RED;
        // 할아버지를 기준으로 오른쪽 외전 실행
        right_rotation(t, z->parent->parent);
      }
    }
    // left => right 로 방향 변경
    else{
      if (z->parent == z->parent->parent->right){
        node_t *uncle = z->parent->parent->left;
        if (uncle->color == RBTREE_RED){
          z->parent->color = RBTREE_BLACK;
          uncle->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else {
          if (z == z->parent->left){
            z = z->parent;
            right_rotation(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotation(t, z->parent->parent);
        }

      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

//삽입 노드는 항상 red 이다.
// node_t *rbtree_insert(rbtree *t, const key_t key) {
//   // TODO: implement insert
//   // NULL 값
//   node_t *y = t->nil;
//   // head 값
//   node_t *x = t->root;

//   // x 가 NULL 값을 가리킬때까지 반복문
//   while (x != t->nil)
//   {
//     // 부모 노드를 연결시키기 위해 부모 값 빈공간에 저장
//     y = x;
//     // 이미 삽입된 키라면 return
//     // if (x->key == key){
//     //   printf("같은 키가 있습니다.\n");
//     //   return x;
//     // }
//     //x의 key 값이 key보다 작을시 x -> right 이동
//     if (x->key <key)
//       x = x->right;
//     // 반대의 경우 x -> left 이동
//     else
//       x = x->left;
//   }
//   // 반복문을 나와서 x가 NULL 값을 가리키면 그때가 z를 삽입할 때
//   node_t *z = (node_t *)calloc(1, sizeof(node_t));
//   z->key = key;
//   // x는 nil을 가리키고 y는 그 위의 부모노드 z에 연결
//   z->parent = y;
  
//   // 애초에 트리가 비어 있다면 y는 t-> nil 그대로
//   if (y == t->nil)
//     // t의 root 값을 z로 저장
//     t->root = z;
//   // z의 key 값이 y의 key 값보다 작다면 y -> left에 저장
//   else if (z->key < y->key)
//     y->left = z;
//   // 반대라면 y -> right에 저장
//   else
//     y->right = z;

//   z->left = t->nil;
//   z->right = t->nil;
//   // 처음에 노드를 삽입 할때 red 노드로 저장
//   z->color = RBTREE_RED;
//   // red-black 트리 규칙에 맞게 함수 시작
//   rb_insert_fixup(t, z);
//   return z;
// }

//test 추가

node_t *rbtree_insert(rbtree *t, const key_t key){
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));

  z->key = key;

  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }

  z->parent = y;

  if (y == t->nil) {
    t->root = z;
  }
  else if (z->key < y->key) {
    y->left = z;
  }
  else {
    y->right = z;
  }
  
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rb_insert_fixup(t, z);

  return z;

}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *p = t->root;
  while (p != t->nil)
  {
    if (p->key == key)
      return p;
    
    else if (p->key < key)
      p = p->right;
    else
      p = p->left;
  }
  
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  if (t->root == t->nil)
    return NULL;
  
  node_t *p = t->root;

  while (p->left != t->nil)
  {
    p = p->left;
  }
  
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  if (t->root == t->nil)
    return NULL;

  node_t *p = t->root;

  while ( p->right != t->nil)
  {
    p = p->right;
  }
  
  return p;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil){
    t->root = v;
  }
  else if(u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
  // while 루프는 x가 루트 노드이거나, x의 칼라가 RED이면 반복문 탈출
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left){
      node_t *w = x->parent->right;

      // case 1
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;
      }
      // case  2
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // case 3
      else{
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t,w);
          w = x->parent->right;
        }

        // case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;
      }
    }
    //RIGHT CASE(root node를 기준으로 좌우 반대로 뒤집으면 뒤는 경우)
    else{
      node_t *w = x->parent->left;

      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }

      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }

      else
      {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->left->color = RBTREE_BLACK;
      right_rotation(t, x->parent);
      x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y;
  node_t *x;
  color_t y_origin_color;

  y = p;
  y_origin_color = y->color;

  if (p->left == t->nil){
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else {
    y = p->right;
    while (y->left != t->nil)
    {
      y = y->left;
    }
    y_origin_color = y->color;
    x = y->right;

    if (y->parent == p){
      x->parent = y;
    }
    else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_origin_color == RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }

  free(p);

  return 0;
}

void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count){
  if (curr == t->nil){
    return;
  }
  subtree_to_array(t, curr->left, arr, n, count);
  if (*count < n){
    // *count는 0, 1, 2, 3 .....이다. 그리고 후위 연산자 ++이므로 0부터 인덱스가 시작된다.
    arr[(*count)++] = curr->key;
  }
  else return;
  subtree_to_array(t, curr->right, arr, n, count);
}

// 오름 차순으로 배열에 값 저장
// rbtree를 가프키는 t 값(주소값)은 변하면 안되므로 const
// arr는 inorder 함수를 거칠 때 마다 변할 수 있으므로 const 없이
// n은 arr의 사이즈로 변하면 안되므로 const를 붙여준다. size_t는 unsigned int로, 어차피 배열의 사이즈 n은 양수 일테니 size_t로 선언
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t->root == t->nil){
    return 0;
  }

  size_t cnt = 0;
  subtree_to_array(t, t->root, arr, n, &cnt);
  return 0;

}
