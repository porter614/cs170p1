for line in open("output.txt", "r"):
	words = line.split(":")

	for word in words:
		print word