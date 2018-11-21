#include <stdio.h>
#include <math.h>
#include "work_file_strings.h"

#define C_FREE(p)       \
    do                  \
    {                   \
        free(p);        \
        p = NULL;       \
    } while(0)


#define MAX_LABEL_NUM  20
#define MAX_LABEL_NAME 10

#define CMD(name, num) CMD_##name = num,
#define CMD_COMPLEX(name, num) CMD_COMPLEX_##name = num,
#define CMD_REG(name, reg_name, num) CMD_REG_##name = num,
#define CMD_JMP(name, num) CMD_JMP_##name = num,
    

enum asm_commands
{
#include "commands.h"            
};

#undef CMD_REG
#undef CMD
#undef CMD_COMPLEX
#undef CMD_JMP

struct names
{
    char label_name[MAX_LABEL_NAME];
    int label_address;  
};


typedef struct labels
{
    struct names names[MAX_LABEL_NUM];
    int size;
} label_t;

int read_code()
{
    FILE * file = fopen("debug.txt", "w");
    FILE * input = fopen("polsk1", "rw");

    const char* name = "polsk1";
    char * buffer =  read_file(name);
    int file_size = size_of_file(input);

    char* code = (char*)calloc(file_size + 1, sizeof(*code));
    int ptr = 0;
    int index = 0;
    int cycle = 0;
    label_t labels = {0};
    

   int quantity = 0;
    while(quantity < 2)
       {    
             printf("hui");
            while(*(buffer + ptr) != '\0')
        {
            if (*(buffer + ptr) == ':')
                {   
                    ptr++;
                    int counter = 0; 
                    sscanf((buffer + ptr), "%s%n", labels.names[labels.size].label_name, &counter);
                    labels.names[labels.size].label_address = index;
                    ptr += counter;
                    (labels.size)++;
                }  
                           
            
            #define CMD(name, num) \
            if(strncmp(buffer + (ptr), #name, strlen(#name)) == 0) \
            { \
                ptr += strlen(#name) * sizeof(char);\
                code[index++] = num;\
            }  
 
           #define CMD_REG(name, reg_name, num)  \
            if(strncmp(buffer + (ptr), #name, strlen(#name)) == 0) \
            { \
                ptr += strlen(#name) * sizeof(char);\
                code[index++] = num;\
                while (isspace(*(buffer + ptr)))\
                    {\
                        ptr++;\
                    }\
                if (strncmp(buffer + (ptr), "RAX", strlen("RAX")) == 0) \
                    {\
                        code[index++] = '1';\
                    }\
                if (strncmp(buffer + (ptr), "RBX", strlen("RBX")) == 0) \
                    {\
                        code[index++] = '2';\
                    }\
                if (strncmp(buffer + (ptr), "RCX", strlen("RCX")) == 0) \
                    {\
                        code[index++] = '3';\
                    }\
                if (strncmp(buffer + (ptr), "RDX", strlen("RDX")) == 0) \
                    {\
                        code[index++] = '4';\
                    }\
                ptr += 3 * sizeof(char);\
            }

           #define CMD_COMPLEX(name, num)   \
               if(strncmp(buffer + (ptr), #name, strlen(#name)) == 0) \
            { \
                ptr += strlen(#name)*sizeof(char);\
                code[index++] = num;\
                while (isspace(*(buffer + ptr)))\
                    {\
                        ptr++;\
                    }\
                double value = 0;                                       \
                int counter = 0;                                        \
                sscanf(buffer + (ptr), " %lg %n", &value, &counter);    \
                memcpy(code + (index), &value, sizeof(double));         \
                index += sizeof(double);                                \
                ptr += counter;                                         \
            }
       
           #define CMD_JMP(name, num) \
               if(strncmp(buffer + (ptr), #name, strlen(#name)) == 0) \
            { \
                ptr += strlen(#name)*sizeof(char);\
                code[index++] = num;\
                while (isspace(*(buffer + ptr)))\
                    {\
                       ptr++;\
                    }\
                int numeros = 0;\
                char nombre[MAX_LABEL_NAME] = {};\
                sscanf(buffer + ptr, "%s%n", nombre, &numeros);\
                for (cycle = 0; cycle < labels.size; cycle++)\
                    {\
                        if (strncmp(labels.names[cycle].label_name, nombre, numeros) == 0)\
                            {\
                                memcpy(code + index++, &labels.names[cycle].label_address, sizeof(char));\
                            }\
                        else {\
                                int tmp = -1;\
                                memcpy(code + index, &tmp, sizeof(char));\
                              }\
                    }\
                ptr += numeros;\
            }
             
           #include "commands.h"    
           #undef CMD
           #undef CMD_COMPLEX    
           #undef CMD_REG
           #undef CMD_JMP
                             

            while (isspace(*(buffer + ptr)))
                {
                    ptr++;
                } 
             
                
        }   
            ptr = 0;
            quantity++;
            index = 0;
            cycle = 0;
            labels.size = 0;
            printf("name %s, adr %d\n", labels.names[0].label_name, labels.names[0].label_address);
            printf("name %s, adr %d\n", labels.names[1].label_name, labels.names[1].label_address);
        }
    fwrite(code, sizeof(char), file_size, file);
    fclose(file);
    fclose(input);
    C_FREE(buffer);
    C_FREE(code);
       return 0;
    
}
