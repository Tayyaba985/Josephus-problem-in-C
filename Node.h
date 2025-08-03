struct person
{
   char name[20];
   int num;
};

struct node
{

    struct person data;

    struct node * next;
    struct node * prev;
};
