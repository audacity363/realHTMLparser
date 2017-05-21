import json
from py_vars import definition

class Variables:
    parser_vars = []

    def __init__(self):
        for i in range(3):
            self.parser_vars.append(definition.Variable(i, "var"+str(i)))
            self.parser_vars[i].generate_value()

    def gen_json(self):
        json_dic = {"vars": []}
        for var in self.parser_vars:
            json_dic["vars"].append(var.get_dic())
        return json.dumps(json_dic, sort_keys=True)
    
    def get_value(self, name):
        for var in self.parser_vars:
            if var.get_name() == name:
                return var.get_value()
        return None
    
    def add_new_var(self, type, value, length = [0, 0, 0]):
        name = "var"+str(len(self.parser_vars)+1)
        self.parser_vars.append(definition.Variable(type, name, value, length[0],
            length[1], length[2]))
