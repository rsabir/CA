typedef struct list_element{
  char expression[200];
  struct list_element *suivant;
}List_element;

typedef struct list{
  List_element *debut;
  List_element *fin;
}List;

extern List *le;

List *list_initialise(void);
void list_ajouter(List *l, char *e);
void list_print(List *l);
void list_free(List *l);
