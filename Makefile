all:
	g++ *.cpp -o SLang
encode:
	enca -L zh_CN -x UTF-8 ./Data/*.txt
