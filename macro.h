int macro_handling(token_t *anker, status_t *stat);
int end_macro_handling(token_t *anker, status_t *stat);
int initMacroAnker(macro_definition_t **anker);
void listAllMacros(macro_definition_t *macros);
int saveMacro(char *name, macro_parms *parms, status_t *body);
