#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL; //no hay memoria
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *aux = tree->root;
    
    if (tree->root == NULL) {
        TreeNode *new = createTreeNode(key, value);
        tree->root = new;
        tree->current = new;
        return;
    }

    while (1){
        if (is_equal(tree, key, aux->pair->key)){
            return;
        }
        if (tree->lower_than(key, aux->pair->key) == 1){
            if (aux->left == NULL) {
                TreeNode *new = createTreeNode(key, value);
                aux->left = new;
                new->parent = aux;
                tree->current = new;
                return;
            }
            aux = aux->left;
        }
        else{
            if (aux->right == NULL) {
                TreeNode *new = createTreeNode(key, value);
                aux->right = new;
                new->parent = aux;
                tree->current = new;
                return;
            }
            aux = aux->right;
        }
    }
    
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    
    //caso sin hijos
    if (node->left == NULL && node->right == NULL){
        if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;

        free(node->pair);
        free(node);
        return;
    }
    //caso dos hijos
    if(node->left != NULL && node->right != NULL){
        TreeNode *aux = minimum(node->right);
        node->pair = aux->pair;
        removeNode(tree, aux);
        return;
    }


    
    //caso un hijo por la derecha
    if(node->left == NULL || node->right != NULL){
        if (node->parent->left == node) {
            node->parent->left = node->right;
            node->right->parent = node->parent;
            free(node->pair);
            free(node);
            return;

        }
        node->parent->right = node->right;
        node->right->parent = node->parent;
        free(node->pair);
        free(node);
        return;        
    }
    //caso un hijo por la izquierda
    if(node->left != NULL || node->right == NULL){
        if (node->parent->left == node) {
            node->parent->left = node->left;
            node->left->parent = node->parent;
            free(node->pair);
            free(node);
            return;

        }
        node->parent->right = node->left;
        node->left->parent = node->parent;
        free(node->pair);
        free(node);
        return; 
    }
    


    
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }

        if(tree->lower_than(key, aux->pair->key) == 1) aux = aux->left;
        else aux = aux->right;      

    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    TreeNode *prev = NULL;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key))
            return aux->pair;
        if(tree->lower_than(key, aux->pair->key) == 1){
            prev = aux;
            aux = aux->left;
        }
            
        else {
            prev = aux;
            aux = aux->right;
        }
    }
    aux = prev;
    prev = tree->current;
    tree->current = aux;
    Pair *par = nextTreeMap(tree);
    tree->current = prev;
    return par;

}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->root;
    while (aux->left != NULL){
        aux = aux->left;
    }
    
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->current;
    if (aux->right != NULL){
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    while (aux->parent != NULL){
        if (aux->parent->left == aux){
            tree->current = aux->parent;
            return aux->parent->pair;
        }
        else aux = aux->parent;
    }
    return NULL;   
}
