txt = "hello>test>max>".encode('ascii')

parse = txt.decode('utf8').split('>')[2]
#begint bij 0, dus 2
#['hello', 'test', 'max', ''] is uitkomst zonder 2
print(parse)