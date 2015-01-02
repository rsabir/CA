
typedef struct node{
  typedef struct node
  {
    struct node *left;
    struct node *right;
    int tokcode;
    char *token;
  } node;

  
  node *mknode(node *left, node *right, int tokcode, char *token);
  void printtree(node *tree);
  void generate(node *tree);
