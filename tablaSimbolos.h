struct elemento {
    char *lexema;
    int componenteLexico;
};
struct entry_s {
    char *key; //Lexema
    int value; //Componente lexico
    struct entry_s *next;
};
typedef struct entry_s entry_t;

struct hashtable_s {
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;
void inicializarTablaSimbolos();
int comprobarTabla(char *key);
void insertarTabla(char *key, int value);
int returnTabla(char *key);
void imprimirTaboa();
