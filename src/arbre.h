
typedef struct node{
  typedef struct node
  {
    struct node *left;
    struct node *right;
    int tokcode;
    char *token;
    char position;//position par rapport au noeud pere (fils droit ou fils guache)
    char *reg;  //le reg du neud, utilisé par mknode

    /*
      char *reg1; //la partie destinataire du code assembleur, utilisé par print tree
      char *reg2; //la partie src du code assembleur, utilisé par print tree 
    */

  } node;

  
  node *mknode(node *left, node *right, int tokcode, char *token);
  void printtree(node *tree);
  void generate(node *tree);
