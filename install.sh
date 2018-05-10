#! /bin/bash

cd ./src
make && make install
#sudo mkdir -p /usr/share/man/man1
sudo install -g 0 -o 0 -m 0644 ../mp/leapfrog.1 /usr/share/man/man1/
sudo install -g 0 -o 0 -m 0644 ../mp/leapfrog.1 /usr/share/man/ru/man1
sudo gzip /usr/share/man/man1/leapfrog.1
sudo gzip /usr/share/man/ru/man1/leapfrog.1
