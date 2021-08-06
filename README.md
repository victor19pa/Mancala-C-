## Como correrlo
# ubicarse en la carpeta mancala
cd mancala
# crear carpeta y cambiar directorio a build
mkdir build && cd build

# instalar dependencias
sudo apt install cmake
cmake ..
make
./mancala
