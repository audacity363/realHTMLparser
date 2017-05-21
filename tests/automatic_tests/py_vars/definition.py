import random
import string

STRING        = 0
ONEDSTRING    = 1
TWODSTRING    = 2
THREEDSTRING  = 3

BOOL          = 4
ONEDBOOL      = 5
TWODBOOL      = 6
THREEDBOOL    = 7

INTEGER       = 8
ONEDINTEGER   = 9
TWODINTEGER   = 10
THREEDINTEGER = 11

FLOAT         = 12
ONEDFLOAT     = 13
TWODFLOAT     = 14
THREEDFLOAT   = 15

class Variable:
    type = None
    name = None
    value = None
    array_length = [0, 0, 0]

    def __init__(self, type, name, value=None, x=0, y=0, z=0):
        self.type = type
        self.name = name
        self.value = value
        if type == ONEDSTRING and x == 0:
            x = random.randrange(0, 100)
        
        self.array_length = self.check_parms(type, x, y, z)

    def check_parms(self, type, x, y, z):
        result = [x, y, z]

        if (type == ONEDSTRING or type == TWODSTRING or 
                type == THREEDSTRING) and x == 0:
            result[0] = random.randrange(1,50)

        if (type == TWODSTRING or type == THREEDSTRING) and y == 0:
            result[1] = random.randrange(1,50)
        
        if(type == THREEDSTRING) and z == 0:
            result[2] = random.randrange(1,50)

        return result

    def generate_string(self, length=10):
        return''.join(random.choice(string.ascii_letters) for _ in range(length))

    def generate_value(self):
        print("Generate: [%s]\n\tx = %d\n\ty = %d\n\tz = %d\n" % (self.name,
            self.array_length[0], self.array_length[1], self.array_length[2]))
        if self.type == STRING:
            self.value = self.generate_string()
        elif self.type == ONEDSTRING:
            self.value = []
            for x in range(self.array_length[0]):
                self.value.append(self.generate_string())
        elif self.type == TWODSTRING:
            self.value = []
            for x in range(self.array_length[0]):
                ytmp = []
                for y in range(self.array_length[1]):
                    ytmp.append(self.generate_string())
                self.value.append(ytmp)
        elif self.type == THREEDSTRING:
            self.value = []
            for x in range(self.array_length[0]):
                ytmp = []
                for y in range(self.array_length[1]):
                    ztmp = []
                    for z in range(self.array_length[2]):
                        ztmp.append(self.generate_string())
                    ytmp.append(ztmp)
                self.value.append(ytmp)

    def get_value(self):
        return self.value

    def get_name(self):
        return self.name

    def get_dic(self):
        return {"name": self.name, "value": self.value}
