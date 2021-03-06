void printsingleBoolean(bool val, FILE *fp, bool json) ;
void printBoolean(vars_t *var, FILE *fp, bool json) ;
void printRawBoolean(vars_t *var, FILE *fp) ;
void print1DBoolean(vars_t *var, FILE *fp, bool json) ;
void printRaw1DBoolean(vars_t *var, FILE *fp) ;
void print1DBooleanWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void print2DBoolean(vars_t *var, FILE *fp, bool json) ;
void printRaw2DBoolean(vars_t *var, FILE *fp) ;
void print2DBooleanWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void print3DBoolean(vars_t *var, FILE *fp, bool json) ;
void printRaw3DBoolean(vars_t *var, FILE *fp) ;
void print3DBooleanWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void printFloat(vars_t *var, FILE *fp, bool json) ;
void printRawFloat(vars_t *var, FILE *fp) ;
void print1DFloat(vars_t *var, FILE *fp, bool json) ;
void print1DFloatWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void print2DFloat(vars_t *var, FILE *fp, bool json) ;
void printRaw2DFloat(vars_t *var, FILE *fp) ;
void print2DFloatWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void print3DFloat(vars_t *var, FILE *fp, bool json) ;
void printRaw3DFloat(vars_t *var, FILE *fp) ;
void printRaw3DFloatWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void printInteger(vars_t *var, FILE *fp, bool json) ;
void printRawInteger(vars_t *var, FILE *fp) ;
void print1DInteger(vars_t *var, FILE *fp, bool json) ;
void printRaw1DInteger(vars_t *var, FILE *fp) ;
void print1DIntegerWithXIndex(vars_t *var, FILE *fp, int x, int json) ;
void print2DInteger(vars_t *var, FILE *fp, bool json) ;
void printRaw2DInteger(vars_t *var, FILE *fp) ;
void print2DIntegerWithXIndex(vars_t *var, FILE *fp, int x_index, int json) ;
void print3DInteger(vars_t *var, FILE *fp, bool json) ;
void printRaw3DInteger(vars_t *var, FILE *fp) ;
void print3DIntegerWithXIndex(vars_t *var, FILE *fp, int x_index, int json) ;
void print3DIntegerWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index, int json) ;
void printAllVars(vars_t *anker) ;
void printAllVarsToFile(vars_t *anker, FILE *fp) ;
void printAllVarsJSON(vars_t *anker) ;
void printAllVarsToFileJSON(vars_t *anker, FILE *fp) ;
void printAllVarsToFile_in(vars_t *anker, FILE *fp, int offset, bool json) ;
void printGroup(vars_t *var, FILE *fp, bool json) ;
void printString(vars_t *var, FILE *fp, bool json) ;
void printRawString(vars_t *var, FILE *fp) ;
void print1DString(vars_t *var, FILE *fp, bool json) ;
void printRaw1DString(vars_t *var, FILE *fp) ;
void print1DStringWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void print2DString(vars_t *var, FILE *fp, bool json) ;
void printRaw2DString(vars_t *var, FILE *fp) ;
void print2DStringWithXIndex(vars_t *var, FILE *fp, int x_index, bool json) ;
void print3DString(vars_t *var, FILE *fp, bool json) ;
void printRaw3DString(vars_t *var, FILE *fp) ;
