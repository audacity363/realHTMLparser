#!/bin/python3
from bs4 import BeautifulSoup
import tempfile
import os
import subprocess
import sys
import json
import py_vars

def getTempFilename():
    name = next(tempfile._get_candidate_names())
    name = os.path.join(tempfile.gettempdir(), name)
    return name

def diffstrs(str1, str2):
    if len(str1) >= len(str2):
        length = len(str2)
    else:
        length = len(str1)
    diffs = [i for i in range(length) if str1[i] != str2[i]]
    return diffs


template = """
<html>
<body>
<div id="for1">
{% for test in var2 %}
<p class="child1">{{ test }}</p>
{% end-for %}
</div>
</body>
</html>
"""

tmp = []

for x in range(4):
    tmp1 = []
    for y in range(6):
        tmp1.append("test"+str(x)+str(y))
    tmp.append(tmp1)

variables = py_vars.Variables()
variables.add_new_var(py_vars.definition.TWODSTRING, tmp, [4, 6, 0])

json_str = variables.gen_json()

template_path = getTempFilename()
output_path = getTempFilename()
json_path = getTempFilename()

#print("JSON: [%s]" % json)

with open(json_path, "w") as json_file:
    json_file.write(json_str)

with open(template_path, "w") as template_file:
    template_file.write(template)

output = subprocess.check_output(["./tests/automatic_tests/parser_test", template_path, output_path, json_path])
print("======================================================================")
print(output.decode("utf-8"))
print("======================================================================")

outputfile = open(output_path, "r")
html = outputfile.read()
print("======================================================================")
print(html)
print("======================================================================")
outputfile.close()

soup = BeautifulSoup(html, "html.parser")
for_loop = soup.find(id="for1")
for_childs = for_loop.find_all("p")

data = variables.get_value("var2")

if len(for_childs) != len(data):
    print("Length does not match")
    sys.exit(1)
i=0
for entry in for_childs:
    result = diffstrs(entry.get_text(), json.dumps(data[i]))
    if len(result) > 0:
        print("Index %d does not match data! ([%s] != [%s])" % (i, entry.get_text(), json.dumps(data[i])))
        print("parser: [%s]\npython: [%s]\n" % (entry.get_text()[result[0]:], json.dumps(data[i])[result[0]:]))
        sys.exit(1)
    i+=1

print("Everything is OK!")
