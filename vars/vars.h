#define true 1
#define false 0

#define VAR_ALREADY_DEFINED 1
#define MEMORY_ALLOC_ERROR 2
#define VAR_NOT_DEFINED 3
#define GRP_NOT_DEFINED 4
#define GRP_ALREADY_DEFINED 5
#define X_INDEX_OUT_OF_RANGE 6
#define Y_INDEX_OUT_OF_RANGE 7
#define Z_INDEX_OUT_OF_RANGE 9
#define ANKER_ALREADY_INIT 8
#define VAR_VALUE_TO_LONG 10
#define WRONG_VAR_TYPE 11
#define BUFFER_TO_SAMLL 12


struct variables_s
{
    char *name;
    void *data;
    int length;
    size_t x_length;
    size_t y_length;
    size_t z_length;
    int type;
    struct variables_s *next_lvl;
    struct variables_s *prev;
    struct variables_s *next;
};

enum {
    GROUP,
    INTEGER,
    ONEDINTEGER,
    TWODINTEGER,
    THREEDINTEGER,
    STRING,
    ONEDSTRING,
    TWODSTRING,
    THREEDSTRING,
    BOOL,
    ONEDBOOL,
    TWODBOOL,
    THREEDBOOL,
    FLOAT,
    ONEDFLOAT,
    TWODFLOAT,
    THREEDFLOAT
};

typedef struct variables_s vars_t;
typedef unsigned short int bool;


//----------------------String function----------------------------------------
int getStringLength(vars_t *anker, char *group, char *name);
int addString(vars_t *anker, char *group, char *name, wchar_t *val, size_t length);
int getString(vars_t *anker, char *group, char *name, wchar_t *val, int length);
int create1DCharArrayFromString(vars_t *inanker, vars_t *outanker, char *group, 
                            char *name, char *new_name);
int create1DCharArrayFromString(vars_t *inanker, vars_t *outanker, char *group, 
                            char *name, char *new_name);
int add1DStringArray(vars_t *anker, char *group, char *name, int var_length, size_t x_length);
int edit1DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index);
int getStringFrom1DArray(vars_t *anker, char *group, char *name, wchar_t *val,
                         int length, int x_index);
int createNewVarFrom1DStringArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *new_name,
                                  int x_index);
int create1DCharArrayFrom1DStringArray(vars_t *inanker, vars_t *outanker,
                                       char *group, char *name, char *new_name,
                                       int x_index);
int add2DStringArray(vars_t *anker, char *group, char *name, int var_length, 
                     int x_length, int y_length);
int edit2DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index, int y_index);
int getStringFrom2DArray(vars_t *anker, char *group, char *name, wchar_t *val,
                         int length, int x_index, int y_index);
int create1DStringArrayFrom2DStringArray(vars_t *inanker, vars_t *outanker,
                                       char *group, char *name, char *new_name,
                                       int x_index);
int add3DStringArray(vars_t *anker, char *group, char *name, int var_length, 
                     int x_length, int y_length, int z_length);
int edit3DStringArray(vars_t *anker, char *group, char *name, wchar_t *val,
                      int x_index, int y_index, int z_index);
int getStringFrom3DArray(vars_t *anker, char *group, char *name, wchar_t *val,
                         int length, int x_index, int y_index, int z_index);
//----------------------End String function------------------------------------

//----------------------Integer function---------------------------------------
int getInteger(vars_t *anker, char *group, char *name, int *val);
//----------------------End Integer function-----------------------------------

//----------------------Float function---------------------------------------
int getFloat(vars_t *anker, char *group, char *name, double *val);
//----------------------End Float function-----------------------------------

//----------------------Utils function-----------------------------------------
int getVarType(vars_t *anker, char *group, char *name);
void printAllVarsJSON(vars_t *anker);
void printAllVarsToFileJSON(vars_t *anker, FILE *fp);
int printVarsToFileJSON(vars_t *anker, char **var_names, int length, FILE *fp);
//----------------------End Utils function-------------------------------------
