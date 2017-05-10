#!/bin/python3
from bs4 import BeautifulSoup
import tempfile
import os
import subprocess
import sys

def getTempFilename():
    name = next(tempfile._get_candidate_names())
    name = os.path.join(tempfile.gettempdir(), name)
    return name

template = """
<html>
<body>
<div id="for1">
{% for test in testgrp.test1 %}
<p class="child1">{{ test }}</p>
{% end-for %}
</div>
</body>
</html>
"""
data = [0, 0, 6, 0, 0]

template_path = getTempFilename()
output_path = getTempFilename()

with open(template_path, "w") as template_file:
    template_file.write(template)

output = subprocess.check_output(["../../main", template_path, output_path])
print("======================================================================")
print(output)
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

if len(for_childs) != len(data):
    print("Length does not match")
    sys.exit(1)
i=0
for entry in for_childs:
    if entry.get_text() != str(data[i]):
        print("Index %d does not match data! ([%s] != [%s])" % (i, entry.get_text(), str(data[i])))
        sys.exit(1)
    i+=1

print("Evrything is OK!")
