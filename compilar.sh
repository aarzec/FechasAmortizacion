shopt -s globstar
allcpps=$(find . -name "*.cpp")
g++ -Wall -g -o app.out $allcpps
