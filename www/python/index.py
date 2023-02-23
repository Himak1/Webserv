#!/usr/bin/python
import cgi

def read_and_print(src):
	text_file = open(src, "r")
	data = text_file.read()
	text_file.close()
	print (data)

print ("""<!doctype html><html><head><meta charset="utf-8"><title>webserv</title>
		<link rel='stylesheet' type='text/css' href='../templates/style.css'>
		</head><body><div id="content"><div id="top"><a href="/"><h1>webserv</h1></a></div>""")

print ('''<div id="middle"><h3 class="">PYTHON</h3>4.2 * 10 =''')
print (4.2 * 10)
print ('''</div>''')

read_and_print("www/templates/bottom.php")
