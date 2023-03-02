#!/usr/bin/python
import cgi

def read_and_print(src):
	text_file = open(src, "r")
	data = text_file.read()
	text_file.close()
	print (data)

read_and_print("www/templates/top.php")

print ('''<div id="middle"><h3 class="">PYTHON</h3>4.2 * 10 =''')
print (4.2 * 10)
print ('''</div>''')

read_and_print("www/templates/bottom.php")
