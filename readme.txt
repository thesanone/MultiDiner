Чтоб собрать нужно:
+ Qt Qmake
   тестировалось на компиляторах minGW и GCC, свежих на март
+ graphviz
  для linux:
	sudo apt-get graphviz (кажется, но в пакетном менеджере он точно есть)
  для Windows
   брать его тут http://www.graphviz.org
   необходимо создать переменную среды с именем "DOT_DIR" (без кавчек), например
   setx DOT_DIR "путь к папке, например "C:\graphviz" (без кавычек), в приведенном примере правильно будет setx DOT_DIR C:\graphviz
