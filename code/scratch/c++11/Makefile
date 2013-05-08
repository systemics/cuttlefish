_: ; @perl -e 'map { /(.*)\./; if (! -e $$1 or ((stat($$1))[9] < (stat($$_))[9])) { print qx(g++ -std=c++0x -o $$1 $$_); } } glob "*.cpp";'
clean: ; @find . -executable -type f -exec rm {} \;
