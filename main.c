/*
Jakub Karwacki, WME23BX1S1
Data utworzenia: 2024.12.07, rozliczono: 2024.12.07
Zadanie 2: Program realizujący drzewo BST (Binary Search Tree) z możliwością dodawania, usuwania i wyświetlania elementów.
*/

// Libraries
    #include <stdio.h>
    #include <stdlib.h>

// Structs
    typedef struct Node {
        int value;
        struct Node *parent;
        struct Node *left;
        struct Node *right;
    } N;

// Prototypes
    // Functions
        int isEmpty(N *root);

    // Node operations
        int showNode(N *node, int mode);
        int addNode(N **root, int v);
        N* find_node(N *root, int v);
        N* find_min(N *root);
        int deleteNode(N **root, int v);

    // Tree operations
        int freeTree(N **root);

    // Traversal
        int vlr(N *root);
        int lvr(N *root);
        int lrv(N *root);
        
    // Menu
        int menu();

// Functions
    // General
        int isEmpty(N *root){ root ? ({ return 1; }) : ({ return 0; }); }
    // Node operations
        int showNode(N *node, int mode){
            node ? ({
                mode == 1 ? ({ printf("node: %p,\n v: %i,\n rodzic: %p,\n left: %p,\n right: %p\n\n", (void*)node, node->value, (void*)node->parent, (void*)node->left, (void*)node->right); }) : ({ printf("node: %p,\n v: %i,\n", (void*)node, node->value); });
                return 1;
            }) : ({ printf("Wezel jest pusty (NULL)\n"); return 0; });
            }

        int addNode(N **root, int v){
            printf("addNode: Dodaj nowy wezel do drzewa (wartosc: %i)\n", v);

            N *new_node = (N*)malloc(sizeof(N));

            new_node ? ({
                new_node->value = v;
                new_node->left = NULL;
                new_node->right = NULL;

                *root ? ({
                    N *current = *root;
                    N *parent = NULL;

                    while (current != NULL) {
                        parent = current;

                        current->value != v ? ({ current->value > v ? (current = current->left) : (current = current->right); }) : ({
                            printf("Niepowodzenie: Wartosc juz istnieje w drzewie\n");

                            showNode(current, 0);
                            free(new_node);

                            return 0;
                        });
                    }

                    new_node->parent = parent;
                    parent->value > v ? (parent->left = new_node) : (parent->right = new_node);

                    printf("Sukces: Dodano nowy wezel do drzewa\n");

                    showNode(new_node, 0);
                    }) : ({
                        new_node->parent = NULL;
                        *root = new_node;

                        printf("Sukces: Utworzono korzen drzewa\n");

                        showNode(new_node, 0);
                    }); return 1; 
                }) : ({ printf("Niepowodzenie: Blad alokacji pamieci\n"); return 0; });}

        N* find_node(N *root, int v) { while(root != NULL && root->value != v) { root = root->value > v ? root->left : root->right; }; return root; }
        N* find_min(N *root) { root ? ({ while(root->left != NULL) { root = root->left; };  return root; }) : ({ return NULL; }); }
        int deleteNode(N **root, int v) {
            printf("deleteNode: Proba usuniecia wezla o wartosci %i\n", v);

            *root ? ({
                N *node_to_delete = find_node(*root, v);

                node_to_delete ? ({
                    node_to_delete->left ? ({
                        node_to_delete->right ? ({ // Child node (left and right)
                            printf("Usuwam wezel z dwoma dziecmi o wartosci: %i\n", node_to_delete->value);

                            N *successor = find_min(node_to_delete->right);
                            int *successor_val = successor->value;

                            printf("Znaleziono nastepnika o wartosci: %i, usuwam go aby zastapic nim wartosc aktualnego wezla\n", successor_val);

                            deleteNode(root, successor_val);
                            node_to_delete->value = successor_val;

                            printf("Zastapiono wartosc wezla (pierwotnie: %i) wartoscia nastepnika: %i\n", v, successor_val);
                        }) : ({ // Child node (left)
                            N *child = node_to_delete->left;
                            child->parent = node_to_delete->parent;
                            node_to_delete->parent ? ({ node_to_delete->parent->left == node_to_delete ? ({ node_to_delete->parent->left = child; }) : ({ node_to_delete->parent->right = child; }); }) : ({ *root = child; });

                            printf("Usunieto wezel o jednym dziecku (lewym) o wartosci: %i\n", node_to_delete->value);

                            showNode(node_to_delete, 0);
                            free(node_to_delete);
                        });
                    }) : ({ node_to_delete->right ? ({ // Child node (right)
                            N* child = node_to_delete->right;
                            child->parent = node_to_delete->parent;
                            node_to_delete->parent ? ({ node_to_delete->parent->left == node_to_delete ? ({ node_to_delete->parent->left = child; }) : ({ node_to_delete->parent->right = child; }); }) : ({ *root = child; });

                            printf("Usunieto wezel o jednym dziecku (prawym) o wartosci: %i\n", node_to_delete->value);

                            showNode(node_to_delete, 0);
                            free(node_to_delete);
                        }) : ({ // Leaf node
                            node_to_delete->parent ? ({ node_to_delete->parent->left == node_to_delete ? ({ node_to_delete->parent->left = NULL; }) : ({ node_to_delete->parent->right = NULL; }); }) : ({ *root = NULL; });

                            printf("Usunieto wezel lisc o wartosci: %i\n", node_to_delete->value);

                            showNode(node_to_delete, 0);
                            free(node_to_delete);
                        });
                    }); return 1;
                }) : printf("Nie znaleziono wezla o wartosci %i w drzewie.\n", v); return 0;
            }) : ({ printf("Drzewo jest puste - brak elementow do usuniecia.\n"); return 0; }); }
        
    // Tree operations
        int freeTree(N **root) {
        root ? ({
            freeTree(&(*root)->left);
            freeTree(&(*root)->right);
            printf("Usunieto wezel o wartosci: %i\n", (*root)->value);
            showNode(*root, 0);
            free(*root);
            *root = NULL;
            return 1;
        }) : ({ return 0; }); }

    // Traversal
        int vlr(N *root) { root ? ({ showNode(root, 1); vlr(root->left); vlr(root->right); return 1; }) : ({ return 0; }); }
        int lvr(N *root) { root ? ({ lvr(root->left); showNode(root, 1); lvr(root->right); return 1; }) : ({ return 0; }); }
        int lrv(N *root) { root ? ({ lrv(root->left); lrv(root->right); showNode(root, 1); return 1; }) : ({ return 0; }); }
    // Menu
        int menu() {
        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            "Drzewo BST",
            "Oznaczenia w algorytmach przejscia drzewa:",
            "V - odwiedzenie wezla; L - przejscie do lewego poddrzewa; R - przejscie do prawego poddrzewa",
            "Menu:",
            "1. Wartosc typu INT do przechowania",
            "2. Przejscie drzewa algorytmem VLR",
            "3. Przejscie drzewa algorytmem LVR",
            "4. Przejscie drzewa algorytmem LRV",
            "5. Usuwanie zawartosci drzewa",
            "6. Wyczyszczanie drzewa",
            "0. Wyjdz"
        ); return 0; }

    // Main
        int main(){
            N *root = NULL;
            int choice = 0;

            do {
                menu();
                scanf_s("%d", &choice);

                switch (choice) {
                    case 0: { break; }
                    case 1: { int n; printf("Wartosc typu INT do przechowania\n?> "); scanf("%i", &n); addNode(&root, n); break; }
                    case 2: { !isEmpty(root) ? ({ printf("Przejscie drzewa algorytmem VLR\n"); vlr(root); }) : ({ printf("Drzewo jest puste\n"); }); break; }
                    case 3: { !isEmpty(root) ? ({ printf("Przejscie drzewa algorytmem LVR\n"); lvr(root); }) : ({ printf("Drzewo jest puste\n"); }); break; }
                    case 4: { !isEmpty(root) ? ({ printf("Przejscie drzewa algorytmem LRV\n"); lrv(root); }) : ({ printf("Drzewo jest puste\n"); }); break; }
                    case 5: { !isEmpty(root) ? ({ int del_val; printf("Podaj wartosc wezla do usuniecia:\n?> "); scanf("%i", &del_val); deleteNode(&root, del_val); }) : ({ printf("Drzewo jest puste\n"); break; }); }
                    case 6: { !isEmpty(root) ? ({ printf("Usuwanie zawartosci drzewa...\n"); freeTree(&root); printf("Usunieto zawartosc drzewa\n");}) : ({ printf("Drzewo jest puste\n"); }); }
                    default: { printf("Nieprawidlowa opcja.\n"); }
                }
            } while(choice != 0);
            
            return 0; }