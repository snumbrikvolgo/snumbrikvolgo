#include <errno.h>
#include <assert.h>
#include "language.h"

node_t* MakeNode(const int key, const elem_t value, node_t* left, node_t* right);
node_t* HacerUnoNodo(const int key, const elem_t value);
int Existe(const char* name, vars_t var);
node_t* MakeNode(const int key, const elem_t value, node_t* left, node_t* right);

const char* s = NULL;

double getN();
char* getV();
node_t* getE(vars_t vars);
node_t* getP(vars_t vars);
node_t* getT(vars_t vars);
node_t* getExp(vars_t vars);
node_t* getAsg(vars_t vars);
node_t* getSi(vars_t vars);
node_t* getLupa(vars_t vars);
node_t* getScan(vars_t vars);
node_t* getPrint(vars_t vars);
node_t* getOp(vars_t vars);
node_t* getFunc();
node_t* getLlame(vars_t vars);
node_t* getDesp(vars_t vars);
node_t* getG(const char* str);
void SkipSpaces();
void skipPunct();

void skipSpaces()
{
  while(*s == ' ' || *s == '\n')
    s++;
  return;
}

void skipPunct()
{
  while(*s == '.' || *s == ',')
    s++;
  return;
}

node_t* getG(const char* str)
{
  s = str;
  skipSpaces();

  node_t* root = 0;
  while(*s == 'f' && *(s + 1) == 'u' && *(s + 2) == 'n' && *(s + 3) == 'c' &&
        *(s + 4) == 'i' && *(s + 5) == 'o' && *(s + 6) == 'n' && !isalpha(*(s + 7)))
        {
          s += 7;
          skipSpaces();

          if (!root)
              root = getFunc();
          else
              root = OPERAT(root, getFunc());

          skipSpaces();
        }
    skipSpaces();
    printf("last %c\n", *s);
    assert(*s == '\0');
    assert(s != str);

    s++;
    return root;
}

node_t* getFunc()
{
  node_t* value = 0;
  char* name = getV();
  printf("%s\n", name);

  int real = Existe(name, funcions);

  if (real != -1)
  {
    errno = NOFUNC;
    fprintf(stderr, "Hay no nombre de la funcion\n");
    exit(0);
  }

  else
  {
    real = funcions.numero;
    funcions.nombre[funcions.numero++] = name;
  }

  skipSpaces();

  vars_t vars = {};

  if (*(s++) != '[')
  {
    errno =  SYNTERROR;
    fprintf(stderr, "Esperado '[' in %s\n", name);
    exit(0);
  }

  skipSpaces();
  int exists = 0;

  while(*s != ']')
  {
    char* var = getV();
    exists = Existe(var, vars);

    if (exists != -1)
    {
      errno = NOVAR;
      fprintf(stderr, "Hay no nombre de el variable\n");
      exit(0);
    }
    else vars.nombre[vars.numero++] = var;

    skipSpaces();
    printf("current = %c\n", *s);
    assert(*s == ',' && *(s + 1) != ']' || *s == ']');
    skipPunct();
    skipSpaces();
  }

  skipSpaces();
  if (*(s++) != ']')
  {
    errno =  SYNTERROR;
    fprintf(stderr, "Esperado ']' in %s\n", name);
    exit(0);
  }

  skipSpaces();

  value = getOp(vars);

  if (!strcmp("main", name))
      return OPERAT(FUNCT(-vars.numero), value);
  return OPERAT(FUNCT(real), value);

}

node_t* getOp(vars_t vars)
{
  node_t* value = 0;
  node_t* cur = 0;

  skipSpaces();

  if (*s == '<' && *(s + 1) == '\"')
  {
    s += 2;
    skipSpaces();
    value = getOp(vars);

    skipSpaces();

    while (*s != '\"' && *(s + 1) != '>')
    {
      if (*s == '\0')
      {
        errno = SYNTERROR;
        fprintf(stderr, "Hay no mensula del fin \n");
        exit(0);
      }

      cur = getOp(vars);
      skipSpaces();
      value = OPERAT(value, cur);

      skipSpaces();
    }
      s += 2;
  }
  else if (*s == 's' && *(s + 1) == 'i' && !isalpha(*(s + 2)))
  {
    value = getSi(vars);
    if (*s == 'o' && *(s + 1) == 't' && *(s + 2) == 'r' && *(s + 3) == 'o'
        && !isalpha(*(s + 4)))
        {
          s += 4;
          skipSpaces();

          char* pointer = s;

          node_t* node = getOp(vars);

          if (s == pointer)
          {
            errno = SYNTERROR;
            fprintf(stderr, "Otro sin condicional\n");
            exit(0);
          }

          value = OTROS(value, node);
        }
  }
  else if (*s == 'o' && *(s + 1) == 't' && *(s + 2) == 'r' && *(s + 3) == 'o'
      && !isalpha(*(s + 4)))
            {
              errno = SYNTERROR;
              fprintf(stderr, "Tienes solo otro sin 'si'\n");
              exit(0);
            }
  else if (*s == 'm' && *(s + 1) == 'i' && *(s + 2) == 'e' && *(s + 3) == 'n' &&
          *(s + 4) == 't' && *(s + 5) == 'r' && *(s + 6) == 'a' && *(s + 7) == 's' &&
          !isalpha(*(s + 8)))
            {
              value = getLupa(vars);
            }
  else if (*s == 'e' && *(s + 1) == 's' && *(s + 2) == 'c' && *(s + 3) == 'r' &&
           *(s + 4) == 'i' && *(s + 5) == 'b' && *(s + 6) == 'e' && !isalpha(*(s + 7)))
           {
             value = getPrint(vars);
           }
   else if (*s == 'l' && *(s + 1) == 'e' && *(s + 2) == 'e' && !isalpha(*(s + 3)))
            {
              value = getScan(vars);
            }
    else if(*s == 'l' && *(s + 1) == 'l' && *(s + 2) == 'a' && *(s + 3) == 'm' &&
            *(s + 4) == 'a' && !isalpha(*(s + 5)))
            {
              value = getLlame(vars);
            }
    else if(*s == 'd' && *(s + 1) == 'e' && *(s + 2) == 's' && *(s + 3) == 'p' &&
            *(s + 4) == 'a' && *(s + 5) == 'c' && *(s + 6) == 'i' && *(s + 7) == 't' &&
            *(s + 8) == 'o' && !isalpha(*(s + 9)))
            {
              value = getDesp(vars);
            }
    else if (isalpha(*s))
            {
              value = getAsg(vars);
            }
    skipSpaces();

    return value;
}

node_t* getLlame(vars_t vars)
{
  s += 5;
  skipSpaces();

  char* name = getV();
  int real = Existe(name, funcions);

  if (real == -1)
  {
    errno = NOFUNC;
    fprintf(stderr, "Hay no nombre de la funcion\n");
    exit(0);
  }

  skipSpaces();
  if (*s++ != '[')
  {
    errno = SYNTERROR;
    fprintf(stderr, "Esperado '[' en vocacion %s\n", name);
    exit(0);
  }
  int exists = 0;
  skipSpaces();
  node_t* value = 0;
  while (*s != ']')
  {
    char* var = getV();
    exists = Existe(var, vars);
    if (exists == -1)
    {
      errno = NOVAR;
      fprintf(stderr, "Hay no nombre de el variable\n");
      exit(0);
    }
    else
    {
      if (value)
        value = PARAMET(value, VARIABLE(exists));

      else
        value = VARIABLE(exists);
    }
    skipSpaces();
    assert(*s == ',' && *(s + 1) != ']' || *s == ']');
    skipPunct();
    skipSpaces();
  }
  if (!value -> left && !value -> right)
    value = PARAMET(value, 0);

  skipSpaces();

  if (*(s++) != ']')
  {
    errno =  SYNTERROR;
    fprintf(stderr, "Esperado ']' en %s\n", name);
    exit(0);
  }

  skipSpaces();
  printf("%c\n", *s);
  if (*(s++) != ';')
  {
    errno = SYNTERROR;
    fprintf(stderr, "No hay fin de terminacion");
    exit(0);
  }

  skipSpaces();

  return MakeNode(OP, LLAME, value, FUNCT(exists));
}
node_t* getDesp(vars_t vars)
{
  s += 9;
  CONDICIONAL(despacito)


  return DESPAC(value, value2);

}
node_t* getSi(vars_t vars)
{
  s += 2;
  CONDICIONAL(si)

  return CONDIC(value, value2);
}

node_t* getLupa(vars_t vars)
{
  s += 8;
  CONDICIONAL(mientras)
  return LUPA(value, value2);
}

node_t* getScan(vars_t vars)
{
  s += 3;
  RECIBE(lee)

  return ENTRADA(VARIABLE(num));
}

node_t* getPrint(vars_t vars)
{
  s += 7;
  RECIBE(escribe)

  return SALIDA(VARIABLE(num));
}

node_t* getAsg(vars_t vars)
{
  skipSpaces();

  char* name = getV();
  int real = Existe(name, vars);

  if (real == -1)
  {
    errno = NOVAR;
    fprintf(stderr, "Hay no nombre de el variable\n");
    exit(0);
  }

  node_t* cur = VARIABLE(real);

  skipSpaces();

  if (*(s++) != '=')
  {
    errno = SYNTERROR;
    fprintf(stderr, "Esperado igual\n");
    exit(0);
  }

  skipSpaces();

  node_t* exprecion = getE(vars);

  skipSpaces();

  if (*(s++) != ';')
  {
    errno = SYNTERROR;
    fprintf(stderr, "No hay fin de terminacion in assgn");
    exit(0);
  }
  return ASSGN(exprecion, cur);
}

node_t* getExp(vars_t vars)
{
  node_t* value = getE(vars);

  skipSpaces();

  if(*s == '>' && *(s + 1) == '=')
    COMCMD(>=, EMAS, 2)
  else if(*s == '<' && *(s + 1) == '=')
    COMCMD(<=, EMENOR, 2)
  else if(*s == '<' && *(s + 1) == '>')
    COMCMD(<>, NIGUAL, 2)
  else if(*s == '=' && *(s + 1) == '=')
    COMCMD(==, IGUAL, 2)
  else if(*s == '>')
    COMCMD(>, MAS, 1)
  else if(*s == '<')
    COMCMD(<, MENOR, 1)

  else
    return value;
}

node_t* getE(vars_t vars)
{
  skipSpaces();

  char op = 0;
  node_t* value = getT(vars);
  char* pos = 0;
  skipSpaces();

  while (*s == '+' || *s == '-')
  {
    op = *s;
    s++;

    skipSpaces();
    pos = s;

    node_t* val2 = getT(vars);

    skipSpaces();

    assert(pos != s);

    if (op == '+')
      {
      value =  PLUS(value, val2);
    }
    if (op == '-')
      value =  MINUS(value, val2);
  }
  return value;
}

node_t* getT(vars_t vars)
{
  skipSpaces();

  char op = 0;
  node_t* val = getP(vars);
  char* pos = 0;
  skipSpaces();

  while (*s == '*' || *s == '/')
  {
    op = *s;
    s++;

    skipSpaces();
    pos = s;

    node_t* val2 = getP(vars);

    skipSpaces();

    assert(pos != s);

    if (op == '*')
      val =  MULTIP(val, val2);
    if (op == '/')
      val =  DIVIS(val, val2);
  }

  return val;
}

node_t* getP(vars_t vars)
{
  node_t* value = 0;
  int real = 0;

  skipSpaces();

  if (*s == '(')
  {
    s++;

    skipSpaces();

    value = getE(vars);

    skipSpaces();

    assert(*(s++) == ')');

    skipSpaces();

    return value;
  }

  else if (*s == 's' && *(s + 1) == 'i' && *(s + 2) == 'n')
  {
    s += 3;
    skipSpaces();

    value = SINUS(getP(vars));

    skipSpaces();
  }
  else if (*s == 'c' && *(s + 1) == 'o' && *(s + 2) == 's')
  {
    s += 3;
    skipSpaces();

    value = COSINUS(getP(vars));

    skipSpaces();
  }
  else if (*s == 's' && *(s + 1) == 'q' && *(s + 2) == 'r' && *(s + 3) == 't')
  {
    s += 4;
    skipSpaces();

    value = RADICAL(getP(vars));

    skipSpaces();
  }
  else if(isalpha(*s))
  {
    skipSpaces();

    char* name = getV();
    real = Existe(name, vars);

    if (real == -1)
    {
      errno = NOVAR;
      fprintf(stderr, "Hay no nombre de el variable\n");
      exit(0);
    }

    value = VARIABLE(real);

    skipSpaces();

  }
  else if(*s >= '0' && *s <= '9' || *s == '+' || *s == '-')
  {
    skipSpaces();

    value = NUMBER(getN());

    skipSpaces();

  }
  return value;

}

double getN()
{
  double val = 0;
  double point = 0;
  int rate = 0;

  skipSpaces();

  while('0' <= *s && *s <= '9')
  {
    val = val * 10 + (*s) - '0';
    s++;
  }
  if (*s == '.')
  {
    s++;
    while('0' <= *s && *s <= '9')
    {
      rate++;
      point = point * 10 + (*s) - '0';
      s++;
    }
  }
  val = val + (point)/(pow(10, rate));

  skipSpaces();

  return val;
}

char* getV()
{
  char* name = (char*) calloc (NAME_SIZE, sizeof(char));

  int length = 0;
  while (isalpha(*s) || isdigit(*s) || *s == '_')
  {
    name[length++] = *(s++);
  }

  name[length] = '\0';

  return name;
}

int Existe(const char* name, vars_t vars)
{
  for (int i = 0; i < vars.numero; i++)
  {
    if (!strcmp(name, vars.nombre[i]))
      return i;
  }

  return -1;
}


node_t* MakeNode(const int key, const elem_t value, node_t* left, node_t* right)
{
  node_t* node = (node_t*) calloc (1, sizeof(*node));

  node -> key = key;
  node -> elem = value;

  node -> left = left;
  if (left) node -> left -> parent = node;
  node -> right = right;
  if (right) node -> right -> parent = node;
  return node;

}
