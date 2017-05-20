int jsoneq(char *json, jsmntok_t *tok, const char *s);
int parseVarObject(char *json_str, jsmntok_t *tokens, int offset);
int loadJson(char *filepath);
int handleJsonString_PRIMITIVE(char *varname, char *value);
int convertToInt(char *str):
double convertToDouble(char *str);

int handleOneDArray(char *varname, jsmntok_t *tokens, int offset, char *json_str);
int handleTwoDArray(char *varname, jsmntok_t *tokens, int offset, char *json_str);
int handleThreeDArray(char *varname, jsmntok_t *tokens, int offset, char *json_str);
int handleJsonArray(char *varname, jsmntok_t *tokens, int offset, char *json_str);
